#include "yaAshinaSoldier.h"

namespace ya
{
	AshinaSoldier::AshinaSoldier()
	{
	}
	AshinaSoldier::~AshinaSoldier()
	{
	}
	void AshinaSoldier::Initialize()
	{
		SetName(L"SwordManObject");

		//mMeshData = MeshData::LoadFromFbx(L"Monster\\Boss_tenzen\\Mesh\\c1020.fbx");
		//
		//
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000600.fbx", L"LookAround");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_001040.fbx", L"DrawSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003000.fbx", L"SwordAttack_1"); // 칼을 우상단에서 우하단으로 크게 휘두르고 제자리로.
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003001.fbx", L"SwordAttack_2"); // 칼을 좌하단에서 우상단으로. 한걸음 내딛으며.
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003003.fbx", L"SwordAttack_3"); // 전진 점프 하며, 칼을 우상단에서 좌하단으로. 한걸음 내딛으며.
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003004.fbx", L"SwordAttack_4"); // 못막는 공격, 하단 베기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003005.fbx", L"SwordAttack_5"); // 못막는 공격, 찌르기. 전진
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003006.fbx", L"SwordAttack_6"); // 양옆으로 휘두르기, 2회 연속공격, 2회전진
		// 
		//
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003051.fbx", L"SwordAttack_7"); // 좌상 - 우하 빠른 공격
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003054.fbx", L"SwordAttack_8"); // 트리플공격
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003056.fbx", L"SwordAttack_9"); // 우상 - 좌하 빠른 공격
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003020.fbx", L"EnergyRestore"); // 에너지 채우기 ?
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003067.fbx", L"Kick"); // 발로 차기
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003008.fbx", L"HandleAttack"); // 칼 손잡이로 치기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003009.fbx", L"SwordAttack_10"); // 손잡이 연계기
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003012.fbx", L"Catch"); // 잡기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_004100.fbx", L"PoundIntoGround"); // 잡기 연계기 바닥에 꽂기
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003014.fbx", L"ChageForm"); // 자세 바꾸기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003015.fbx", L"ChargeAttack"); // 자세 바꾼상태에서 거리주면 연계기
		// 
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_005000.fbx", L"WalkNoSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_008010.fbx", L"Hit1"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_008011.fbx", L"Hit2"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_008300.fbx", L"GrogyDownFront"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_008301.fbx", L"GrogyDownBack"); 
		//  
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_008400.fbx", L"GuardLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_008401.fbx", L"GuardRight"); 
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_008600.fbx", L"ParriedLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_008601.fbx", L"ParriedRight"); 
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_008700.fbx", L"BlockedLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_008701.fbx", L"BlockedRight"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_012000.fbx", L"DeathBlow1"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_012001.fbx", L"DeathBlow1_Death"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_012200.fbx", L"DeathBlowBackside"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_405000.fbx", L"WalkWithSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_405010.fbx", L"RunWithSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_500000.fbx", L"Defense");
		//mMeshData->AnimationSave(L"Monster\\Boss_tenzen\\AnimationData\\tenzen.animationdata");



		////fbx 로드
		////mMeshData = MeshData::LoadFromFbx(L"Monster\\AshinaSoldier\\Mesh\\c1010.fbx");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_000000.fbx", L"Idle");
		//mMeshData->AnimationSave(L"Monster\\AshinaSoldier\\AnimationData\\tenzen.animationdata");
		

		mMeshData = std::make_shared<MeshData>();
		mMeshData->Load(L"Monster\\AshinaSoldier\\Mesh\\c1010.fbx");
		mMeshData->AnimationLoad(L"Monster\\AshinaSoldier\\AnimationData\\tenzen.animationdata");
		MeshObject* object = mMeshData->Instantiate(eLayerType::Monster, GetScene());

		//동일 애니메이션 반복시 보간을 하지 않고 싶은 경우. (기본은 보간을 하도록 되어있음.)
		BoneAnimator* animator = mMeshData->GetAnimator();
		animator->Play(L"Idle");

	}
	void AshinaSoldier::Update()
	{
	}
	void AshinaSoldier::FixedUpdate()
	{
	}
	void AshinaSoldier::Render()
	{
	}
	void AshinaSoldier::DeathBlow()
	{
	}
	void AshinaSoldier::OnCollisionEnter(Collider2D* collider)
	{
	}
	void AshinaSoldier::OnCollisionStay(Collider2D* collider)
	{
	}
	void AshinaSoldier::OnCollisionExit(Collider2D* collider)
	{
	}
}