#include "yaTenzen.h"
#include "yaMeshData.h"

//test
#include "yaInput.h"
#include "yaCollider2D.h"
#include "yaBoneAnimator.h"
#include "yaBoneCollider.h"

#define STATE_HAVE(STATE) (mState & STATE) == STATE
#define ADD_STATE(STATE) mState |= STATE
#define RM_STATE(STATE) mState &= ~STATE

namespace ya
{
	void Tenzen::Initialize()
	{
		SetName(L"TenzenObject");
		mMeshData = MeshData::LoadFromFbx(L"Monster\\Boss_tenzen\\Mesh\\c1020.fbx");

		mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000000.fbx", L"Idle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000401.fbx", L"SitIdle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000402.fbx", L"SitToGetUp");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000410.fbx", L"ArmCrossStart");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000411.fbx", L"ArmCrossIdle");
		mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000412.fbx", L"ArmCrossEnd");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000600.fbx", L"LookAround");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000700.fbx", L"Surprised");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_001040.fbx", L"DrawSword");
		mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003000.fbx", L"SwingSword1"); // 칼을 우상단에서 우하단으로 크게 휘두르고 제자리로.

		


		MeshObject* object = mMeshData->Instantiate(eLayerType::Monster);
		Transform* meshTr = object->GetComponent<Transform>();
		meshTr->SetRotation(Vector3(180, 180, 0));

		BoneCollider* katana =  object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile);
		katana->SetMeshAndBone(mMeshData, L"R_Katana_long");

		katana->SetAnimOffSet(1, Vector3(-1.0, -0.8, 0));
		katana->SetAnimOffSet(L"Idle", Vector3(-1.0, -0.8, 0));
		
		katana->SetAnimOffSet(2,Vector3(-5, -0.5, 0));
		
		katana->SetColliderActiveFrame(2, 10, 100);
		katana->SetColliderActiveFrame(L"SwingSword1", 10, 100);

		katana->SetScale(Vector3(2, 0.3, 0.3));
		mKatana = katana;

		MonsterBase::Initialize();


	}
	void Tenzen::Update()
	{


		if (Input::GetKeyDown(eKeyCode::NUM_1))
			mMeshData->Play(L"ArmCrossEnd");
		if (Input::GetKeyDown(eKeyCode::NUM_2))
			mMeshData->Play(L"Idle");
		if (Input::GetKeyDown(eKeyCode::NUM_3))
			mMeshData->Play(L"SwingSword1");

		MonsterBase::Update();

	}
	void Tenzen::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void Tenzen::Render()
	{
		MonsterBase::Render();
	}
	void Tenzen::SetAnimation()
	{/*
		if (STATE_HAVE(TenzenState_Idle))
			mMeshData->Play(L"Idle");

		else if (STATE_HAVE(tenzenstate))
			mMeshData->Play(L"Idle");

		else if (STATE_HAVE(TenzenState_Idle))
			mMeshData->Play(L"Idle");

		else if (STATE_HAVE(TenzenState_Idle))
			mMeshData->Play(L"Idle");*/

	}
}