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

#ifndef NAVMESHTESTERTOOL_H
#define NAVMESHTESTERTOOL_H

#include <string>
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include <filesystem>

namespace ya
{
	class NavMeshTool
	{

		NavMeshTool();
		struct NavMeshSetHeader
		{
			int magic;
			int version;
			int numTiles;
			dtNavMeshParams params;
		};
		struct NavMeshTileHeader
		{
			dtTileRef tileRef;
			int dataSize;
		};
		static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
		static const int NAVMESHSET_VERSION = 1;

		dtNavMesh* m_navMesh;
		dtNavMeshQuery* m_navQuery;

		dtQueryFilter m_filter;

		dtStatus m_pathFindStatus;

		int m_straightPathOptions;

		static const int MAX_POLYS = 256;
		static const int MAX_SMOOTH = 2048;

		dtPolyRef m_startRef;
		dtPolyRef m_endRef;
		dtPolyRef m_polys[MAX_POLYS];
		dtPolyRef m_parent[MAX_POLYS];
		int m_npolys;
		float m_straightPath[MAX_POLYS * 3];
		unsigned char m_straightPathFlags[MAX_POLYS];
		dtPolyRef m_straightPathPolys[MAX_POLYS];
		int m_nstraightPath;
		float m_polyPickExt[3];
		float m_smoothPath[MAX_SMOOTH * 3];
		int m_nsmoothPath;
		float m_queryPoly[4 * 3];

		static const int MAX_RAND_POINTS = 64;
		float m_randPoints[MAX_RAND_POINTS * 3];
		int m_nrandPoints;
		bool m_randPointsInCircle;

		float m_spos[3];
		float m_epos[3];
		float m_hitPos[3];
		float m_hitNormal[3];
		bool m_hitResult;
		float m_distanceToWall;
		float m_neighbourhoodRadius;
		float m_randomRadius;
		bool m_sposSet;
		bool m_eposSet;

		int m_pathIterNum;
		dtPolyRef m_pathIterPolys[MAX_POLYS];
		int m_pathIterPolyCount;
		float m_prevIterPos[3], m_iterPos[3], m_steerPos[3], m_targetPos[3];

		static const int MAX_STEER_POINTS = 10;
		float m_steerPoints[MAX_STEER_POINTS * 3];
		int m_steerPointCount;

		class Transform* m_mapMeshTr;
		//싱글톤
		static NavMeshTool* m_pInst;

		class Scene* m_scene;
	public:
		static NavMeshTool* GetInst();
		static void DestroyInst();
		void Init(class Scene*);
		void reset();
		
		void recalc();
		void Load(const std::wstring& path);
		void SetMapMeshTr(class Transform* tr) { m_mapMeshTr = tr; }
		class Transform* GetMapMeshTr() { return m_mapMeshTr; }
		
		void SetStartPos(float* pos) { m_spos[0] = pos[0]; m_spos[1] = pos[1]; m_spos[2] = pos[2]; }
		void SetEndPos(float* pos) { m_epos[0] = pos[0]; m_epos[1] = pos[1]; m_epos[2] = pos[2]; }
		
		float* GetPath() { return m_smoothPath; }
		int GetPathSize() { return m_nsmoothPath; }
	};
}
#endif // NAVMESHTESTERTOOL_H

