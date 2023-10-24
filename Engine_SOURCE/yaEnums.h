#pragma once

#include "CommonInclude.h"

namespace ya::enums
{
	enum class eSceneType
	{
		Title,
		Play,
		Loading,
		End,
	};

	enum class eLayerType
	{
		None = 0,
		Camera,
		Grid,
		Ground,
		Wall,
		Logbridge,
		HangingObject,
		Hook,
		Monster,
		MonsterProjectile,
		Player,
		PlayerProjectile,
		WallCheckCollision,
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
		BoneAnimator,
		ParticleSystem,
		AudioListener,
		AudioSource,
		Light,
		BoundarySphere,
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
		Animation,
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
		Cylinder,
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
		None,			//처음 몬스터 생겨났을때의 상태로
						//여기에서 기본 행동을 합니다 EX) 경계 서는 몹이면 경계를
						//순찰 도는 몹이면 이동 로직을 짜주면 되고 
						//None 상태에서만 처음 은신이 가능합니다.
		Idle,			//	
		Boundary,		//경계
		Chase,			//		
		Battle,			//여기에서 분기 공격 , 후퇴, 막기 나눠서갈것
		Run,			//도망
		Defense,		//막기
		Attack,			//공격
		DashAttack,
		Sit,			//휴식
		Groggy,			//스턴 상태
		Hit,			//
		Death,			
		End,
	};


	enum class RotationOrder : UINT8
	{
		XYZ,
		XZY,
		YXZ,
		YZX,
		ZXY,
		ZYX,

		END
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