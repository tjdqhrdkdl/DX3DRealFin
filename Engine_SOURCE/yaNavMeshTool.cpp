//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yaNavMeshTool.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourCommon.h"
#include "Recast.h"
#include "RecastDebugDraw.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourDebugDraw.h"
#include "DetourCommon.h"
#include "yaMesh.h"
#include "yaRenderer.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaGameObject.h"
#include "yaResources.h"
#ifdef WIN32
#	define snprintf _snprintf
#endif

// Uncomment this to dump all the requests in stdout.
#define DUMP_REQS
namespace ya
{
	NavMeshTool* NavMeshTool::m_pInst;

	enum SamplePolyFlags
	{
		SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
		SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
		SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
		SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
		SAMPLE_POLYFLAGS_DISABLED = 0x10,		// Disabled polygon
		SAMPLE_POLYFLAGS_ALL = 0xffff	// All abilities.
	};
	enum SamplePolyAreas
	{
		SAMPLE_POLYAREA_GROUND,
		SAMPLE_POLYAREA_WATER,
		SAMPLE_POLYAREA_ROAD,
		SAMPLE_POLYAREA_DOOR,
		SAMPLE_POLYAREA_GRASS,
		SAMPLE_POLYAREA_JUMP
	};
	// Returns a random number [0..1]
	static float frand()
	{
		//	return ((float)(rand() & 0xffff)/(float)0xffff);
		return (float)rand() / (float)RAND_MAX;
	}

	inline bool inRange(const float* v1, const float* v2, const float r, const float h)
	{
		const float dx = v2[0] - v1[0];
		const float dy = v2[1] - v1[1];
		const float dz = v2[2] - v1[2];
		return (dx * dx + dz * dz) < r * r && fabsf(dy) < h;
	}


	static int fixupCorridor(dtPolyRef* path, const int npath, const int maxPath,
		const dtPolyRef* visited, const int nvisited)
	{
		int furthestPath = -1;
		int furthestVisited = -1;

		// Find furthest common polygon.
		for (int i = npath - 1; i >= 0; --i)
		{
			bool found = false;
			for (int j = nvisited - 1; j >= 0; --j)
			{
				if (path[i] == visited[j])
				{
					furthestPath = i;
					furthestVisited = j;
					found = true;
				}
			}
			if (found)
				break;
		}

		// If no intersection found just return current path. 
		if (furthestPath == -1 || furthestVisited == -1)
			return npath;

		// Concatenate paths.	

		// Adjust beginning of the buffer to include the visited.
		const int req = nvisited - furthestVisited;
		const int orig = rcMin(furthestPath + 1, npath);
		int size = rcMax(0, npath - orig);
		if (req + size > maxPath)
			size = maxPath - req;
		if (size)
			memmove(path + req, path + orig, size * sizeof(dtPolyRef));

		// Store visited
		for (int i = 0; i < req; ++i)
			path[i] = visited[(nvisited - 1) - i];

		return req + size;
	}

	// This function checks if the path has a small U-turn, that is,
	// a polygon further in the path is adjacent to the first polygon
	// in the path. If that happens, a shortcut is taken.
	// This can happen if the target (T) location is at tile boundary,
	// and we're (S) approaching it parallel to the tile edge.
	// The choice at the vertex can be arbitrary, 
	//  +---+---+
	//  |:::|:::|
	//  +-S-+-T-+
	//  |:::|   | <-- the step can end up in here, resulting U-turn path.
	//  +---+---+
	static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
	{
		if (npath < 3)
			return npath;

		// Get connected polygons
		static const int maxNeis = 16;
		dtPolyRef neis[maxNeis];
		int nneis = 0;

		const dtMeshTile* tile = 0;
		const dtPoly* poly = 0;
		if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
			return npath;

		for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
		{
			const dtLink* link = &tile->links[k];
			if (link->ref != 0)
			{
				if (nneis < maxNeis)
					neis[nneis++] = link->ref;
			}
		}

		// If any of the neighbour polygons is within the next few polygons
		// in the path, short cut to that polygon directly.
		static const int maxLookAhead = 6;
		int cut = 0;
		for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
			for (int j = 0; j < nneis; j++)
			{
				if (path[i] == neis[j]) {
					cut = i;
					break;
				}
			}
		}
		if (cut > 1)
		{
			int offset = cut - 1;
			npath -= offset;
			for (int i = 1; i < npath; i++)
				path[i] = path[i + offset];
		}

		return npath;
	}

	static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
		const float minTargetDist,
		const dtPolyRef* path, const int pathSize,
		float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
		float* outPoints = 0, int* outPointCount = 0)
	{
		// Find steer target.
		static const int MAX_STEER_POINTS = 3;
		float steerPath[MAX_STEER_POINTS * 3];
		unsigned char steerPathFlags[MAX_STEER_POINTS];
		dtPolyRef steerPathPolys[MAX_STEER_POINTS];
		int nsteerPath = 0;
		navQuery->findStraightPath(startPos, endPos, path, pathSize,
			steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
		if (!nsteerPath)
			return false;

		if (outPoints && outPointCount)
		{
			*outPointCount = nsteerPath;
			for (int i = 0; i < nsteerPath; ++i)
				dtVcopy(&outPoints[i * 3], &steerPath[i * 3]);
		}


		// Find vertex far enough to steer to.
		int ns = 0;
		while (ns < nsteerPath)
		{
			// Stop at Off-Mesh link or when point is further than slop away.
			if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
				!inRange(&steerPath[ns * 3], startPos, minTargetDist, 1000.0f))
				break;
			ns++;
		}
		// Failed to find good point to steer to.
		if (ns >= nsteerPath)
			return false;

		dtVcopy(steerPos, &steerPath[ns * 3]);
		steerPos[1] = startPos[1];
		steerPosFlag = steerPathFlags[ns];
		steerPosRef = steerPathPolys[ns];

		return true;
	}


	NavMeshTool::NavMeshTool() :
		m_navMesh(0),
		m_navQuery(0),
		m_pathFindStatus(DT_FAILURE),
		m_straightPathOptions(0),
		m_startRef(0),
		m_endRef(0),
		m_npolys(0),
		m_nstraightPath(0),
		m_nsmoothPath(0),
		m_nrandPoints(0),
		m_randPointsInCircle(false),
		m_hitResult(false),
		m_distanceToWall(0),
		m_sposSet(true),
		m_eposSet(true),
		m_pathIterNum(0),
		m_pathIterPolyCount(0),
		m_steerPointCount(0)
	{
		m_filter.setIncludeFlags(SAMPLE_POLYFLAGS_ALL ^ SAMPLE_POLYFLAGS_DISABLED);
		m_filter.setExcludeFlags(0);

		m_polyPickExt[0] = 2;
		m_polyPickExt[1] = 4;
		m_polyPickExt[2] = 2;

		m_neighbourhoodRadius = 2.5f;
		m_randomRadius = 5.0f;
	}

	//void NavMeshTool::init(Sample* sample)
	//{
	//	m_sample = sample;
	//	m_navMesh = sample->getNavMesh();
	//	m_navQuery = sample->getNavMeshQuery();
	//	recalc();
	//
	//	if (m_navQuery)
	//	{
	//		// Change costs.
	//		m_filter.setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);
	//		m_filter.setAreaCost(SAMPLE_POLYAREA_WATER, 10.0f);
	//		m_filter.setAreaCost(SAMPLE_POLYAREA_ROAD, 1.0f);
	//		m_filter.setAreaCost(SAMPLE_POLYAREA_DOOR, 1.0f);
	//		m_filter.setAreaCost(SAMPLE_POLYAREA_GRASS, 2.0f);
	//		m_filter.setAreaCost(SAMPLE_POLYAREA_JUMP, 1.5f);
	//	}
	//
	//	m_neighbourhoodRadius = sample->getAgentRadius() * 20.0f;
	//	m_randomRadius = sample->getAgentRadius() * 30.0f;
	//}

	NavMeshTool* NavMeshTool::GetInst()
	{
		if (!m_pInst)
			m_pInst = new NavMeshTool();
		return m_pInst;
	}

	void NavMeshTool::DestroyInst()
	{
		if (!m_pInst)
			return;
		dtFreeNavMesh(m_pInst->m_navMesh);
		dtFreeNavMeshQuery(m_pInst->m_navQuery);
		delete m_pInst;
		m_pInst = nullptr;
	}

	void NavMeshTool::Init()
	{
		Load(L"Map\\solo_navmesh.bin");
		if (m_navQuery)
		{
			// Change costs.
			m_filter.setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);
			m_filter.setAreaCost(SAMPLE_POLYAREA_WATER, 10.0f);
			m_filter.setAreaCost(SAMPLE_POLYAREA_ROAD, 1.0f);
			m_filter.setAreaCost(SAMPLE_POLYAREA_DOOR, 1.0f);
			m_filter.setAreaCost(SAMPLE_POLYAREA_GRASS, 2.0f);
			m_filter.setAreaCost(SAMPLE_POLYAREA_JUMP, 1.5f);
		}
		m_neighbourhoodRadius = 20.0f;
		m_randomRadius = 30.0f;
	}

	void NavMeshTool::reset()
	{
		m_startRef = 0;
		m_endRef = 0;
		m_npolys = 0;
		m_nstraightPath = 0;
		m_nsmoothPath = 0;
		memset(m_hitPos, 0, sizeof(m_hitPos));
		memset(m_hitNormal, 0, sizeof(m_hitNormal));
		m_distanceToWall = 0;
	}


	void NavMeshTool::recalc()
	{
		if (!m_navMesh)
			return;

		if (m_sposSet)
			m_navQuery->findNearestPoly(m_spos, m_polyPickExt, &m_filter, &m_startRef, 0);
		else
			m_startRef = 0;

		if (m_eposSet)
			m_navQuery->findNearestPoly(m_epos, m_polyPickExt, &m_filter, &m_endRef, 0);
		else
			m_endRef = 0;

		m_pathFindStatus = DT_FAILURE;


		m_pathIterNum = 0;
		if (m_sposSet && m_eposSet && m_startRef && m_endRef)
		{
#ifdef DUMP_REQS
			printf("pi  %f %f %f  %f %f %f  0x%x 0x%x\n",
				m_spos[0], m_spos[1], m_spos[2], m_epos[0], m_epos[1], m_epos[2],
				m_filter.getIncludeFlags(), m_filter.getExcludeFlags());
#endif

			m_navQuery->findPath(m_startRef, m_endRef, m_spos, m_epos, &m_filter, m_polys, &m_npolys, MAX_POLYS);

			m_nsmoothPath = 0;

			if (m_npolys)
			{
				// Iterate over the path to find smooth path on the detail mesh surface.
				dtPolyRef polys[MAX_POLYS];
				memcpy(polys, m_polys, sizeof(dtPolyRef) * m_npolys);
				int npolys = m_npolys;

				float iterPos[3], targetPos[3];
				m_navQuery->closestPointOnPoly(m_startRef, m_spos, iterPos, 0);
				m_navQuery->closestPointOnPoly(polys[npolys - 1], m_epos, targetPos, 0);

				static const float STEP_SIZE = 0.5f;
				static const float SLOP = 0.01f;

				m_nsmoothPath = 0;

				dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
				m_nsmoothPath++;

				// Move towards target a small advancement at a time until target reached or
				// when ran out of memory to store the path.
				while (npolys && m_nsmoothPath < MAX_SMOOTH)
				{
					// Find location to steer towards.
					float steerPos[3];
					unsigned char steerPosFlag;
					dtPolyRef steerPosRef;

					if (!getSteerTarget(m_navQuery, iterPos, targetPos, SLOP,
						polys, npolys, steerPos, steerPosFlag, steerPosRef))
						break;

					bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
					bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;

					// Find movement delta.
					float delta[3], len;
					dtVsub(delta, steerPos, iterPos);
					len = dtMathSqrtf(dtVdot(delta, delta));
					// If the steer target is end of path or off-mesh link, do not move past the location.
					if ((endOfPath || offMeshConnection) && len < STEP_SIZE)
						len = 1;
					else
						len = STEP_SIZE / len;
					float moveTgt[3];
					dtVmad(moveTgt, iterPos, delta, len);

					// Move
					float result[3];
					dtPolyRef visited[16];
					int nvisited = 0;
					m_navQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &m_filter,
						result, visited, &nvisited, 16);

					npolys = fixupCorridor(polys, npolys, MAX_POLYS, visited, nvisited);
					npolys = fixupShortcuts(polys, npolys, m_navQuery);

					float h = 0;
					m_navQuery->getPolyHeight(polys[0], result, &h);
					result[1] = h;
					dtVcopy(iterPos, result);

					// Handle end of path and off-mesh links when close enough.
					if (endOfPath && inRange(iterPos, steerPos, SLOP, 1.0f))
					{
						// Reached end of path.
						dtVcopy(iterPos, targetPos);
						if (m_nsmoothPath < MAX_SMOOTH)
						{
							dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
							m_nsmoothPath++;
						}
						break;
					}
					else if (offMeshConnection && inRange(iterPos, steerPos, SLOP, 1.0f))
					{
						// Reached off-mesh connection.
						float startPos[3], endPos[3];

						// Advance the path up to and over the off-mesh connection.
						dtPolyRef prevRef = 0, polyRef = polys[0];
						int npos = 0;
						while (npos < npolys && polyRef != steerPosRef)
						{
							prevRef = polyRef;
							polyRef = polys[npos];
							npos++;
						}
						for (int i = npos; i < npolys; ++i)
							polys[i - npos] = polys[i];
						npolys -= npos;

						// Handle the connection.
						dtStatus status = m_navMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
						if (dtStatusSucceed(status))
						{
							if (m_nsmoothPath < MAX_SMOOTH)
							{
								dtVcopy(&m_smoothPath[m_nsmoothPath * 3], startPos);
								m_nsmoothPath++;
								// Hack to make the dotted path not visible during off-mesh connection.
								if (m_nsmoothPath & 1)
								{
									dtVcopy(&m_smoothPath[m_nsmoothPath * 3], startPos);
									m_nsmoothPath++;
								}
							}
							// Move position at the other side of the off-mesh link.
							dtVcopy(iterPos, endPos);
							float eh = 0.0f;
							m_navQuery->getPolyHeight(polys[0], iterPos, &eh);
							iterPos[1] = eh;
						}
					}

					// Store results.
					if (m_nsmoothPath < MAX_SMOOTH)
					{
						dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
						m_nsmoothPath++;
					}
				}
			}

		}
		else
		{
			m_npolys = 0;
			m_nsmoothPath = 0;
		}

	}

	void NavMeshTool::Load(const std::wstring& path)
	{
		std::string strPath(path.begin(), path.end());
		std::filesystem::path fullPath = std::filesystem::absolute(L"..\\Resources") / path;

		std::wstring name = std::filesystem::path(path).stem();
		name += L".bin";
		std::wstring p = fullPath.c_str();
		FILE* fp = nullptr;
		_wfopen_s(&fp, fullPath.c_str(), L"rb");
		if (!fp) return;

		// Read header.
		NavMeshSetHeader header;
		size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
		if (readLen != 1)
		{
			fclose(fp);
			return;
		}
		if (header.magic != NAVMESHSET_MAGIC)
		{
			fclose(fp);
			return;
		}
		if (header.version != NAVMESHSET_VERSION)
		{
			fclose(fp);
			return;
		}
		dtNavMesh* mesh = dtAllocNavMesh();
		if (!mesh)
		{
			fclose(fp);
			return;
		}
		dtStatus status = mesh->init(&header.params);
		if (dtStatusFailed(status))
		{
			fclose(fp);
			return;
		}

		// Read tiles.
		for (int i = 0; i < header.numTiles; ++i)
		{
			NavMeshTileHeader tileHeader;
			readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
			if (readLen != 1)
			{
				fclose(fp);
				return;
			}

			if (!tileHeader.tileRef || !tileHeader.dataSize)
				break;

			unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
			if (!data) break;
			memset(data, 0, tileHeader.dataSize);
			readLen = fread(data, tileHeader.dataSize, 1, fp);
			if (readLen != 1)
			{
				dtFree(data);
				fclose(fp);
				return;
			}

			mesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
		}

		fclose(fp);
		m_navMesh = mesh;
		m_navQuery = dtAllocNavMeshQuery();
		if (dtStatusFailed(m_navQuery->init(mesh, 2048)))
			return;


		// Crate Debug Mesh
		// 디버그 메쉬 끄려면 이 밑으로 주석.
		//for (int i = 0; i < mesh->getMaxTiles(); ++i)
		//{
		//	const dtMeshTile* tile = mesh->getTile(i);

		//	std::vector<ya::renderer::Vertex> vtxs;
		//	tile->verts; //이게 버텍스 덩어리임.
		//	float* a = &tile->verts[tile->header->polyCount * 3];
		//	float cur = a[0];
		//	cur = a[1];
		//	cur = a[2];
		//	const float* orig = m_navMesh->getParams()->orig;
		//	for (size_t i = 0; i < 10000; i++)
		//	{
		//		ya::renderer::Vertex v;
		//		float* a = &tile->verts[i * 3];
		//		v.pos = Vector4(a[0], a[2], a[1], 1);
		//		v.color = Vector4(1, 0, 1, 1);
		//		vtxs.push_back(v);
		//	}
		//	for (size_t i = 0; i < 10000; i++)
		//	{
		//		ya::renderer::Vertex v;
		//		float* a = &tile->detailVerts[i * 3];
		//		v.pos = Vector4(a[0], a[2], a[1], 1);
		//		v.color = Vector4(1, 0, 1, 1);
		//		vtxs.push_back(v);
		//	}

		//	std::shared_ptr<Mesh> navMesh = std::make_shared<Mesh>();
		//	navMesh->CreateVertexBuffer(vtxs.data(), vtxs.size());
		//	Microsoft::WRL::ComPtr<ID3D11Buffer> vtxBuffer = navMesh->GetVertexBuffer();

		//	std::vector<UINT> indexes;

		//	for (int i = 0; i < tile->header->polyCount; ++i)
		//	{
		//		const dtPoly* p = &tile->polys[i];
		//		if (p->getType() == DT_POLYTYPE_OFFMESH_CONNECTION)	// Skip off-mesh links.
		//			continue;

		//		const dtPolyDetail* pd = &tile->detailMeshes[i];
		//		std::shared_ptr<Mesh> polyMesh = std::make_shared<Mesh>();
		//		polyMesh->SetVertexBuffer(vtxBuffer);
		//		
		//		for (int j = 0; j < pd->triCount; ++j)
		//		{
		//			const unsigned char* t = &tile->detailTris[(pd->triBase + j) * 4];
		//			for (int k = 0; k < 3; ++k)
		//			{
		//				if (t[k] < p->vertCount)

		//					indexes.push_back(p->verts[t[k]]);
		//				//인덱싱 중임
		//				//여기있는 버텍스, 그릴 차례!
		//				//삼각형 그릴거임.
		//				//3개씩. 버텍스 계속 알려주께!
		//				//p->verts[t[k]] * 3이 너가 그릴 버텍스의 포지션 주소임.
		//				//p->verts[t[0]],p->verts[t[1]],p->verts[t[2]] 각각 버텍스를 가리킴 
		//				//*3을 해주는 이유는 버텍스의 xyz좌표가 들어있기 때문이다.
		//				else
		//					indexes.push_back(10000 + (pd->vertBase + t[k] - p->vertCount));
		//					//dd->vertex(&tile->detailVerts[(pd->vertBase + t[k] - p->vertCount) * 3], col);
		//					
		//			}
		//		}
		//		if (indexes.size() > 0)
		//		{
		//			polyMesh->CreateIndexBuffer(indexes.data(), indexes.size());
		//			indexes.clear();
		//			GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground);
		//			ground->SetName(L"ss");
		//			MeshRenderer* groundRenderer = ground->AddComponent<MeshRenderer>();
		//			groundRenderer->SetMesh(polyMesh);
		//			groundRenderer->SetMaterial(Resources::Find<Material>(L"DebugPaintedMaterial"), 0);

		//			if (m_mapMeshTr)
		//			{
		//				ground->GetComponent<Transform>()->SetParent(m_mapMeshTr);
		//			}
		//		}
		//	}
		//}


	}

	static void getPolyCenter(dtNavMesh* navMesh, dtPolyRef ref, float* center)
	{
		center[0] = 0;
		center[1] = 0;
		center[2] = 0;

		const dtMeshTile* tile = 0;
		const dtPoly* poly = 0;
		dtStatus status = navMesh->getTileAndPolyByRef(ref, &tile, &poly);
		if (dtStatusFailed(status))
			return;

		for (int i = 0; i < (int)poly->vertCount; ++i)
		{
			const float* v = &tile->verts[poly->verts[i] * 3];
			center[0] += v[0];
			center[1] += v[1];
			center[2] += v[2];
		}
		const float s = 1.0f / poly->vertCount;
		center[0] *= s;
		center[1] *= s;
		center[2] *= s;
	}


}