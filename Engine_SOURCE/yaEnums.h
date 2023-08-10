#pragma once

namespace ya::enums
{
	enum class eSceneType
	{
		Tilte,
		Play,
		End,
	};

	enum class eLayerType
	{
		None = 0,
		Camera,
		Grid,
		Ground,
		Logbridge,
		Monster,
		Player,

		Particle,
		PostProcess,
		UI,
		End = 16,
	};

	enum class eComponentType
	{
		None,
		Transform, // 위치 데이터 수정하는 컴포넌트
		Camera,
		Mesh,
		Rigidbody,
		Collider,
		//Collider2,
		MeshRenderer,
		SpriteRenderer,
		Animator,
		ParticleSystem,
		AudioListener,
		AudioSource,
		Light,
		UI,
		//FadeOut,FadeIn
		Script,
		End,
	};

	enum class eResourceType
	{
		Mesh,
		Texture,
		Material,
		Sound,
		/*Font,*/
		Prefab,
		MeshData,
		GraphicShader,
		ComputeShader,
		AudioClip,
		Script,
		End,
	};

	enum class eColliderType
	{
		None,
		Rect,
		Circle,
		Box,
		Sphere,
		End,
	};

	enum class eAnimationType
	{
		None,
		SecondDimension,
		ThirdDimension,
		End,
	};

	enum class eLightType
	{
		Directional,
		Point,
		Spot,
		End,
	};

	enum class eSituation
	{
		None,
		Idle,			//기본 상태
		Boundary,		//경계
		Chase,			//		
		Battle,			//여기에서 분기 공격 , 후퇴, 막기 나눠서갈것
		Run,			//도망
		Defense,		//막기
		Attack,			//공격
		Sit,			//휴식
		Groggy,			//스턴 상태
		Death,			
		End,
	};





	//struct Ray
	//{
	//	Vector3 origin;
	//	Vector3 dest;
	//};
	extern const char* charComponentType[(int)eComponentType::End];
	extern const wchar_t* wcharComponentType[(int)eComponentType::End];
	extern const char* charResourceType[(int)eResourceType::End];
	extern const wchar_t* wcharResourceType[(int)eResourceType::End];
}