#pragma once
#include "CommonInclude.h"
#include "yaMath.h"
#include "yaGraphicDevice_DX11.h"

#include "yaMesh.h"
#include "yaShader.h"
#include "yaConstantBuffer.h"
#include "yaCamera.h"
#include "yaLight.h"
#include "yaStructedBuffer.h"

#include "yaMultiRenderTarget.h"

using namespace ya::math;
using namespace ya::graphics;

namespace ya::renderer
{
	struct Vertex
	{
		Vector4 pos;
		Vector4 color;
		Vector2 uv;
		Vector3 tangent;
		Vector3 biNormal;
		Vector3 normal;
	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Matrix world;
		Matrix inverseWorld;
		Matrix view;
		Matrix inverseView;
		Matrix projection;
	};

	CBUFFER(MaterialCB, CBSLOT_MATERIAL)
	{
		UINT usedAlbedo;
		UINT usedNormal;
		UINT usedSpecular;

		UINT padd1;
	};

	CBUFFER(GridCB, CBSLOT_GRID)
	{
		Vector4 cameraPosition;
		Vector2 cameraScale;
		Vector2 resolution;
	};

	CBUFFER(AnimationCB, CBSLOT_ANIMATION)
	{
		Vector2 leftTop;
		Vector2 size;
		Vector2 offset;
		Vector2 atlasSize;

		UINT type;
	};

	//CBSLOT_NUMBEROFLIGHT
	CBUFFER(LightCB, CBSLOT_NUMBEROFLIGHT)
	{
		UINT numberOfLight;
		UINT indexOfLight;
	};

	CBUFFER(ParticleSystemCB, CBSLOT_PARTICLESYSTEM)
	{
		Vector4 worldPosition;
		Vector4 startColor;
		Vector4 startSize;

		UINT maxParticles;
		UINT simulationSpace;
		float radius;
		float startSpeed;

		float startLifeTime;
		float deltaTime;
		float elapsedTime; //누적시간
		int padding;
	};

	CBUFFER(NoiseCB, CBSLOT_NOISE)
	{
		Vector4 noiseSize;
		float noiseTime;
	};


	extern Vertex vertexes[4];
	extern Camera* mainCamera;
	extern ConstantBuffer* constantBuffers[];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthstencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[];
	
	extern graphics::MultiRenderTarget* renderTargets[];

	extern std::vector<Camera*> cameras[];
	extern std::vector<DebugMesh> debugMeshes;
	extern std::vector<Light*> lights;
	extern std::vector<LightAttribute> lightAttributes;
	extern StructedBuffer* lightsBuffer;

	extern ya::GameObject* inspectorGameObject;

	void Initialize();
	void Render();
	void Release();

	//mrt
	void CreateRenderTargets();
	void ClearRenderTargets();

	//Renderer
	void PushLightAttribute(LightAttribute lightAttribute);
	void BindLights();
	void BindNoiseTexture();
	void CopyRenderTarget();
	
}

