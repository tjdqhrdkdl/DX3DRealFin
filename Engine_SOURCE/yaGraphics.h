#pragma once
#include <wrl.h>
#include "yaMath.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")


#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name

#define CBSLOT_TRANSFORM		0
#define CBSLOT_MATERIAL			1
#define CBSLOT_GRID				2
#define CBSLOT_ANIMATION		3
#define CBSLOT_NUMBEROFLIGHT	4
#define CBSLOT_PARTICLESYSTEM	5
#define CBSLOT_NOISE			6
#define CBSLOT_BONEANIMATION	7

using namespace ya::math;
namespace ya::graphics
{
	enum class ValidationMode
	{
		Disabled,
		Enabled,
		GPU,
	};
	struct Vertex
	{
		Vector4 pos;
		Vector4 color;
		Vector2 uv;
		Vector3 tangent;
		Vector3 biNormal;
		Vector3 normal;

		Vector4 weight;
		Vector4 Indices;
	};
	enum class eShaderStage
	{
		VS,
		HS,
		DS,
		GS,
		PS,
		CS,
		ALL,
		Count,
	};

	enum class eSamplerType
	{
		Point,
		Linear,
		Anisotropic,
		End,
	};

	enum class eRSType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireframeNone,
		End,
	};

	enum class eDSType
	{
		Less,
		Greater,
		NoWrite,
		None,
		End,
	};

	enum class eBSType
	{
		Default,
		AlphaBlend,
		OneOne,
		End,
	};

	enum class eRTType
	{
		Swapchain,
		Deferred,
		Light,
		Shadow,
		End,
	};

	enum class eRenderingMode
	{
		DeferredOpaque,
		DeferredMask,

		Light,

		Opaque,
		CutOut,
		Transparent,
		PostProcess,

		None,
		End,
	};


	struct GpuBuffer
	{
		enum class eType
		{
			Buffer,
			Texture,
			UnknownType,
		} type = eType::UnknownType;

		D3D11_BUFFER_DESC desc;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

		GpuBuffer() = default;
		virtual ~GpuBuffer() = default;
	};

	enum class eCBType
	{
		Transform,
		Material,
		Grid,
		Animation,
		Light,
		ParticleSystem,
		Noise,
		Bone,
		End,
	};

	enum class eGPUParam
	{
		Int,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Matrix,
	};

	enum class eSRVType
	{
		SRV,
		UAV,
		End,
	};

	enum class eTextureSlot
	{
		Albedo,
		Normal,
		PositionTarget,
		NormalTarget,
		AlbedoTarget,
		SpecularTarget,
		DiffuseLightTarget,
		SpecularLightTarget,

		Specular,

		//CubeT8,
		//CubeT9,

		//Array2DT10,
		//Array2DT11,

		End,
	};
	struct MaterialColr
	{
		Vector4 diffuseColor;
		Vector4 specularColor;
		Vector4 AmbientColor;
		Vector4 EmessiveColor;
	};
	struct DebugMesh
	{
		enums::eColliderType type;
		math::Vector3 position;
		math::Vector3 rotatation;
		math::Vector3 scale;
		
		float radius;
		float duration;
		float time;
	};

	struct LightAttribute
	{
		Vector4 diffuse;
		Vector4 specular;
		Vector4 ambient;

		Vector4 position;
		Vector4 direction;

		enums::eLightType type;
		float radius;
		float angle;
		int padding;
	};

	struct Particle
	{
		Vector4 position;
		Vector4 direction;

		float lifeTime;
		float time;
		float speed;
		UINT active;
	};

	struct ParticleShared
	{
		UINT activeCount;
	};

	///Animation 3D
	struct BoneFrameTransform
	{
		Vector4 translate;
		Vector4 scale;
		Vector4 rotation;
	};

	struct BoneKeyFrame
	{
		double time;
		int frame;
		Vector3 translate;
		Vector3 scale;
		Vector4 rotation;
	};

	struct BoneMatrix
	{
		std::wstring name;
		int depth;
		int parentIdx;
		Matrix offset;
		Matrix bone;
		std::vector<std::vector< BoneKeyFrame>> keyFrames;
	};

	struct BoneAnimationClip
	{
		std::wstring name;
		int startFrame;
		int endFrame;
		int frameLength;

		double startTime;
		double endTime;
		double timeLength;
		float updateTime;
		//fbxsdk::FbxTime::EMode mode;
		UINT mode;
	};
}