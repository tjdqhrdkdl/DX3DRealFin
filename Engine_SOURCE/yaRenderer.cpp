#include "yaRenderer.h"
#include "yaTime.h"
#include "yaResources.h"
#include "yaMaterial.h"
#include "yaSceneManager.h"
#include "yaPaintShader.h"
#include "yaParticleShader.h"
#include "yaApplication.h"
#include "yaBoneShader.h"
extern ya::Application application;

namespace ya::renderer
{
	Vertex vertexes[4] = {};
	ConstantBuffer* constantBuffers[(UINT)eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)eRSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthstencilStates[(UINT)eDSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)eBSType::End] = {};
	
	graphics::MultiRenderTarget* renderTargets[(UINT)eRTType::End] = {};

	Camera* mainCamera = nullptr;
	Camera* UICamera = nullptr;
	std::vector<Camera*> cameras[(UINT)eSceneType::End];
	std::vector<DebugMesh> debugMeshes;
	std::vector<Light*> lights;
	std::vector<LightAttribute> lightAttributes;
	ya::graphics::StructedBuffer* lightsBuffer = nullptr;

	std::shared_ptr<Texture> postProcessTexture = nullptr;

	ya::GameObject* inspectorGameObject = nullptr;

	void LoadMesh()
	{
		#pragma region POINT MESH
		Vertex v = {};
		std::shared_ptr<Mesh> pointMesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"PointMesh", pointMesh);
		pointMesh->CreateVertexBuffer(&v, 1);
		UINT pointIndex = 0;
		pointMesh->CreateIndexBuffer(&pointIndex, 1);
		#pragma endregion
		#pragma region RECT MESH
		//RECT
		vertexes[0].pos = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);
		vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);
		vertexes[0].uv = Vector2(0.f, 0.f);

		vertexes[1].pos = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
		vertexes[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
		vertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
		vertexes[3].color = Vector4(0.f, 0.f, 1.f, 1.f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		// Crate Mesh
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"RectMesh", mesh);
		mesh->CreateVertexBuffer(vertexes, 4);

		std::vector<UINT> indexes;
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);
		indexes.push_back(0);
		mesh->CreateIndexBuffer(indexes.data(), indexes.size());
#pragma endregion
		#pragma region DEBUG RECTMESH
		vertexes[0].pos = Vector4(-0.5f, 0.5f, -0.00001f, 1.0f);
		vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);
		vertexes[0].uv = Vector2(0.f, 0.f);

		vertexes[1].pos = Vector4(0.5f, 0.5f, -0.00001f, 1.0f);
		vertexes[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector4(0.5f, -0.5f, -0.00001f, 1.0f);
		vertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector4(-0.5f, -0.5f, -0.00001f, 1.0f);
		vertexes[3].color = Vector4(0.f, 0.f, 1.f, 1.f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		// Crate Mesh
		std::shared_ptr<Mesh> debugmesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"DebugRectMesh", debugmesh);
		debugmesh->CreateVertexBuffer(vertexes, 4);
		debugmesh->CreateIndexBuffer(indexes.data(), indexes.size());
#pragma endregion
		#pragma region CIRCLE MESH
		std::vector<Vertex> circleVtxes;
		Vertex center = {};
		center.pos = Vector4(0.0f, 0.0f, -0.00001f, 1.0f);
		center.color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		center.uv = Vector2::Zero;

		circleVtxes.push_back(center);

		int iSlice = 80;
		float fRadius = 0.5f;
		float fTheta = XM_2PI / (float)iSlice;


		for (size_t i = 0; i < iSlice; i++)
		{
			Vertex vtx = {};
			vtx.pos = Vector4
			(
				fRadius * cosf(fTheta * (float)i)
				, fRadius * sinf(fTheta * (float)i)
				, -0.00001f, 1.0f
			);
			vtx.color = center.color;

			circleVtxes.push_back(vtx);
		}
		indexes.clear();
		for (UINT i = 0; i < (UINT)(iSlice - 2); i++)
		{
			indexes.push_back(i + 1u);
		}
		indexes.push_back(1);

		// Crate Mesh
		std::shared_ptr<Mesh> cirlceMesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"CircleMesh", cirlceMesh);
		cirlceMesh->CreateVertexBuffer(circleVtxes.data(), circleVtxes.size());
		cirlceMesh->CreateIndexBuffer(indexes.data(), indexes.size());
#pragma endregion
		#pragma region Cube Mesh
		Vertex arrCube[24] = {};

		//struct Vertex
		//{
		//	Vector4 pos;
		//	Vector4 color;
		//	Vector2 uv;
		//};

		// 윗면
		arrCube[0].pos = Vector4(-0.5f, 0.5f, 0.5f ,1.0f);
		arrCube[0].color = Vector4(1.f, 1.f, 1.f, 1.f);
		arrCube[0].uv = Vector2(0.f, 0.f);
		arrCube[0].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[0].normal = Vector3(0.f, 1.f, 0.f);
		arrCube[0].biNormal= Vector3(0.0f, 0.0f, 1.0f);

		arrCube[1].pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		arrCube[1].uv = Vector2(1.f, 0.f);

		arrCube[1].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[1].biNormal = Vector3(0.0f, 0.0f, 1.0f);
		arrCube[1].normal = Vector3(0.f, 1.f, 0.f);

		arrCube[2].pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[2].color = Vector4(1.f, 1.f, 1.f, 1.f);
		arrCube[2].uv = Vector2(0.f, 1.f);
		arrCube[2].normal = Vector3(0.f, 1.f, 0.f);
		arrCube[2].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[2].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		arrCube[3].pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[3].color = Vector4(1.f, 1.f, 1.f, 1.f);
		arrCube[3].uv = Vector2(1.f, 1.f);
		arrCube[3].normal = Vector3(0.f, 1.f, 0.f);
		arrCube[3].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[3].biNormal = Vector3(0.0f, 0.0f, 1.0f);


		// 아랫 면	
		arrCube[4].pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[4].color = Vector4(1.f, 0.f, 0.f, 1.f);
		arrCube[4].uv = Vector2(0.f, 0.f);
		arrCube[4].normal = Vector3(0.f, -1.f, 0.f);
		arrCube[4].tangent = Vector3(-1.0f, 0.0f, 0.0f);
		arrCube[4].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		arrCube[5].pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[5].color = Vector4(1.f, 0.f, 0.f, 1.f);
		arrCube[5].uv = Vector2(1.f, 0.f);
		arrCube[5].normal = Vector3(0.f, -1.f, 0.f);
		arrCube[5].tangent = Vector3(-1.0f, 0.0f, 0.0f);
		arrCube[5].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		arrCube[6].pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[6].color = Vector4(1.f, 0.f, 0.f, 1.f);
		arrCube[6].uv = Vector2(0.f, 1.f);
		arrCube[6].normal = Vector3(0.f, -1.f, 0.f);
		arrCube[6].tangent = Vector3(-1.0f, 0.0f, 0.0f);
		arrCube[6].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		arrCube[7].pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[7].color = Vector4(1.f, 0.f, 0.f, 1.f);
		arrCube[7].uv = Vector2(1.f, 1.f);
		arrCube[7].normal = Vector3(0.f, -1.f, 0.f);
		arrCube[7].tangent = Vector3(-1.0f, 0.0f, 0.0f);
		arrCube[7].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		// 왼쪽 면
		arrCube[8].pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[8].color = Vector4(0.f, 1.f, 0.f, 1.f);
		arrCube[8].uv = Vector2(0.f, 0.f);
		arrCube[8].normal = Vector3(-1.f, 0.f, 0.f);
		arrCube[8].tangent = Vector3(0.0f, 1.0f, 0.0f);
		arrCube[8].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		arrCube[9].pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[9].color = Vector4(0.f, 1.f, 0.f, 1.f);
		arrCube[9].uv = Vector2(1.f, 0.f);
		arrCube[9].normal = Vector3(-1.f, 0.f, 0.f);
		arrCube[9].tangent = Vector3(0.0f, 1.0f, 0.0f);
		arrCube[9].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		arrCube[10].pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[10].color = Vector4(0.f, 1.f, 0.f, 1.f);
		arrCube[10].uv = Vector2(0.f, 1.f);
		arrCube[10].normal = Vector3(-1.f, 0.f, 0.f);
		arrCube[10].tangent = Vector3(0.0f, 1.0f, 0.0f);
		arrCube[10].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		arrCube[11].pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[11].color = Vector4(0.f, 1.f, 0.f, 1.f);
		arrCube[11].uv = Vector2(1.f, 1.f);
		arrCube[11].normal = Vector3(-1.f, 0.f, 0.f);
		arrCube[11].tangent = Vector3(0.0f, 1.0f, 0.0f);
		arrCube[11].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		// 오른쪽 면
		arrCube[12].pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[12].color = Vector4(0.f, 0.f, 1.f, 1.f);
		arrCube[12].uv = Vector2(0.f, 0.f);
		arrCube[12].normal = Vector3(1.f, 0.f, 0.f);
		arrCube[12].tangent = Vector3(0.0f, -1.0f, 0.0f);
		arrCube[12].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		arrCube[13].pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[13].color = Vector4(0.f, 0.f, 1.f, 1.f);
		arrCube[13].uv = Vector2(1.f, 0.f);
		arrCube[13].normal = Vector3(1.f, 0.f, 0.f);
		arrCube[13].tangent = Vector3(0.0f, -1.0f, 0.0f);
		arrCube[13].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		arrCube[14].pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[14].color = Vector4(0.f, 0.f, 1.f, 1.f);
		arrCube[14].uv = Vector2(0.f, 1.f);
		arrCube[14].normal = Vector3(1.f, 0.f, 0.f);
		arrCube[14].tangent = Vector3(0.0f, -1.0f, 0.0f);
		arrCube[14].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		arrCube[15].pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[15].color = Vector4(0.f, 0.f, 1.f, 1.f);
		arrCube[15].uv = Vector2(1.f, 1.f);
		arrCube[15].normal = Vector3(1.f, 0.f, 0.f);
		arrCube[15].tangent = Vector3(0.0f, -1.0f, 0.0f);
		arrCube[15].biNormal = Vector3(0.0f, 0.0f, 1.0f);

		// 뒷 면
		arrCube[16].pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[16].color = Vector4(1.f, 1.f, 0.f, 1.f);
		arrCube[16].uv = Vector2(0.f, 0.f);
		arrCube[16].normal = Vector3(0.f, 0.f, 1.f);
		arrCube[16].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[16].biNormal = Vector3(0.0f, -1.0f, 1.0f);

		arrCube[17].pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[17].color = Vector4(1.f, 1.f, 0.f, 1.f);
		arrCube[17].uv = Vector2(1.f, 0.f);
		arrCube[17].normal = Vector3(0.f, 0.f, 1.f);
		arrCube[17].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[17].biNormal = Vector3(0.0f, -1.0f, 1.0f);

		arrCube[18].pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[18].color = Vector4(1.f, 1.f, 0.f, 1.f);
		arrCube[18].uv = Vector2(0.f, 1.f);
		arrCube[18].normal = Vector3(0.f, 0.f, 1.f);
		arrCube[18].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[18].biNormal = Vector3(0.0f, -1.0f, 1.0f);

		arrCube[19].pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[19].color = Vector4(1.f, 1.f, 0.f, 1.f);
		arrCube[19].uv = Vector2(1.f, 1.f);
		arrCube[19].normal = Vector3(0.f, 0.f, 1.f);
		arrCube[19].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[19].biNormal = Vector3(0.0f, -1.0f, 1.0f);

		// 앞 면
		arrCube[20].pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);;
		arrCube[20].color = Vector4(1.f, 0.f, 1.f, 1.f);
		arrCube[20].uv = Vector2(0.f, 0.f);
		arrCube[20].normal = Vector3(0.f, 0.f, -1.f);
		arrCube[20].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[20].biNormal = Vector3(0.0f, 1.0f, 1.0f);

		arrCube[21].pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[21].color = Vector4(1.f, 0.f, 1.f, 1.f);
		arrCube[21].uv = Vector2(1.f, 0.f);
		arrCube[21].normal = Vector3(0.f, 0.f, -1.f);
		arrCube[21].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[21].biNormal = Vector3(0.0f, 1.0f, 1.0f);

		arrCube[22].pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[22].color = Vector4(1.f, 0.f, 1.f, 1.f);
		arrCube[22].uv = Vector2(0.f, 1.f);
		arrCube[22].normal = Vector3(0.f, 0.f, -1.f);
		arrCube[22].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[22].biNormal = Vector3(0.0f, 1.0f, 1.0f);

		arrCube[23].pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[23].color = Vector4(1.f, 0.f, 1.f, 1.f);
		arrCube[23].uv = Vector2(1.f, 1.f);
		arrCube[23].normal = Vector3(0.f, 0.f, -1.f);
		arrCube[23].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[23].biNormal = Vector3(0.0f, 1.0f, 1.0f);

		indexes.clear();
		for (UINT i = 0; i < 6; i++)
		{
			indexes.push_back(i * 4u);
			indexes.push_back(i * 4u + 1u);
			indexes.push_back(i * 4u + 2u);

			indexes.push_back(i * 4u);
			indexes.push_back(i * 4u + 2u);
			indexes.push_back(i * 4u + 3u);
		}

		// Crate Mesh
		std::shared_ptr<Mesh> cubMesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"CubeMesh", cubMesh);
		cubMesh->CreateVertexBuffer(arrCube, 24);
		cubMesh->CreateIndexBuffer(indexes.data(), indexes.size());
		#pragma endregion
		#pragma region Sphere Mesh
		
		v = {};
		fRadius = 0.5f;
		std::vector<Vertex> sphereVtx;
	

		// Top
		v.pos = Vector4(0.0f, fRadius, 0.0f, 1.0f);
		v.uv = Vector2(0.5f, 0.f);
		v.color = Vector4(1.f, 1.f, 1.f, 1.f);
		v.normal = Vector3(0.0f, fRadius, 0.0f);
		v.normal.Normalize();
		v.tangent = Vector3(1.f, 0.f, 0.f);
		v.biNormal = Vector3(0.f, 0.f, 1.f);
		sphereVtx.push_back(v);

		// Body
		UINT iStackCount = 80; // 가로 분할 개수
		UINT iSliceCount = 80; // 세로 분할 개수

		float fStackAngle = XM_PI / iStackCount;
		float fSliceAngle = XM_2PI / iSliceCount;

		float fUVXStep = 1.f / (float)iSliceCount;
		float fUVYStep = 1.f / (float)iStackCount;

		for (UINT i = 1; i < iStackCount; ++i)
		{
			float phi = i * fStackAngle;

			for (UINT j = 0; j <= iSliceCount; ++j)
			{
				float theta = j * fSliceAngle;

				v.pos = Vector4(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
					, fRadius * cosf(i * fStackAngle)
					, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle), 1.0f);
				v.uv = Vector2(fUVXStep * j, fUVYStep * i);
				v.color = Vector4(1.f, 1.f, 1.f, 1.f);
				v.normal = Vector3(v.pos.x, v.pos.y, v.pos.z);
				//v.normal.Normalize();

				v.tangent.x = -fRadius * sinf(phi) * sinf(theta);
				v.tangent.y = 0.f;
				v.tangent.z = fRadius * sinf(phi) * cosf(theta);
				v.tangent.Normalize();

				v.tangent.Cross(v.normal, v.biNormal);
				v.biNormal.Normalize();

				sphereVtx.push_back(v);
			}
		}

		// Bottom
		v.pos = Vector4(0.f, -fRadius, 0.f, 1.0f);
		v.uv = Vector2(0.5f, 1.f);
		v.color = Vector4(1.f, 1.f, 1.f, 1.f);
		v.normal = Vector3(v.pos.x, v.pos.y, v.pos.z);
		v.normal.Normalize();

		v.tangent = Vector3(1.f, 0.f, 0.f);
		v.biNormal = Vector3(0.f, 0.f, -1.f);
		sphereVtx.push_back(v);

		// 인덱스
		// 북극점
		indexes.clear();
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			indexes.push_back(0);
			indexes.push_back(i + 2);
			indexes.push_back(i + 1);
		}

		// 몸통
		for (UINT i = 0; i < iStackCount - 2; ++i)
		{
			for (UINT j = 0; j < iSliceCount; ++j)
			{
				// + 
				// | \
				// +--+
				indexes.push_back((iSliceCount + 1) * (i)+(j)+1);
				indexes.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
				indexes.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

				// +--+
				//  \ |
				//    +
				indexes.push_back((iSliceCount + 1) * (i)+(j)+1);
				indexes.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
				indexes.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			}
		}

		// 남극점
		UINT iBottomIdx = (UINT)sphereVtx.size() - 1;
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			indexes.push_back(iBottomIdx);
			indexes.push_back(iBottomIdx - (i + 2));
			indexes.push_back(iBottomIdx - (i + 1));
		}

		std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"SphereMesh", sphereMesh);
		sphereMesh->CreateVertexBuffer(sphereVtx.data(), sphereVtx.size());
		sphereMesh->CreateIndexBuffer(indexes.data(), indexes.size());

		#pragma endregion
	}

	void LoadShader()
	{
#pragma region DEFAULT TRIANGLE SHADER
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");

		Resources::Insert<Shader>(L"RectShader", shader);
#pragma endregion
#pragma region SPRITE SHADER
		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		spriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		spriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		spriteShader->SetRSState(eRSType::SolidNone);
		spriteShader->SetDSState(eDSType::None);
		Resources::Insert<Shader>(L"SpriteShader", spriteShader);
#pragma endregion
#pragma region POSTURE SHADER
		std::shared_ptr<Shader> postureShader = std::make_shared<Shader>();
		postureShader->Create(eShaderStage::VS, L"PostureVS.hlsl", "main");
		postureShader->Create(eShaderStage::PS, L"PosturePS.hlsl", "main");
		postureShader->SetRSState(eRSType::SolidNone);
		postureShader->SetDSState(eDSType::None);
		Resources::Insert<Shader>(L"PostureShader", postureShader);
#pragma endregion


#pragma region UI SHADER
		std::shared_ptr<Shader> uiShader = std::make_shared<Shader>();
		uiShader->Create(eShaderStage::VS, L"UserInterfaceVS.hlsl", "main");
		uiShader->Create(eShaderStage::PS, L"UserInterfacePS.hlsl", "main");
		//uiShader->SetBSState(eBSType::AlphaBlend);

		Resources::Insert<Shader>(L"UIShader", uiShader);
#pragma endregion
#pragma region METER SHADER
		std::shared_ptr<Shader> meterShader = std::make_shared<Shader>();
		meterShader->Create(eShaderStage::VS, L"MeterVS.hlsl", "main");
		meterShader->Create(eShaderStage::PS, L"MeterPS.hlsl", "main");
		meterShader->SetDSState(eDSType::None);

		Resources::Insert<Shader>(L"MeterShader", meterShader);
#pragma endregion
#pragma region DangerUI SHADER
		std::shared_ptr<Shader> DangerUIShader = std::make_shared<Shader>();
		DangerUIShader->Create(eShaderStage::VS, L"DangerUIVS.hlsl", "main");
		DangerUIShader->Create(eShaderStage::PS, L"DangerUIPS.hlsl", "main");
		DangerUIShader->SetDSState(eDSType::None);

		Resources::Insert<Shader>(L"DangerUIShader", DangerUIShader);
#pragma endregion
#pragma region AlphaAnimation Shader
		std::shared_ptr<Shader> AlphaAnimationShader = std::make_shared<Shader>();
		AlphaAnimationShader->Create(eShaderStage::VS, L"AlphaAnimationVS.hlsl", "main");
		AlphaAnimationShader->Create(eShaderStage::PS, L"AlphaAnimationPS.hlsl", "main");
		AlphaAnimationShader->SetDSState(eDSType::None);

		Resources::Insert<Shader>(L"AlphaAnimationShader", AlphaAnimationShader);
#pragma endregion
#pragma region GRID SHADER
		std::shared_ptr<Shader> gridShader = std::make_shared<Shader>();
		gridShader->Create(eShaderStage::VS, L"GridVS.hlsl", "main");
		gridShader->Create(eShaderStage::PS, L"GridPS.hlsl", "main");
		gridShader->SetRSState(eRSType::SolidNone);
		gridShader->SetDSState(eDSType::NoWrite);
		gridShader->SetBSState(eBSType::AlphaBlend);

		Resources::Insert<Shader>(L"GridShader", gridShader);
#pragma endregion
#pragma region DEBUG SHADER
		std::shared_ptr<Shader> debugShader = std::make_shared<Shader>();
		debugShader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
		debugShader->Create(eShaderStage::PS, L"DebugPS.hlsl", "main");
		debugShader->SetRSState(eRSType::SolidNone);
		debugShader->SetDSState(eDSType::NoWrite);
		debugShader->SetBSState(eBSType::AlphaBlend);
		debugShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		Resources::Insert<Shader>(L"DebugShader", debugShader);
#pragma endregion
#pragma region DEBUG PAINTED SHADER
		std::shared_ptr<Shader> debugPaintedShader = std::make_shared<Shader>();
		debugPaintedShader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
		debugPaintedShader->Create(eShaderStage::PS, L"DebugPaintedPS.hlsl", "main");
		debugPaintedShader->SetRSState(eRSType::SolidNone);
		debugPaintedShader->SetBSState(eBSType::AlphaBlend);

		Resources::Insert<Shader>(L"DebugPaintedShader", debugPaintedShader);
#pragma endregion
#pragma region PAINT SHADER
		std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
		paintShader->Create(L"PaintCS.hlsl", "main");
		Resources::Insert<PaintShader>(L"PaintShader", paintShader);
#pragma endregion
#pragma region PARTICLE SHADER
		std::shared_ptr<Shader> particleShader = std::make_shared<Shader>();
		particleShader->Create(eShaderStage::VS, L"ParticleVS.hlsl", "main");
		particleShader->Create(eShaderStage::GS, L"ParticleGS.hlsl", "main");
		particleShader->Create(eShaderStage::PS, L"ParticlePS.hlsl", "main");
		particleShader->SetRSState(eRSType::SolidNone);
		particleShader->SetDSState(eDSType::NoWrite);
		particleShader->SetBSState(eBSType::AlphaBlend);
		particleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		Resources::Insert<Shader>(L"ParticleShader", particleShader);

		std::shared_ptr<ParticleShader> particleCS = std::make_shared<ParticleShader>();
		Resources::Insert<ParticleShader>(L"ParticleCS", particleCS);
		particleCS->Create(L"ParticleCS.hlsl", "main");
#pragma endregion
#pragma region POST PROCESS SHADER
		std::shared_ptr<Shader> postProcessShader = std::make_shared<Shader>();
		postProcessShader->Create(eShaderStage::VS, L"PostProcessVS.hlsl", "main");
		postProcessShader->Create(eShaderStage::PS, L"PostProcessPS.hlsl", "main");
		postProcessShader->SetDSState(eDSType::NoWrite);
		Resources::Insert<Shader>(L"PostProcessShader", postProcessShader);
#pragma endregion
#pragma region BASIC 3D
		std::shared_ptr<Shader> basicShader = std::make_shared<Shader>();
		basicShader->Create(eShaderStage::VS, L"BasicVS.hlsl", "main");
		basicShader->Create(eShaderStage::PS, L"BasicPS.hlsl", "main");
		Resources::Insert<Shader>(L"BasicShader", basicShader);
#pragma endregion
#pragma region DEFERRED
		std::shared_ptr<Shader> deferredShader = std::make_shared<Shader>();
		deferredShader->Create(eShaderStage::VS, L"DeferredVS.hlsl", "main");
		deferredShader->Create(eShaderStage::PS, L"DeferredPS.hlsl", "main");
		Resources::Insert<Shader>(L"DeferredShader", deferredShader);
#pragma endregion
#pragma region LIGHT
		std::shared_ptr<Shader> lightShader = std::make_shared<Shader>();
		lightShader->Create(eShaderStage::VS, L"LightDirVS.hlsl", "main");
		lightShader->Create(eShaderStage::PS, L"LightDirPS.hlsl", "main");

		lightShader->SetRSState(eRSType::SolidBack);
		lightShader->SetDSState(eDSType::None);
		lightShader->SetBSState(eBSType::OneOne);

		Resources::Insert<Shader>(L"LightDirShader", lightShader);

		lightShader = std::make_shared<Shader>();
		lightShader->Create(eShaderStage::VS, L"LightPointVS.hlsl", "main");
		lightShader->Create(eShaderStage::PS, L"LightPointPS.hlsl", "main");

		lightShader->SetRSState(eRSType::SolidFront);
		lightShader->SetDSState(eDSType::None);
		lightShader->SetBSState(eBSType::OneOne);

		Resources::Insert<Shader>(L"LightPointShader", lightShader);
#pragma endregion

#pragma region MERGE
		std::shared_ptr<Shader> MergeShader = std::make_shared<Shader>();
		MergeShader->Create(eShaderStage::VS, L"MergeVS.hlsl", "main");
		MergeShader->Create(eShaderStage::PS, L"MergePS.hlsl", "main");

		MergeShader->SetRSState(eRSType::SolidBack);
		MergeShader->SetDSState(eDSType::None);
		MergeShader->SetBSState(eBSType::Default);

		Resources::Insert<Shader>(L"MergeShader", MergeShader);
#pragma endregion

#pragma region Shadow
		std::shared_ptr<Shader> shadowShader = std::make_shared<Shader>();
		shadowShader->Create(eShaderStage::VS, L"DepthMapVS.hlsl", "main");
		shadowShader->Create(eShaderStage::PS, L"DepthMapPS.hlsl", "main");

		shadowShader->SetRSState(eRSType::SolidBack);
		shadowShader->SetDSState(eDSType::Less);
		shadowShader->SetBSState(eBSType::Default);

		Resources::Insert<Shader>(L"ShadowShader", shadowShader);
#pragma endregion



#pragma region SKYBOX SHADER
		std::shared_ptr<Shader> skyBoxShader = std::make_shared<Shader>();
		skyBoxShader->Create(eShaderStage::VS, L"SkyBoxVS.hlsl", "main");
		skyBoxShader->Create(eShaderStage::PS, L"SkyBoxPS.hlsl", "main");
		skyBoxShader->SetRSState(eRSType::SolidFront);
		skyBoxShader->SetDSState(eDSType::Less);
		skyBoxShader->SetBSState(eBSType::Default);
		Resources::Insert<Shader>(L"SkyBoxShader", skyBoxShader);
#pragma endregion

		//Compute
		std::shared_ptr<BoneShader> computeShader = std::make_shared<BoneShader>();
		computeShader->Create(L"BoneAnimationCS.hlsl", "CS_Animation3D");
		Resources::Insert<BoneShader>(L"BoneComputeShader", computeShader);
	}

	void SetUpState()
	{
		#pragma region Input layout
		D3D11_INPUT_ELEMENT_DESC arrLayoutDesc[8] = {};

		arrLayoutDesc[0].AlignedByteOffset = 0;
		arrLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayoutDesc[0].InputSlot = 0;
		arrLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[0].SemanticName = "POSITION";
		arrLayoutDesc[0].SemanticIndex = 0;

		arrLayoutDesc[1].AlignedByteOffset = 16;
		arrLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayoutDesc[1].InputSlot = 0;
		arrLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[1].SemanticName = "COLOR";
		arrLayoutDesc[1].SemanticIndex = 0;

		arrLayoutDesc[2].AlignedByteOffset = 32;
		arrLayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayoutDesc[2].InputSlot = 0;
		arrLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[2].SemanticName = "TEXCOORD";
		arrLayoutDesc[2].SemanticIndex = 0;

		arrLayoutDesc[3].AlignedByteOffset = 40;
		arrLayoutDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayoutDesc[3].InputSlot = 0;
		arrLayoutDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[3].SemanticName = "TANGENT";
		arrLayoutDesc[3].SemanticIndex = 0;

		arrLayoutDesc[4].AlignedByteOffset = 52;
		arrLayoutDesc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayoutDesc[4].InputSlot = 0;
		arrLayoutDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[4].SemanticName = "BINORMAL";
		arrLayoutDesc[4].SemanticIndex = 0;

		arrLayoutDesc[5].AlignedByteOffset = 64;
		arrLayoutDesc[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayoutDesc[5].InputSlot = 0;
		arrLayoutDesc[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[5].SemanticName = "NORMAL";
		arrLayoutDesc[5].SemanticIndex = 0;

		arrLayoutDesc[6].AlignedByteOffset = 76;
		arrLayoutDesc[6].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayoutDesc[6].InputSlot = 0;
		arrLayoutDesc[6].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[6].SemanticName = "BLENDWEIGHT";
		arrLayoutDesc[6].SemanticIndex = 0;

		arrLayoutDesc[7].AlignedByteOffset = 92;
		arrLayoutDesc[7].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayoutDesc[7].InputSlot = 0;
		arrLayoutDesc[7].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[7].SemanticName = "BLENDINDICES";
		arrLayoutDesc[7].SemanticIndex = 0;
		//Vector3 tangent;
		//Vector3 biNormal;
		//Vector3 normal;


		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"RectShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> postureShader = Resources::Find<Shader>(L"PostureShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, postureShader->GetVSBlobBufferPointer()
			, postureShader->GetVSBlobBufferSize()
			, postureShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> spriteShader = Resources::Find<Shader>(L"SpriteShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, spriteShader->GetVSBlobBufferPointer()
			, spriteShader->GetVSBlobBufferSize()
			, spriteShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> uiShader = Resources::Find<Shader>(L"UIShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, uiShader->GetVSBlobBufferPointer()
			, uiShader->GetVSBlobBufferSize()
			, uiShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> gridShader = Resources::Find<Shader>(L"GridShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, gridShader->GetVSBlobBufferPointer()
			, gridShader->GetVSBlobBufferSize()
			, gridShader->GetInputLayoutAddressOf());


		std::shared_ptr<Shader> debugShader = Resources::Find<Shader>(L"DebugShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, debugShader->GetVSBlobBufferPointer()
			, debugShader->GetVSBlobBufferSize()
			, debugShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> debugPaintedShader = Resources::Find<Shader>(L"DebugPaintedShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, debugPaintedShader->GetVSBlobBufferPointer()
			, debugPaintedShader->GetVSBlobBufferSize()
			, debugPaintedShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> particleShader = Resources::Find<Shader>(L"ParticleShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, particleShader->GetVSBlobBufferPointer()
			, particleShader->GetVSBlobBufferSize()
			, particleShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> postProcessShader = Resources::Find<Shader>(L"PostProcessShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, postProcessShader->GetVSBlobBufferPointer()
			, postProcessShader->GetVSBlobBufferSize()
			, postProcessShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> meterShader = Resources::Find<Shader>(L"MeterShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, meterShader->GetVSBlobBufferPointer()
			, meterShader->GetVSBlobBufferSize()
			, meterShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> dangerUIShader = Resources::Find<Shader>(L"DangerUIShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, dangerUIShader->GetVSBlobBufferPointer()
			, dangerUIShader->GetVSBlobBufferSize()
			, dangerUIShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> AlphaAnimationShader = Resources::Find<Shader>(L"AlphaAnimationShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, AlphaAnimationShader->GetVSBlobBufferPointer()
			, AlphaAnimationShader->GetVSBlobBufferSize()
			, AlphaAnimationShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> basicShader = Resources::Find<Shader>(L"BasicShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 8
			, basicShader->GetVSBlobBufferPointer()
			, basicShader->GetVSBlobBufferSize()
			, basicShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> deferredShader = Resources::Find<Shader>(L"DeferredShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 8
			, deferredShader->GetVSBlobBufferPointer()
			, deferredShader->GetVSBlobBufferSize()
			, deferredShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> lightShader = Resources::Find<Shader>(L"LightDirShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 6
			, lightShader->GetVSBlobBufferPointer()
			, lightShader->GetVSBlobBufferSize()
			, lightShader->GetInputLayoutAddressOf());

		lightShader = Resources::Find<Shader>(L"LightPointShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 6
			, lightShader->GetVSBlobBufferPointer()
			, lightShader->GetVSBlobBufferSize()
			, lightShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> mergeShader = Resources::Find<Shader>(L"MergeShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 6
			, mergeShader->GetVSBlobBufferPointer()
			, mergeShader->GetVSBlobBufferSize()
			, mergeShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> skyBoxShader = Resources::Find<Shader>(L"SkyBoxShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 8
			, skyBoxShader->GetVSBlobBufferPointer()
			, skyBoxShader->GetVSBlobBufferSize()
			, skyBoxShader->GetInputLayoutAddressOf());

		//std::shared_ptr<Shader> shadowShader = Resources::Find<Shader>(L"ShadowShader");
		//GetDevice()->CreateInputLayout(arrLayoutDesc, 8
		//	, shadowShader->GetVSBlobBufferPointer()
		//	, shadowShader->GetVSBlobBufferSize()
		//	, shadowShader->GetInputLayoutAddressOf());

#pragma endregion
		#pragma region sampler state
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, samplerStates[(UINT)eSamplerType::Point].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf()
		);

		GetDevice()->BindsSamplers((UINT)eSamplerType::Point
			, 1, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());

		GetDevice()->BindsSamplers((UINT)eSamplerType::Linear
			, 1, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf());

		GetDevice()->BindsSamplers((UINT)eSamplerType::Anisotropic
			, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
#pragma endregion
		#pragma region Rasterizer state
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

		GetDevice()->CreateRasterizerState(&rsDesc
			, rasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

		GetDevice()->CreateRasterizerState(&rsDesc
			, rasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc
			, rasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc
			, rasterizerStates[(UINT)eRSType::WireframeNone].GetAddressOf());
#pragma endregion
		#pragma region Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		
		GetDevice()->CreateDepthStencilState(&dsDesc
			, depthstencilStates[(UINT)eDSType::Less].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc
			, depthstencilStates[(UINT)eDSType::Greater].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc
			, depthstencilStates[(UINT)eDSType::NoWrite].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc
			, depthstencilStates[(UINT)eDSType::None].GetAddressOf());
#pragma endregion
		#pragma region Blend State
		//None
		//blendStates[(UINT)eBSType::Default] = nullptr;

		D3D11_BLEND_DESC bsDesc = {};
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;

		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&bsDesc, blendStates[(UINT)eBSType::Default].GetAddressOf());
		//blendStates[(UINT)eBSType::Default] = nullptr;

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&bsDesc, blendStates[(UINT)eBSType::AlphaBlend].GetAddressOf());

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&bsDesc, blendStates[(UINT)eBSType::OneOne].GetAddressOf());

#pragma endregion
	}

	void LoadBuffer()
	{
		#pragma region CONSTANT BUFFER
		constantBuffers[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		constantBuffers[(UINT)eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffers[(UINT)eCBType::Material]->Create(sizeof(MaterialCB));

		constantBuffers[(UINT)eCBType::Grid] = new ConstantBuffer(eCBType::Grid);
		constantBuffers[(UINT)eCBType::Grid]->Create(sizeof(GridCB));

		constantBuffers[(UINT)eCBType::Animation] = new ConstantBuffer(eCBType::Animation);
		constantBuffers[(UINT)eCBType::Animation]->Create(sizeof(AnimationCB));

		constantBuffers[(UINT)eCBType::Light] = new ConstantBuffer(eCBType::Light);
		constantBuffers[(UINT)eCBType::Light]->Create(sizeof(LightCB));

		constantBuffers[(UINT)eCBType::ParticleSystem] = new ConstantBuffer(eCBType::ParticleSystem);
		constantBuffers[(UINT)eCBType::ParticleSystem]->Create(sizeof(ParticleSystemCB));

		constantBuffers[(UINT)eCBType::Noise] = new ConstantBuffer(eCBType::Noise);
		constantBuffers[(UINT)eCBType::Noise]->Create(sizeof(NoiseCB));

		constantBuffers[(UINT)eCBType::Bone] = new ConstantBuffer(eCBType::Bone);
		constantBuffers[(UINT)eCBType::Bone]->Create(sizeof(BoneAnimationCB));

		constantBuffers[(UINT)eCBType::LightMatrix] = new ConstantBuffer(eCBType::LightMatrix);
		constantBuffers[(UINT)eCBType::LightMatrix]->Create(sizeof(LightMatrixCB));

		constantBuffers[(UINT)eCBType::UniformData] = new ConstantBuffer(eCBType::UniformData);
		constantBuffers[(UINT)eCBType::UniformData]->Create(sizeof(UniformDataCB));

		constantBuffers[(UINT)eCBType::HpMeter] = new ConstantBuffer(eCBType::HpMeter);
		constantBuffers[(UINT)eCBType::HpMeter]->Create(sizeof(MeterCB));

		constantBuffers[(UINT)eCBType::Time] = new ConstantBuffer(eCBType::Time);
		constantBuffers[(UINT)eCBType::Time]->Create(sizeof(TimeCB));
		

#pragma endregion
		#pragma region STRUCTED BUFFER
		lightsBuffer = new graphics::StructedBuffer();
		lightsBuffer->Create(sizeof(LightAttribute), 128, eSRVType::SRV, nullptr, true);
#pragma endregion
	}

	void LoadTexture()
	{
		#pragma region STATIC TEXTURE
		Resources::Load<Texture>(L"SmileTexture", L"Smile.png");
		Resources::Load<Texture>(L"DeathBlowTexture", L"UI\\Texture\\DeathBlow.png");
		Resources::Load<Texture>(L"LockOnTexture", L"UI\\Texture\\LockOn.png");

		Resources::Load<Texture>(L"DefaultSprite", L"Light.png");
		
		Resources::Load<Texture>(L"CartoonSmoke", L"particle\\CartoonSmoke.png");
		Resources::Load<Texture>(L"SparkParticle", L"Effect\\Texture\\Spark.png");
		Resources::Load<Texture>(L"noise_01", L"noise\\noise_01.png");
		Resources::Load<Texture>(L"noise_02", L"noise\\noise_02.png");
		Resources::Load<Texture>(L"noise_03", L"noise\\noise_03.jpg");

		Resources::Load<Texture>(L"BasicCube", L"Cube\\TILE_01.tga");
		Resources::Load<Texture>(L"BasicCubeNormal", L"Cube\\TILE_01_N.tga");

		Resources::Load<Texture>(L"Brick", L"Cube\\Brick.jpg");
		Resources::Load<Texture>(L"Brick_N", L"Cube\\Brick_N.jpg");

	#pragma endregion
		#pragma region DYNAMIC TEXTURE
		std::shared_ptr<Texture> uavTexture = std::make_shared<Texture>();
		uavTexture->Create(1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE 
			| D3D11_BIND_UNORDERED_ACCESS);
		Resources::Insert<Texture>(L"PaintTexture", uavTexture);
	#pragma endregion

		//noise
		postProcessTexture = std::make_shared<Texture>();
		postProcessTexture->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		postProcessTexture->BindShaderResource(eShaderStage::PS, 60);

	}

	void LoadMaterial()
	{
		#pragma region DEFAULT
		std::shared_ptr <Texture> texture = Resources::Find<Texture>(L"PaintTexture");
		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"RectShader");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::Albedo, texture);
		Resources::Insert<Material>(L"RectMaterial", material);
		#pragma endregion
		#pragma region SPRITE
		std::shared_ptr <Texture> spriteTexture= Resources::Find<Texture>(L"DefaultSprite");
		std::shared_ptr<Shader> spriteShader = Resources::Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();	
		spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
		spriteMaterial->SetShader(spriteShader);
		spriteMaterial->SetTexture(eTextureSlot::Albedo, spriteTexture);
		Resources::Insert<Material>(L"SpriteMaterial", spriteMaterial);

		#pragma endregion
#pragma region POSTURE
		std::shared_ptr<Shader> postureBarShader = Resources::Find<Shader>(L"PostureShader");
		std::shared_ptr<Material> postureBarMaterial = std::make_shared<Material>();
		postureBarMaterial->SetRenderingMode(eRenderingMode::Transparent);
		postureBarMaterial->SetShader(postureBarShader);
		Resources::Insert<Material>(L"PostureBarMaterial", postureBarMaterial);

#pragma endregion

#pragma region HpLayout
		
		std::shared_ptr<Shader> hpLayoutShader = Resources::Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> hpLayoutMaterial = std::make_shared<Material>();
		hpLayoutMaterial->SetRenderingMode(eRenderingMode::Transparent);
		hpLayoutMaterial->SetShader(hpLayoutShader);
		Resources::Insert<Material>(L"HpLayoutMaterial", hpLayoutMaterial);

#pragma endregion


		#pragma region UI
		std::shared_ptr<Shader> uiShader = Resources::Find<Shader>(L"UIShader");
		std::shared_ptr<Material> uiMaterial = std::make_shared<Material>();
		uiMaterial->SetRenderingMode(eRenderingMode::Transparent);
	
		uiMaterial->SetShader(uiShader);
		Resources::Insert<Material>(L"UIMaterial", uiMaterial);
		#pragma endregion

#pragma region METER
		std::shared_ptr<Shader> meterShader = Resources::Find<Shader>(L"MeterShader");
		std::shared_ptr<Material> meterMaterial = std::make_shared<Material>();
		meterMaterial->SetRenderingMode(eRenderingMode::Transparent);
		meterMaterial->SetShader(meterShader);
		Resources::Insert<Material>(L"MeterMaterial", meterMaterial);
#pragma endregion
#pragma region DangerUI
		std::shared_ptr<Shader> dangerUIShader = Resources::Find<Shader>(L"DangerUIShader");
		std::shared_ptr<Material> dangerUIMaterial = std::make_shared<Material>();
		dangerUIMaterial->SetRenderingMode(eRenderingMode::Transparent);
		dangerUIMaterial->SetShader(dangerUIShader);
		Resources::Insert<Material>(L"DangerUIMaterial", dangerUIMaterial);
#pragma endregion
		#pragma region GRID
		std::shared_ptr<Shader> gridShader = Resources::Find<Shader>(L"GridShader");
		std::shared_ptr<Material> gridMaterial = std::make_shared<Material>();
		gridMaterial->SetShader(gridShader);
		Resources::Insert<Material>(L"GridMaterial", gridMaterial);
#pragma endregion
#pragma region DEBUG
		std::shared_ptr<Shader> debugShader = Resources::Find<Shader>(L"DebugShader");
		std::shared_ptr<Material> debugMaterial = std::make_shared<Material>();
		debugMaterial->SetRenderingMode(eRenderingMode::Transparent);
		debugMaterial->SetShader(debugShader);
		Resources::Insert<Material>(L"DebugMaterial", debugMaterial);
#pragma endregion
#pragma region DEBUG PAINTED
		std::shared_ptr<Shader> debugPaintedShader = Resources::Find<Shader>(L"DebugPaintedShader");
		std::shared_ptr<Material> debugPaintedMaterial = std::make_shared<Material>();
		debugPaintedMaterial->SetRenderingMode(eRenderingMode::Transparent);
		debugPaintedMaterial->SetShader(debugPaintedShader);
		Resources::Insert<Material>(L"DebugPaintedMaterial", debugPaintedMaterial);
#pragma endregion
		#pragma region PARTICLE
		std::shared_ptr<Shader> particleShader = Resources::Find<Shader>(L"ParticleShader");
		std::shared_ptr<Material> particleMaterial = std::make_shared<Material>();
		particleMaterial->SetRenderingMode(eRenderingMode::Transparent);
		particleMaterial->SetShader(particleShader);
		Resources::Insert<Material>(L"ParticleMaterial", particleMaterial);
		#pragma endregion
		#pragma region POSTPROCESS
		std::shared_ptr<Shader> postProcessShader = Resources::Find<Shader>(L"PostProcessShader");
		std::shared_ptr<Material> postProcessMaterial = std::make_shared<Material>();
		postProcessMaterial->SetRenderingMode(eRenderingMode::PostProcess);
		postProcessMaterial->SetShader(postProcessShader);
		Resources::Insert<Material>(L"PostProcessMaterial", postProcessMaterial);
#pragma endregion

#pragma region BASIC
		std::shared_ptr<Shader> basicShader = Resources::Find<Shader>(L"BasicShader");
		std::shared_ptr<Material> basicMaterial = std::make_shared<Material>();
		basicMaterial->SetRenderingMode(eRenderingMode::Transparent);
		basicMaterial->SetShader(basicShader);

		std::shared_ptr <Texture> albedo = Resources::Find<Texture>(L"Brick");
		basicMaterial->SetTexture(eTextureSlot::Albedo, albedo);
		albedo = Resources::Find<Texture>(L"Brick_N");
		basicMaterial->SetTexture(eTextureSlot::Normal, albedo);
		Resources::Insert<Material>(L"BasicMaterial", basicMaterial);
#pragma endregion
#pragma region DEFFERD
		std::shared_ptr<Shader> deferredShader = Resources::Find<Shader>(L"DeferredShader");
		std::shared_ptr<Material> deferredMaterial = std::make_shared<Material>();
		deferredMaterial->SetRenderingMode(eRenderingMode::DeferredOpaque);
		deferredMaterial->SetShader(deferredShader);

		// specular map 추가 사용가능
		albedo = Resources::Find<Texture>(L"Brick");
		deferredMaterial->SetTexture(eTextureSlot::Albedo, albedo);
		albedo = Resources::Find<Texture>(L"Brick_N");
		deferredMaterial->SetTexture(eTextureSlot::Normal, albedo);
		Resources::Insert<Material>(L"DeferredMaterial", deferredMaterial);
#pragma endregion
#pragma region LIGHT
		std::shared_ptr<Shader> lightShader = Resources::Find<Shader>(L"LightDirShader");
		std::shared_ptr<Material> lightMaterial = std::make_shared<Material>();
		lightMaterial->SetRenderingMode(eRenderingMode::None);
		lightMaterial->SetShader(lightShader);

		albedo = Resources::Find<Texture>(L"PositionTarget");
		lightMaterial->SetTexture(eTextureSlot::PositionTarget, albedo);
		albedo = Resources::Find<Texture>(L"NormalTarget");
		lightMaterial->SetTexture(eTextureSlot::NormalTarget, albedo);
		albedo = Resources::Find<Texture>(L"SpecularTarget");
		lightMaterial->SetTexture(eTextureSlot::SpecularTarget, albedo);

		Resources::Insert<Material>(L"LightMaterial", lightMaterial);

		lightShader = Resources::Find<Shader>(L"LightPointShader");
		lightMaterial = std::make_shared<Material>();
		lightMaterial->SetRenderingMode(eRenderingMode::None);
		lightMaterial->SetShader(lightShader);

		albedo = Resources::Find<Texture>(L"PositionTarget");
		lightMaterial->SetTexture(eTextureSlot::PositionTarget, albedo);
		albedo = Resources::Find<Texture>(L"NormalTarget");
		lightMaterial->SetTexture(eTextureSlot::NormalTarget, albedo);
		albedo = Resources::Find<Texture>(L"SpecularTarget");
		lightMaterial->SetTexture(eTextureSlot::SpecularTarget, albedo);
		albedo = Resources::Find<Texture>(L"ShadowMapTarget");
		lightMaterial->SetTexture(eTextureSlot::ShadowMap, albedo);

		Resources::Insert<Material>(L"LightPointMaterial", lightMaterial);
#pragma endregion
#pragma region MERGE
		std::shared_ptr<Shader> mergeShader = Resources::Find<Shader>(L"MergeShader");
		std::shared_ptr<Material> mergeMaterial = std::make_shared<Material>();
		mergeMaterial->SetRenderingMode(eRenderingMode::None);
		mergeMaterial->SetShader(mergeShader);

		albedo = Resources::Find<Texture>(L"PositionTarget");
		mergeMaterial->SetTexture(eTextureSlot::PositionTarget, albedo);

		albedo = Resources::Find<Texture>(L"AlbedoTarget");
		mergeMaterial->SetTexture(eTextureSlot::AlbedoTarget, albedo);

		albedo = Resources::Find<Texture>(L"DiffuseLightTarget");
		mergeMaterial->SetTexture(eTextureSlot::DiffuseLightTarget, albedo);
		albedo = Resources::Find<Texture>(L"SpecualrLightTarget");
		mergeMaterial->SetTexture(eTextureSlot::SpecularLightTarget, albedo);


		Resources::Insert<Material>(L"MergeMaterial", mergeMaterial);
#pragma endregion

#pragma region SKYBOX
		std::shared_ptr<Shader> skyBoxShader = Resources::Find<Shader>(L"SkyBoxShader");
		std::shared_ptr<Material> skyBoxMaterial = std::make_shared<Material>();
		skyBoxMaterial->SetRenderingMode(eRenderingMode::Opaque);
		skyBoxMaterial->SetShader(skyBoxShader);
		Resources::Insert<Material>(L"SkyBoxMaterial", skyBoxMaterial);

		std::shared_ptr<Texture> skyBoxTexture = Resources::Load<Texture>(L"Sky01", L"SkyBox\\SkyWater.dds");
		skyBoxTexture->BindShaderResource(eShaderStage::PS, 11);

#pragma endregion
#pragma region SHADOW
		std::shared_ptr<Shader> shadowShader = Resources::Find<Shader>(L"ShadowShader");
		std::shared_ptr<Material> shadowMaterial = std::make_shared<Material>();
		shadowMaterial->SetRenderingMode(eRenderingMode::None);
		shadowMaterial->SetShader(shadowShader);

		albedo = Resources::Find<Texture>(L"ShadowMapTarget");
		shadowMaterial->SetTexture(eTextureSlot::ShadowMap, albedo);

		Resources::Insert<Material>(L"ShadowMapMaterial", shadowMaterial);
#pragma endregion

		
	}

	void Initialize()
	{
		CreateRenderTargets();
		LoadMesh();
		LoadShader();
		SetUpState();
		LoadBuffer();
		LoadTexture();
		LoadMaterial();
	}

	void Release()
	{
		for (size_t i = 0; i < (UINT)eCBType::End; i++)
		{
			delete constantBuffers[i]; 
			constantBuffers[i] = nullptr;
		}

		delete lightsBuffer;
		lightsBuffer = nullptr;

		for (size_t i = 0; i < (UINT)eRTType::End; i++)
		{
			if (renderTargets[i] == nullptr)
			{
				continue;
			}

			delete renderTargets[i];
			renderTargets[i] = nullptr;
		}
	}


	void Render()
	{
		//렌더타겟 설정 -> 이제 카메라에서 해줌
		//renderTargets[(UINT)eRTType::Swapchain]->OmSetRenderTarget();

		BindNoiseTexture();
		BindLights();
		
		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		for (Camera* cam : cameras[(UINT)type])
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		cameras[(UINT)type].clear();
		renderer::lightAttributes.clear();
	}

	void CreateRenderTargets()
	{
		UINT width = application.GetWidth();
		UINT height = application.GetHeight();
		//Swapchain MRT
		{
			
			std::shared_ptr<Texture> arrRTTex[8] = {};
			std::shared_ptr<Texture> dsTex = nullptr;

			arrRTTex[0] = Resources::Find<Texture>(L"RenderTargetTexture");
			dsTex = Resources::Find<Texture>(L"DepthStencilTexture");

			renderTargets[(UINT)eRTType::Swapchain] = new MultiRenderTarget();
			renderTargets[(UINT)eRTType::Swapchain]->Create(arrRTTex, dsTex);
		}
		// deferred MRT
		{
			std::shared_ptr<Texture> arrRTTex[8] = { };
			std::shared_ptr<Texture> pos = std::make_shared<Texture>();
			std::shared_ptr<Texture> normal = std::make_shared<Texture>();
			std::shared_ptr<Texture> albedo = std::make_shared<Texture>();
			std::shared_ptr<Texture> specular = std::make_shared<Texture>();

			Resources::Insert<Texture>(L"PositionTarget", pos);
			Resources::Insert<Texture>(L"NormalTarget", normal);
			Resources::Insert<Texture>(L"AlbedoTarget", albedo);
			Resources::Insert<Texture>(L"SpecularTarget", specular);

			arrRTTex[0] = pos;
			arrRTTex[1] = normal;
			arrRTTex[2] = albedo;
			arrRTTex[3] = specular;

			arrRTTex[0]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			arrRTTex[1]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			arrRTTex[2]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			arrRTTex[3]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

			std::shared_ptr<Texture> dsTex = nullptr;
			dsTex = Resources::Find<Texture>(L"DepthStencilTexture");

			renderTargets[(UINT)eRTType::Deferred] = new MultiRenderTarget();
			renderTargets[(UINT)eRTType::Deferred]->Create(arrRTTex, dsTex);
		}

		// Light MRT
		{
			std::shared_ptr<Texture> arrRTTex[8] = { };
			std::shared_ptr<Texture> diffuse = std::make_shared<Texture>();
			std::shared_ptr<Texture> specular = std::make_shared<Texture>();

			Resources::Insert<Texture>(L"DiffuseLightTarget", diffuse);
			Resources::Insert<Texture>(L"SpecualrLightTarget", specular);

			arrRTTex[0] = diffuse;
			arrRTTex[1] = specular;

			arrRTTex[0]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			arrRTTex[1]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

			renderTargets[(UINT)eRTType::Light] = new MultiRenderTarget();
			renderTargets[(UINT)eRTType::Light]->Create(arrRTTex, nullptr);
		}

		{
			std::shared_ptr<Texture> arrRTTex[8] = { };
			std::shared_ptr<Texture> depthMap = std::make_shared<Texture>();

			Resources::Insert<Texture>(L"ShadowMapTarget", depthMap);
			arrRTTex[0] = depthMap;
			arrRTTex[0]->Create(1600, 900, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);


			std::shared_ptr<Texture> depthStencilTex = std::make_shared<Texture>();
			depthStencilTex->Create(1600, 900, DXGI_FORMAT_D32_FLOAT
				, D3D11_BIND_DEPTH_STENCIL);

			renderTargets[(UINT)eRTType::Shadow] = new MultiRenderTarget();
			renderTargets[(UINT)eRTType::Shadow]->Create(arrRTTex, depthStencilTex);
		}
	}

	void ClearRenderTargets()
	{
		for (size_t i = 0; i < (UINT)eRTType::End; i++)
		{
			if (renderTargets[i] == nullptr)
				continue;

			if (i == 0)
			{
				FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
				renderTargets[i]->Clear(backgroundColor);
			}
			else
			{
				FLOAT backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				renderTargets[i]->Clear(backgroundColor);
			}
		}
	}

	void PushLightAttribute(LightAttribute lightAttribute)
	{
		lightAttributes.push_back(lightAttribute);
	}

	void BindLights()
	{
		lightsBuffer->SetData(lightAttributes.data(), (UINT)lightAttributes.size());
		lightsBuffer->BindSRV(eShaderStage::VS, 13);
		lightsBuffer->BindSRV(eShaderStage::PS, 13);

		renderer::LightCB trCb = {};
		trCb.numberOfLight = (UINT)lightAttributes.size();

		for (UINT i = 0; i < (UINT)lights.size(); i++)
			lights[i]->SetIndex(i);
		
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Light];
		cb->SetData(&trCb);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	float noiseTime = 10.0f;
	void BindNoiseTexture()
	{
		std::shared_ptr<Texture> noise = Resources::Find<Texture>(L"noise_03");
		noise->BindShaderResource(eShaderStage::VS, 16);
		noise->BindShaderResource(eShaderStage::HS, 16);
		noise->BindShaderResource(eShaderStage::DS, 16);
		noise->BindShaderResource(eShaderStage::GS, 16);
		noise->BindShaderResource(eShaderStage::PS, 16);
		noise->BindShaderResource(eShaderStage::CS, 16);

		NoiseCB info = {};
		info.noiseSize.x = (float)noise->GetWidth();
		info.noiseSize.y = (float)noise->GetHeight();
		noiseTime -= Time::DeltaTime();
		info.noiseTime = noiseTime;

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Noise];
		cb->SetData(&info);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::HS);
		cb->Bind(eShaderStage::DS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}

	void CopyRenderTarget()
	{
		std::shared_ptr<Texture> renderTarget = Resources::Find<Texture>(L"RenderTargetTexture");

		ID3D11ShaderResourceView* srv = nullptr;
		GetDevice()->BindShaderResource(eShaderStage::PS, 60, &srv);

		ID3D11Texture2D* dest = postProcessTexture->GetTexture().Get();
		ID3D11Texture2D* source = renderTarget->GetTexture().Get();

		GetDevice()->CopyResource(dest, source);

		postProcessTexture->BindShaderResource(eShaderStage::PS, 60);
	}
}

