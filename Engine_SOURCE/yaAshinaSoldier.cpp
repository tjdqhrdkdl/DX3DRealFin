#include "yaAshinaSoldier.h"
#include "yaMeshData.h"

//test
#include "yaInput.h"
#include "yaCollider2D.h"
#include "yaBoneAnimator.h"
#include "yaBoneCollider.h"
#include "yaActionScript.h"
#include "yaPlayer.h"
#include "yaMonsterCollisionScript.h"
#include "yaSoldierSwordScript.h"

#include "yaNavMesh.h"
#include "yaAudioClip.h"

#define STATE_HAVE(STATE) (mState & STATE) == STATE
#define ADD_STATE(STATE) mState |= STATE
#define RM_STATE(STATE) mState &= ~STATE

#define BEFORE_STATE_HAVE(STATE) (mBeforeState & STATE) == STATE

namespace ya
{
	float AshinaSoldierEyeSightAngleCos = 0.1f;
	float AshinaSoldierWalkSpeed = 59;
	float AshinaSoldierBaseSpeed = 70;
	AshinaSoldier::AshinaSoldier()
		: MonsterBase()
		, mAlertTime(10)
		, mbNavOn(false)
	{
	}
	AshinaSoldier::~AshinaSoldier()
	{
	}
	void AshinaSoldier::Initialize()
	{
		MonsterBase::Initialize();
		SetName(L"AshinaSoldierObject");

		////fbx 로드
		//mMeshData = MeshData::LoadFromFbx(L"Monster\\AshinaSoldier\\Mesh\\c1010.fbx");
		//
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_000000.fbx", L"Idle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_000600.fbx", L"LookAround");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_001040.fbx", L"DrawSword");
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_003000.fbx", L"SwordAttack_1"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_003001.fbx", L"SwordAttack_2"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_003003.fbx", L"SwordAttack_3"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_003007.fbx", L"SwordAttack_4"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_003008.fbx", L"SwordAttack_5"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_003012.fbx", L"SwordAttack_6"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_003013.fbx", L"SwordAttack_7"); // 
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_005000.fbx", L"WalkWithNoSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_405000.fbx", L"WalkFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_405001.fbx", L"WalkBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_405002.fbx", L"WalkLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_405003.fbx", L"WalkRight");
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_405010.fbx", L"Run");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_005211.fbx", L"RunBack");
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008010.fbx", L"HitFront"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008011.fbx", L"HitBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008012.fbx", L"HitLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008013.fbx", L"HitRight"); 
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008300.fbx", L"GrogyDownFront"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008301.fbx", L"GrogyDownBack"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008550.fbx", L"GrogyDownParried");
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008500.fbx", L"GuardLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008501.fbx", L"GuardRight"); 
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008600.fbx", L"ParriedLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_008601.fbx", L"ParriedRight"); 
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_010000.fbx", L"Death"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_012000.fbx", L"DeathBlow2"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_012001.fbx", L"DeathBlow2_Death");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_012100.fbx", L"GroggyParried"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_012110.fbx", L"DeathBlow1"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_012111.fbx", L"DeathBlow1_Death"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_012200.fbx", L"DeathBlowAssasinated"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_012201.fbx", L"DeathBlowAssasinated_Death"); 
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_500000.fbx", L"Defense");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_501040.fbx", L"UnDefense");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_505000.fbx", L"DefenseMoveFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_505001.fbx", L"DefenseMoveBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_505002.fbx", L"DefenseMoveLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a000_505003.fbx", L"DefenseMoveRight");
		// 
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_000000.fbx", L"Spear_Idle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_400600.fbx", L"Spear_LookAround");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003000.fbx", L"Spear_SwordAttack_1"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003001.fbx", L"Spear_SwordAttack_2"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003002.fbx", L"Spear_SwordAttack_3"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003003.fbx", L"Spear_SwordAttack_4"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003004.fbx", L"Spear_SwordAttack_5"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003006.fbx", L"Spear_SwordAttack_6"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003008.fbx", L"Spear_SwordAttack_7"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003009.fbx", L"Spear_SwordAttack_8"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003010.fbx", L"Spear_SwordAttack_9"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003050.fbx", L"Spear_SwordAttack_10"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003051.fbx", L"Spear_SwordAttack_11"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003055.fbx", L"Spear_SwordAttack_12"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003056.fbx", L"Spear_SwordAttack_13"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_003100.fbx", L"Spear_SwordAttack_14"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_005000.fbx", L"Spear_WalkWithNoSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_405000.fbx", L"Spear_WalkFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_405001.fbx", L"Spear_WalkBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_405002.fbx", L"Spear_WalkLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_405003.fbx", L"Spear_WalkRight");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_405010.fbx", L"Spear_Run");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_008500.fbx", L"Spear_GuardLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_008501.fbx", L"Spear_GuardRight"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_008550.fbx", L"Spear_GrogyDownParried"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_008600.fbx", L"Spear_ParriedLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_008601.fbx", L"Spear_ParriedRight"); 																					   
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_500000.fbx", L"Spear_Defense");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_501040.fbx", L"Spear_UnDefense");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_505000.fbx", L"Spear_DefenseMoveFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_505001.fbx", L"Spear_DefenseMoveBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_505002.fbx", L"Spear_DefenseMoveLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSoldier\\Animation\\a200_505003.fbx", L"Spear_DefenseMoveRight");
		////
		//mMeshData->AnimationSave(L"Monster\\AshinaSoldier\\AnimationData\\AshinaSoldier.animationdata");
		// 
		 
		mMeshData = std::make_shared<MeshData>();
		mMeshData->Load(L"Monster\\AshinaSoldier\\MeshData\\c1010.meshdata");
		mMeshData->AnimationLoad(L"Monster\\AshinaSoldier\\AnimationData\\AshinaSoldier.animationdata");
		MeshObject* object = mMeshData->Instantiate(eLayerType::Monster, GetScene());
		//오브젝트 트랜스폼
		Transform* tr = GetComponent<Transform>();
		//tr->SetPosition(Vector3(0, 0, 0));
		tr->SetScale(Vector3(1, 1, 1));
		mTransform = tr;

		//메시 데이터 트랜스폼
		Transform* meshTr = object->GetComponent<Transform>();
		meshTr->SetRotation(Vector3(180, 0, 0));
		meshTr->SetPosition(Vector3(0, -1, 0));
		meshTr->SetScale(Vector3(1, 1, 1));
		meshTr->SetParent(tr);


		//무기 콜라이더 추가
		//Initialize

		//애니메이션 별로 오프셋과 발동 프레임 정해주기. 
		//발동 프레임은 기본이 false로 발동하고싶으면 조정해주면 된다.
		KatanaColliderInit();

		//피격 콜라이더
		mCollider = AddComponent<Collider2D>();
		mCollider->SetType(eColliderType::Box);
		mCollider->SetSize(Vector3(1.f, 2.f, 0.5f));
		mCollider->SetCenter(Vector3(0.f, 0.f, 0.f));

		//리지드 바디 , 액션 스크립트
		AddComponent<Rigidbody>()->SetFriction(180);
		AddComponent<MonsterCollisionScript>();
		mActionScript = AddComponent<ActionScript>();

		//애니메이션 이벤트
		SetAnimationEvent();


		//동일 애니메이션 반복시 보간을 하지 않고 싶은 경우. (기본은 보간을 하도록 되어있음.)
		BoneAnimator* animator = mMeshData->GetAnimator();
		animator->SetAnimationSelfChange(L"Run", false);
		animator->SetAnimationSelfChange(L"WalkFront", false);
		//애니메이션 끝부분 이상한 것 잘라내는 시간
		animator->SetAnimationTailTime(0.2f);


		std::vector<GameObject*> childObjects = mMeshData->GetChildObjects();
		std::vector<std::vector<std::shared_ptr<Material>>> materials = mMeshData->GetMaterialsVec();
		for (size_t i = 0; i < materials.size(); i++)
		{
			if (materials[i][0]->GetName() == L"#22#.001")
				mHat .push_back( childObjects[i]->GetComponent<Transform>());
			if (materials[i][0]->GetName() == L"#23#")
				mStraw.push_back(childObjects[i]->GetComponent<Transform>());
			if (materials[i][0]->GetName() == L"#18#")
				mAccessories.push_back(childObjects[i]->GetComponent<Transform>());
			if (materials[i][0]->GetName() == L"#11#")
				mArmor.push_back(childObjects[i]->GetComponent<Transform>());
			if (materials[i][0]->GetName() == L"#00#")
				mBaseSword.push_back(childObjects[i]->GetComponent<Transform>());
			if (materials[i][0]->GetName() == L"#01#")
				mBaseSwordInScabbard.push_back(childObjects[i]->GetComponent<Transform>());
			if (materials[i][0]->GetName() == L"#04#")
				mSpear.push_back(childObjects[i]->GetComponent<Transform>());


			if (materials[i][0]->GetName() == L"#02#")//ShortSword
				childObjects[i]->GetComponent<Transform>()->SetScale(Vector3::Zero);
			if (materials[i][0]->GetName() == L"#05#")//gun
				childObjects[i]->GetComponent<Transform>()->SetScale(Vector3::Zero);
			if (materials[i][0]->GetName() == L"#06#")//stick
				childObjects[i]->GetComponent<Transform>()->SetScale(Vector3::Zero);
			if (materials[i][0]->GetName() == L"#07#")//stone
				childObjects[i]->GetComponent<Transform>()->SetScale(Vector3::Zero);
		}
		for (size_t i = 0; i < mSpear.size(); i++)
		{
			mSpear[i]->SetScale(Vector3::Zero);
		}
		for (size_t i = 0; i < mBaseSwordInScabbard.size(); i++)
		{
			mBaseSwordInScabbard[i]->SetScale(Vector3::One);
		}
		for (size_t i = 0; i < mBaseSword.size(); i++)
		{
			mBaseSword[i]->SetScale(Vector3::Zero);
		}

		//SetPlayerObject(dynamic_cast<Player*>(SceneManager::GetActiveScene()->GetPlayer()));
		SetPlayerObject(dynamic_cast<Player*>(GetScene()->GetPlayer()));

		//몬스터 스테이트
		SetSpeed(AshinaSoldierBaseSpeed);


		//네비 매쉬
		AddComponent<NavMesh>();



		ADD_STATE(MonsterState_Guard);
		ADD_STATE(MonsterState_Idle);



		mAttackParams.damage = 10;
		mAttackParams.unGuardable = false;
		mAttackParams.special = SpecialAttack::None;

		SetMaxHP(100.0f);
		SetHp(GetMaxHP());
		SetPostureMax(100);
		SetPosture(0);
		SetAlertnessCount(0.f);
		SetDeathBlow(false);
		SetResurrectionCount(1);
		SetResurrectionCountMax(1);

		SetOriginState(GetState());
		SetOriginPosition(mTransform->GetPosition());

		//오디오 클립 로드
		Resources::Load<AudioClip>(L"ashinasoldier_v_recognize", L"..\\Resources\\Sound\\ashinasoldier\\Recognize\\c101008400i.wav");
		Resources::Load<AudioClip>(L"ashinasoldier_v_hit", L"..\\Resources\\Sound\\ashinasoldier\\Hit\\c101008100.wav");
		Resources::Load<AudioClip>(L"ashinasoldier_v_attack_1", L"..\\Resources\\Sound\\ashinasoldier\\Attack\\c101008302.wav");
		Resources::Load<AudioClip>(L"ashinasoldier_v_attack_2", L"..\\Resources\\Sound\\ashinasoldier\\Attack\\c101008302b.wav");
		Resources::Load<AudioClip>(L"ashinasoldier_v_attack_3", L"..\\Resources\\Sound\\ashinasoldier\\Attack\\c101008302c.wav");
		Resources::Load<AudioClip>(L"ashinasoldier_v_attack_4", L"..\\Resources\\Sound\\ashinasoldier\\Attack\\c101008302d.wav");
		Resources::Load<AudioClip>(L"ashinasoldier_v_attack_swing_1", L"..\\Resources\\Sound\\ashinasoldier\\Attack\\c101006004.wav");
		Resources::Load<AudioClip>(L"ashinasoldier_v_attack_swing_2", L"..\\Resources\\Sound\\ashinasoldier\\Attack\\c101006004c.wav");
		Resources::Load<AudioClip>(L"ashinasoldier_v_attack_swing_3", L"..\\Resources\\Sound\\ashinasoldier\\Attack\\c101006004d.wav");
		Resources::Load<AudioClip>(L"ashinasoldier_v_attack_swing_4", L"..\\Resources\\Sound\\ashinasoldier\\Attack\\c101006007b.wav");
	}
	void AshinaSoldier::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N_1))
			ADD_STATE(MonsterState_Alert);
		if (Input::GetKeyDown(eKeyCode::N_2))
			ADD_STATE(MonsterState_Recognize);
		if (Input::GetKeyDown(eKeyCode::N_3))
			ADD_STATE(MonsterState_Idle);
		if (Input::GetKeyDown(eKeyCode::N_4))
			mState = 0;
		if (Input::GetKeyDown(eKeyCode::NUM_0))
			SetStyle(0);
		if (Input::GetKeyDown(eKeyCode::NUM_1))
			SetStyle(1);
		if (Input::GetKeyDown(eKeyCode::NUM_2))
			SetStyle(2);

		if (!(STATE_HAVE(MonsterState_Dead)))
		{
			Idle();
			Alert();
			Recognize();
			Attack();
			Defense();
			Trace();
			OnHit();
			Guard();

			Groggy();

			Move();
			LookAtPlayer();
		}

		else
		{
			mCollider->Active(false);
			mActionScript->SetCheckCollider(false);
			mMonsterUI->UIOff();
		}
		if (mAnimationName != L"")
		{
			if (mbAnimReset)
			{
				mMeshData->Play(mAnimationName);
				mbAnimReset = false;
			}
			else if (mAnimationName != mMeshData->GetPlayAnimationName())
				mMeshData->Play(mAnimationName);
		}
		MonsterBase::Update();
		mBeforeState = mState;
	}
	void AshinaSoldier::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void AshinaSoldier::Render()
	{
		MonsterBase::Render();
	}
	void AshinaSoldier::DeathBlow()
	{
		//recognize 상태가 아니면 암살이다.
		if (!(STATE_HAVE(MonsterState_Recognize)))
		{
			mAnimationName = L"DeathBlowAssasinated";
			RM_STATE(MonsterState_LookAt);

		}

		//recognize 상태이면 인살이다.
		else
		{
			mAnimationName = L"DeathBlow1";

			ADD_STATE(MonsterState_LookAt);
		}

		ADD_STATE(MonsterState_OnHit);
		RM_STATE(MonsterState_Groggy);
		SetDeathBlow(false);

		GetComponent<Transform>()->SetPosition(GetPlayerPos() + mPlayerObject->GetComponent<Transform>()->Forward()
			* (mPlayerObject->GetComponent<Transform>()->GetFinalScale().z / 2
				+ GetComponent<Transform>()->GetFinalScale().z / 2 +0.5));
		SetPosture(0);
		SetResurrectionCount(GetResurrectionCount() - 1);
		RM_STATE(MonsterState_Move);
		RM_STATE(MonsterState_Attack);
		RM_STATE(MonsterState_Defense);
		RM_STATE(MonsterState_Trace);
		RM_STATE(MonsterState_Idle);
		mCamScript->SetLockOnFree();
	}
	void AshinaSoldier::Idle()
	{
		if (STATE_HAVE(MonsterState_Idle))
		{
			mAnimationName = L"Idle";
			RM_STATE(MonsterState_LookAt);
		}
	}
	void AshinaSoldier::Alert()
	{
		if (mAnimationName != L"DeathBlow1" && mAnimationName != L"DeathBlow1_Death"
			&& mAnimationName != L"DeathBlowAssasinated" && mAnimationName != L"DeathBlowAssasinated_Death"
			&& !mPlayerObject->IsStateFlag(ePlayerState::Death))
		{
			if (STATE_HAVE(MonsterState_Recognize))
			{
				RM_STATE(MonsterState_Idle);
				SetAlertnessCount(0);
				return;
			}

			//경계게이지 쌓이는 로직
			//시야는 체크만. 시야 내부에 있을경우 거리를 측정한다.
			//거리가 어느정도 가까운가에 따라 경계게이지 축적
			if (!(STATE_HAVE(MonsterState_Alert)))
			{

				float cosTheta = EyeSightCheck();
				if (cosTheta > AshinaSoldierEyeSightAngleCos)
				{
					float dist = GetDistanceToPlayer();
					float alert = GetAlertnessCount();


					if (mPlayerObject->IsStateFlag(ePlayerState::Crouch))
					{
						if (cosTheta > AshinaSoldierEyeSightAngleCos * 4)
						{
							if (dist < 5)
								SetAlertnessCount(alert + 200 * Time::DeltaTime());
							else if (dist < 7)
								SetAlertnessCount(alert + 50 * Time::DeltaTime());
							else if (dist < 10)
								SetAlertnessCount(alert + 20 * Time::DeltaTime());
							else if (dist < 12)
								SetAlertnessCount(alert + 10 * Time::DeltaTime());
							else
							{
								alert -= 10 * Time::DeltaTime();
								if (alert < 0)
									alert = 0;
								SetAlertnessCount(alert);
							}
						}
						else
						{
							alert -= 10 * Time::DeltaTime();
							if (alert < 0)
								alert = 0;
							SetAlertnessCount(alert);
						}
					}

					else
					{
						if (dist < 6)
							SetAlertnessCount(alert + 200 * Time::DeltaTime());
						else if (dist < 12)
							SetAlertnessCount(alert + 50 * Time::DeltaTime());
						else if (dist < 18)
							SetAlertnessCount(alert + 20 * Time::DeltaTime());
						else
						{
							alert -= 10 * Time::DeltaTime();
							if (alert < 0)
								alert = 0;
							SetAlertnessCount(alert);
						}
					}

				}
				else
				{
					float alert = GetAlertnessCount() - 10 * Time::DeltaTime();
					if (alert < 0)
						alert = 0;
					SetAlertnessCount(alert);
				}
				if (GetAlertnessCount() > 100)
				{
					mPlayerLastPosition = GetPlayerPos();
					ADD_STATE(MonsterState_Alert);
				}
			}

			//경계상태에서 플레이어 추적
			//플레이어 추적 실패 시 다시 Idle
			else if (STATE_HAVE(MonsterState_Alert))
			{
				mAnimationName = L"WalkWithNoSword";
				float cosTheta = EyeSightCheck();
				Vector3 dir = mPlayerLastPosition - mTransform->GetPosition();
				dir.y = 0;
				SetSpeed(200);
				GetComponent<NavMesh>()->RenewPath(mPlayerLastPosition);
				RM_STATE(MonsterState_LookAt);
				RotateForwardTo(GetComponent<NavMesh>()->GetDir());
				mbNavOn = true;
				if (dir.Length() < 4)
				{
					mAnimationName = L"LookAround";
					RM_STATE(MonsterState_Move);
				}
				else
				{
					mActionScript->Velocity(5);
					SetSpeed(AshinaSoldierWalkSpeed);
				}

				float dist = GetDistanceToPlayer();
				mAlertTimeChecker += Time::DeltaTime();

				if (mAlertTimeChecker > mAlertTime)
				{
					RM_STATE(MonsterState_Alert);
					RM_STATE(MonsterState_Move);
					mbNavOn = false;

					SetSpeed(AshinaSoldierWalkSpeed);

					ADD_STATE(MonsterState_Idle);
					mAlertTimeChecker = 0;
					SetAlertnessCount(0);
				}
				else if (cosTheta > AshinaSoldierEyeSightAngleCos && dist < 12)
				{
					mActionScript->Velocity(10);
					ADD_STATE(MonsterState_Recognize);
					Resources::Find<AudioClip>(L"recognize_sound")->Play();
					Resources::Find<AudioClip>(L"ashinasoldier_v_recognize")->Play();
					mbNavOn = false;
					ADD_STATE(MonsterState_LookAt);

				}
			}
		}
	}
	void AshinaSoldier::Recognize()
	{
		if (mPlayerObject->IsStateFlag(ePlayerState::Death))
		{
			SetRecognize(false);
			mState = 0;
			ADD_STATE(MonsterState_Idle);
			for (size_t i = 0; i < mBaseSwordInScabbard.size(); i++)
			{
				mBaseSwordInScabbard[i]->SetScale(Vector3::One);
			}
			for (size_t i = 0; i < mBaseSword.size(); i++)
			{
				mBaseSword[i]->SetScale(Vector3::Zero);
			}
			return;
		}
		if (STATE_HAVE(MonsterState_Recognize))
		{
			RM_STATE(MonsterState_Idle);
			SetRecognize(true);
			if (!(STATE_HAVE(MonsterState_DrawSword)) && !(STATE_HAVE(MonsterState_OnHit)))
			{
				mAnimationName = L"DrawSword";
				ADD_STATE(MonsterState_LookAt);
				RM_STATE(MonsterState_Move);
			}

			else if (STATE_HAVE(MonsterState_DrawSword))
			{

				if (!(STATE_HAVE(MonsterState_Attack)) && !(STATE_HAVE(MonsterState_Defense))
					&& !(STATE_HAVE(MonsterState_Trace)) && !(STATE_HAVE(MonsterState_GuardSuccess))
					&& !(STATE_HAVE(MonsterState_OnHit)) && !(STATE_HAVE(MonsterState_AttackBlocked))
					&& !(STATE_HAVE(MonsterState_Groggy)))
				{
					Vector3 pos = mTransform->GetPosition();
					Vector3 playerPos = GetPlayerPos();

					//플레이어 거리가 너무 멀어졌을 때
					float traceDist = 8;
					if (GetDistanceToPlayer() > traceDist)
					{
						ADD_STATE(MonsterState_Trace);
					}

					else if (fabsf(playerPos.y - pos.y) > 1)
					{
						ADD_STATE(MonsterState_Trace);
					}
					//플레이어 거리가 적당히 멀때 -> 근접형 공격 가능

					//플레이어 거리가 가까울 때 -> 백스텝 가능
					else
					{
						int choice = rand() % 2;
						switch (choice)
						{
						case 0:
							ADD_STATE(MonsterState_Attack);
							break;
						}
						ADD_STATE(MonsterState_Attack);

					}

				}
			}
		}

		else
			SetRecognize(false);


	}
	void AshinaSoldier::Attack()
	{
		//어택 초이스
		if (STATE_HAVE(MonsterState_Attack) && !(BEFORE_STATE_HAVE(MonsterState_Attack)))
		{
			//if (curName.substr(0, 11) == L"SwordAttack")
			//	return;

				int choice = rand() % 7;
				switch (choice)
				{
				case 0:
					mAnimationName = L"SwordAttack_1";
					break;
				case 1:
					mAnimationName = L"SwordAttack_2";
					break;
				case 2:
					mAnimationName = L"SwordAttack_3";
					break;
				case 3:
					mAnimationName = L"SwordAttack_4";
					break;
				case 4:
					mAnimationName = L"SwordAttack_5";
					break;
				case 5:
					mAnimationName = L"SwordAttack_6";
					break;
				case 6:
					mAnimationName = L"SwordAttack_7";
					break;
				}
		}

	}
	void AshinaSoldier::Defense()
	{

	}
	void AshinaSoldier::Trace()
	{
		if (STATE_HAVE(MonsterState_Trace))
		{
			mAnimationName = L"Run";


			//ADD_STATE(MonsterState_Move);
			//mMoveDir = mTransform->Forward();
			SetSpeed(200);
			Vector3 pos = mTransform->GetPosition();
			Vector3 playerPos = GetPlayerPos();
			GetComponent<NavMesh>()->RenewPath(playerPos);
			RM_STATE(MonsterState_LookAt);
			RotateForwardTo(GetComponent<NavMesh>()->GetDir());
			if (GetDistanceToPlayer() < 4)
			{
				ADD_STATE(MonsterState_LookAt);
				RM_STATE(MonsterState_Move);
				RM_STATE(MonsterState_Trace);
			}
		}

		else if(!mbNavOn)
		{
			GetComponent<NavMesh>()->SetTraceOn(false);
		}
	}

	void AshinaSoldier::Guard()
	{
		BoneAnimator* animator = mMeshData->GetAnimator();
		if (animator->GetPlayAnimationName().substr(0, 11) == L"SwordAttack")
		{
			if (animator->GetCurrentFrameIdx() < 20)
				ADD_STATE(MonsterState_Guard);
			else
				RM_STATE(MonsterState_Guard);
		}
		else if (STATE_HAVE(MonsterState_Trace) || STATE_HAVE(MonsterState_GuardSuccess)
			|| STATE_HAVE(MonsterState_OnHit)
			|| animator->GetPlayAnimationName().substr(0, 3) == L"Hit")
		{
			ADD_STATE(MonsterState_Guard);
		}
		else
		{
			RM_STATE(MonsterState_Guard);
		}
		if (STATE_HAVE(MonsterState_SuperArmor) || STATE_HAVE(MonsterState_AttackBlocked))
		{
			RM_STATE(MonsterState_Guard);
		}

		if (STATE_HAVE(MonsterState_GuardSuccess))
		{
			if (STATE_HAVE(MonsterState_GuardLeft))
				mAnimationName = L"GuardLeft";
			else
				mAnimationName = L"GuardRight";
			RM_STATE(MonsterState_OnHit);
		}
	}

	void AshinaSoldier::OnHit()
	{
		if (STATE_HAVE(MonsterState_OnHit))
		{
			if (mAnimationName == L"DeathBlow1" || mAnimationName == L"DeathBlow1_Death"
				|| mAnimationName == L"DeathBlowAssasinated" || mAnimationName == L"DeathBlowAssasinated_Death")
				;
			else if (STATE_HAVE(MonsterState_OnHitFront))
				mAnimationName = L"HitFront";
			else
				mAnimationName = L"HitBack";
		}
	}

	void AshinaSoldier::Groggy()
	{
		if (!(STATE_HAVE(MonsterState_Groggy)) && GetPosture() >= 100
			&& mAnimationName != L"DeathBlow1" && mAnimationName != L"DeathBlow1_Death"
			&& mAnimationName != L"DeathBlowAssasinated" && mAnimationName != L"DeathBlowAssasinated_Death")
		{
			SetPosture(100);
			ADD_STATE(MonsterState_Groggy);


			RM_STATE(MonsterState_Move);
			RM_STATE(MonsterState_Attack);
			RM_STATE(MonsterState_Defense);
			RM_STATE(MonsterState_Guard);
			RM_STATE(MonsterState_Trace);
			RM_STATE(MonsterState_AttackBlocked);
			RM_STATE(MonsterState_GuardSuccess);

		}
		if (STATE_HAVE(MonsterState_Groggy))
		{
			mAnimationName = L"GrogyDownParried";
		}
	}

	void AshinaSoldier::Move()
	{
		if (STATE_HAVE(MonsterState_Move))
		{
			if(mbMoveForward)
				mMoveDir = mTransform->Forward();

			mActionScript->Move(mMoveDir, GetSpeed());
		}
	}

	void AshinaSoldier::LookAtPlayer()
	{
		if (STATE_HAVE(MonsterState_LookAt))
		{
			RotateForwardTo(GetMonster2PlayerNormalize());
		}
	}

	void AshinaSoldier::SetAnimationEvent()
	{
		//mSwordScript->SetAttackLeft(false);
		//mSwordScript->SetAttackLeft(false);
		//mActionScript->SetJumpTime(0.2f);
		////애니메이션 이벤트 추가
		mMeshData->GetAnimationEndEvent(L"DrawSword") = std::bind(&AshinaSoldier::DrawSwordEndEvent, this);


		mMeshData->GetAnimationFrameEvent(L"DrawSword", 16) = [this]() {
			for (size_t i = 0; i < mBaseSwordInScabbard.size(); i++)
			{
				mBaseSwordInScabbard[i]->SetScale(Vector3::Zero);
			}
			for (size_t i = 0; i < mBaseSword.size(); i++)
			{
				mBaseSword[i]->SetScale(Vector3::One);
			}
		};

		// 
		// 
		////complete event는 애니메이션이 루프를 돌때, 한번 애니메이션을 완료할때 생기는 이벤트
		////End Event 는 애니메이션이 종료 또는 Complete될 때 생기는 이벤트
		////Start Event 는 애니메이션이 다른 애니메이션으로 전환되며 새로운 애니메이션이 시작할 때 생기는 이벤트

		//공격
		//// 우좌 베기 3000
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 12) = [this]() { mbMoveForward = true; ADD_STATE(MonsterState_Move);  SetSpeed(AshinaSoldierBaseSpeed * (float)1.5); mSwordScript->SetBlock(true); mSwordScript->SetAttackLeft(false); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 18) = [this]() {  RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 16) = [this]() {  RM_STATE(MonsterState_LookAt); AttackSoundEvent(); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 19) = std::bind(&AshinaSoldier::AttackSoundEvent, this);


		//// 좌우 베기 3001
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 1) = [this]() {mbMoveForward = true; ADD_STATE(MonsterState_Move);  SetSpeed(AshinaSoldierBaseSpeed * (float)1.5); mSwordScript->SetBlock(true); mSwordScript->SetAttackLeft(true); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 17) = [this]() { RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 19) = [this]() {  RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 15) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 5) = std::bind(&AshinaSoldier::AttackSoundEvent, this);


		//// 전진 우좌 옆베기 3003
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 10) = [this]() { mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(AshinaSoldierBaseSpeed * (float)1.5); mSwordScript->SetBlock(true); mSwordScript->SetAttackLeft(false); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 25) = [this]() { RM_STATE(MonsterState_Move); RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 26) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 5) = std::bind(&AshinaSoldier::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 24) = std::bind(&AshinaSoldier::AttackSoundEvent, this);

		//// 좌우 내려베기 3007
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 4) = [this]() {  mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(AshinaSoldierBaseSpeed * (float)1.5); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 17) = [this]() { RM_STATE(MonsterState_Move);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 20) = [this]() { RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 14) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 6) = std::bind(&AshinaSoldier::AttackSoundEvent, this);

		// 5연베기 3008
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 25) = [this]() {  mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(AshinaSoldierBaseSpeed * 2); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 34) = [this]() { RM_STATE(MonsterState_Move); RM_STATE(MonsterState_LookAt);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 40) = [this]() { ADD_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 50) = [this]() { ADD_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 57) = [this]() { RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 60) = [this]() { RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 65) = [this]() { ADD_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 77) = [this]() { RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 87) = [this]() { ADD_STATE(MonsterState_Move); ADD_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 97) = [this]() { RM_STATE(MonsterState_Move); RM_STATE(MonsterState_LookAt); };

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 31) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 45) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 58) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 75) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 75) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 96) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 27) = std::bind(&AshinaSoldier::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 55) = std::bind(&AshinaSoldier::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 94) = std::bind(&AshinaSoldier::AttackSoundEvent, this);

		////// 좌우 - 내려베기 3012
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 13) = [this]() { mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(AshinaSoldierBaseSpeed * (float)1.5);};
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 23) = [this]() { RM_STATE(MonsterState_Move); RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 37) = [this]() { ADD_STATE(MonsterState_LookAt);   };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 46) = [this]() { ADD_STATE(MonsterState_Move);   };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 54) = [this]() { RM_STATE(MonsterState_Move); RM_STATE(MonsterState_LookAt);   };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 19) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 20) = std::bind(&AshinaSoldier::AttackSoundEvent, this);

		//// 전진 찌르기 3013
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 8) = [this]() {  mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(AshinaSoldierBaseSpeed * (float)2); mSwordScript->SetBlock(false); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 26) = [this]() { RM_STATE(MonsterState_Move);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 32) = [this]() { RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 23) = std::bind(&AshinaSoldier::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 6) = std::bind(&AshinaSoldier::AttackSoundEvent, this);




		mMeshData->GetAnimationEndEvent(L"SwordAttack_1") = std::bind(&AshinaSoldier::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_2") = std::bind(&AshinaSoldier::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_3") = std::bind(&AshinaSoldier::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_4") = std::bind(&AshinaSoldier::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_5") = std::bind(&AshinaSoldier::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_6") = std::bind(&AshinaSoldier::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_7") = std::bind(&AshinaSoldier::AttackEndEvent, this);

		mMeshData->GetAnimationStartEvent(L"SwordAttack_1") = std::bind(&AshinaSoldier::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_2") = std::bind(&AshinaSoldier::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_3") = std::bind(&AshinaSoldier::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_4") = std::bind(&AshinaSoldier::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_5") = std::bind(&AshinaSoldier::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_6") = std::bind(&AshinaSoldier::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_7") = std::bind(&AshinaSoldier::AttackStartEvent, this);

		////mMeshData->GetAnimationEndEvent(L"Defense") = std::bind(&AshinaSoldier::DefenseEndEvent, this);

		mMeshData->GetAnimationFrameEvent(L"GuardLeft", 17) = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"GuardRight", 17) = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"GuardLeft") = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"GuardRight") = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationStartEvent(L"GuardLeft") = [this]() { ADD_STATE(MonsterState_GuardSuccess); };
		mMeshData->GetAnimationStartEvent(L"GuardRight") = [this]() { ADD_STATE(MonsterState_GuardSuccess); };


		mMeshData->GetAnimationStartEvent(L"ParriedLeft") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationStartEvent(L"ParriedRight") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationFrameEvent(L"ParriedLeft", 20) = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"ParriedRight", 20) = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };

		mMeshData->GetAnimationEndEvent(L"ParriedLeft") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"ParriedRight") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };



		mMeshData->GetAnimationFrameEvent(L"HitFront", 5) = [this]() {  RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"HitBack", 5) = [this]() {  RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"HitFront", 18) = [this]() { RM_STATE(MonsterState_OnHit);  mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"HitBack", 18) = [this]() { RM_STATE(MonsterState_OnHit); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"HitFront") = [this]() { RM_STATE(MonsterState_OnHit);  mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"HitBack") = [this]() { RM_STATE(MonsterState_OnHit); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationStartEvent(L"HitFront") = [this]() {Resources::Find<AudioClip>(L"ashinasoldier_v_hit")->Play();  ADD_STATE(MonsterState_OnHit);mbMoveForward = false;  ADD_STATE(MonsterState_Move); mMoveDir = -mTransform->Forward(); };
		mMeshData->GetAnimationStartEvent(L"HitBack") = [this]() { Resources::Find<AudioClip>(L"ashinasoldier_v_hit")->Play(); ADD_STATE(MonsterState_OnHit); mbMoveForward = false; ADD_STATE(MonsterState_Move); mMoveDir = -mTransform->Forward(); };


		mMeshData->GetAnimationEndEvent(L"GrogyDownParried") = [this]() { RM_STATE(MonsterState_OnHit); RM_STATE(MonsterState_Groggy); /*SetPosture(80); SetDeathBlow(false); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); */};
		mMeshData->GetAnimationStartEvent(L"GrogyDownParried") = [this]() { SetDeathBlow(true); RM_STATE(MonsterState_Guard); /*mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) - (float)0.2 * mPlayerObject->GetComponent<Transform>()->Right()); mCamScript->SetCameraZoomDistance(1.5);*/ };
		mMeshData->GetAnimationFrameEvent(L"GrogyDownParried", 70) = [this]() { SetDeathBlow(false); SetPosture(80); };

		mMeshData->GetAnimationEndEvent(L"DeathBlow1") = [this]() { RM_STATE(MonsterState_OnHit); ADD_STATE(MonsterState_Recognize); SetPosture(0); SetHp(GetMaxHP()); mMonsterUI->UIOn(); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationStartEvent(L"DeathBlow1") = [this]() { ADD_STATE(MonsterState_OnHit); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 37) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlow1_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 38) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlow1_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 39) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlow1_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 58) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 59) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 60) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 95) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 96) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };

		mMeshData->GetAnimationEndEvent(L"DeathBlow1_Death") = [this]() { mMeshData->GetAnimator()->SetStop(true); mState = 0; ADD_STATE(MonsterState_Dead); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationStartEvent(L"DeathBlow1_Death") = [this]() {ADD_STATE(MonsterState_OnHit); SetHp(0); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 3) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 40) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 41) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 42) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };

		mMeshData->GetAnimationEndEvent(L"DeathBlowAssasinated") = [this]() { RM_STATE(MonsterState_OnHit); ADD_STATE(MonsterState_Recognize); SetPosture(0); SetHp(GetMaxHP()); mMonsterUI->UIOn(); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationStartEvent(L"DeathBlowAssasinated") = [this]() { ADD_STATE(MonsterState_OnHit); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 45) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlowAssasinated_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 46) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlowAssasinated_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 47) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlowAssasinated_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 58) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 59) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 60) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 95) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 96) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };

		mMeshData->GetAnimationEndEvent(L"DeathBlowAssasinated_Death") = [this]() { mMeshData->GetAnimator()->SetStop(true); mState = 0; ADD_STATE(MonsterState_Dead); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationStartEvent(L"DeathBlowAssasinated_Death") = [this]() {ADD_STATE(MonsterState_OnHit); SetHp(0); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 3) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 40) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 41) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 42) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };


	}

	float AshinaSoldier::EyeSightCheck()
	{
		Vector3 pos = mTransform->GetPosition();

		if (GetPlayerObject() == nullptr)
			return 0.0f;

		Transform* playerTr = GetPlayerObject()->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();

		//시야체크(y축이 동일하다고 가정)
		Vector3 posY = pos;
		Vector3 playerPosY = playerPos;
		posY.y = 0; playerPosY.y = 0;
		Vector3 dir = playerPosY - posY;
		dir.Normalize();
		return dir.Dot(mTransform->Forward());
	}

	void AshinaSoldier::AttackStartEvent()
	{
		ADD_STATE(MonsterState_Attack);
		mBeforeState |= MonsterState_Attack;

	}

	float AshinaSoldier::GetDistanceToPlayer()
	{
		Vector3 pos = mTransform->GetPosition();
		Transform* playerTr = GetPlayerObject()->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();

		//거리체크(y축 스케일고려)
		Vector3 scaledPos = pos;
		Vector3 scaledPlayerPos = playerPos;

		scaledPos.y -= mTransform->GetScale().y / 2;
		scaledPlayerPos.y -= playerTr->GetScale().y / 2;

		return Vector3::Distance(scaledPos, scaledPlayerPos);
	}

	void AshinaSoldier::RotateForwardTo(Vector3 dir)
	{
		Quaternion quater = Quaternion::FromToRotation(mTransform->Forward(), dir);
		Vec3 theta = quater.ToEuler();

		theta *= 180.f / XM_PI;
		if (fabsf(theta.y) > 1)
		{
			if (theta.y > 0)
				mActionScript->Rotate(mTransform->Up(), theta.y * GetSpeed() / 100);
			else
				mActionScript->Rotate(-mTransform->Up(), theta.y * GetSpeed() / 100);
		}
	}

	void AshinaSoldier::DrawSwordEndEvent()
	{
		ADD_STATE(MonsterState_DrawSword);

		//ADD_STATE(MonsterState_Guard);
	}
	void AshinaSoldier::DefenseEndEvent()
	{
		RM_STATE(MonsterState_Defense);
	}
	void AshinaSoldier::AttackEndEvent()
	{
		RM_STATE(MonsterState_Attack);
		mBeforeState &= ~MonsterState_Attack;
		ADD_STATE(MonsterState_LookAt);
		mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f);
		mSwordScript->SetBlock(true);
		mSwordScript->SetAttackLeft(true);

		mAttackParams.damage = 10;
		mAttackParams.unGuardable = false;
		mAttackParams.special = SpecialAttack::None;
	}
	void AshinaSoldier::TraceEndEvent()
	{
		RM_STATE(MonsterState_Trace);
	}

	void AshinaSoldier::AttackSoundEvent()
	{
		std::shared_ptr<AudioClip> clip;

		switch (rand() % 4)
		{
		case 0:
			clip = Resources::Find<AudioClip>(L"ashinasoldier_v_attack_1");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 1:
			clip = Resources::Find<AudioClip>(L"ashinasoldier_v_attack_2");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 2:
			clip = Resources::Find<AudioClip>(L"ashinasoldier_v_attack_3");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 3:
			clip = Resources::Find<AudioClip>(L"ashinasoldier_v_attack_4");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		}
	}

	void AshinaSoldier::AttackSwingSoundEvent()
	{
		std::shared_ptr<AudioClip> clip;

		switch (rand() % 4)
		{
		case 0:
			clip = Resources::Find<AudioClip>(L"ashinasoldier_v_attack_swing_1");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 1:
			clip = Resources::Find<AudioClip>(L"ashinasoldier_v_attack_swing_2");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 2:
			clip = Resources::Find<AudioClip>(L"ashinasoldier_v_attack_swing_3");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 3:
			clip = Resources::Find<AudioClip>(L"ashinasoldier_v_attack_swing_4");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		}
	}

	void AshinaSoldier::OnCollisionEnter(Collider2D* collider)
	{
		GameObject* colObj = collider->GetOwner();
		eLayerType layer = colObj->GetLayerType();
		if (layer == eLayerType::PlayerProjectile)
		{
			BoneCollider* boneColObj = dynamic_cast<BoneCollider*>(colObj);
			if (boneColObj != nullptr
				&& false == boneColObj->CheckHitObjects(this)
				&& mAnimationName != L"DeathBlow1"
				&& mAnimationName != L"DeathBlow1_Death"
				&& mAnimationName != L"DeathBlowAssasinated"
				&& mAnimationName != L"DeathBlowAssasinated_Death")
			{
				//막기 상태

				boneColObj->AddHitObjects(this);
				if (STATE_HAVE(MonsterState_Guard))
				{
					//좌 막기 우 막기 구분
					Transform* tr = GetComponent<Transform>();
					Transform* colTr = colObj->GetComponent<Transform>();
					Vector3 colPos = colTr->GetWorldPositioin();
					Vector3 pos = tr->GetWorldPositioin();
					Vector3 dir = colPos - pos;
					dir.Normalize();

					Quaternion rot = Quaternion::FromToRotation(tr->Forward(), dir);
					float theta = rot.ToEuler().y;
					theta *= 180.f / XM_PI;
					if (theta <90 && theta > -90)
					{
						ADD_STATE(MonsterState_GuardSuccess);
						if (theta > 0)
							RM_STATE(MonsterState_GuardLeft);
						else if (theta < 0)
							ADD_STATE(MonsterState_GuardLeft);
					}

					//막기 상태지만 뒤를 맞는 상황
					else
					{
						SetHp(GetHP() - 20);

						if (!(STATE_HAVE(MonsterState_SuperArmor)))
						{
							ADD_STATE(MonsterState_OnHit);
							RM_STATE(MonsterState_OnHitFront);
						}

						else
							SetPosture(GetPosture() + 20);
					}


					RM_STATE(MonsterState_Move);
					RM_STATE(MonsterState_Attack);
					RM_STATE(MonsterState_Defense);
					RM_STATE(MonsterState_Trace);

					mbAnimReset = true;
					if (GetHP() == 0)
						SetPosture(GetPostureMax());
					else
						SetPosture(GetPosture() + 10);
					mMeshData->GetAnimator()->SetAnimationChangeTime(0.1f);

				}

				//맞는 상태
				else
				{
					//앞 맞기 뒤 맞기 구분
					Transform* tr = GetComponent<Transform>();
					Transform* colTr = colObj->GetComponent<Transform>();
					Vector3 colPos = colTr->GetWorldPositioin();
					Vector3 pos = tr->GetWorldPositioin();
					Vector3 dir = colPos - pos;
					dir.Normalize();

					Quaternion rot = Quaternion::FromToRotation(tr->Forward(), dir);
					float theta = rot.ToEuler().y;
					theta *= 180.f / XM_PI;

					SetHp(GetHP() - 5);
					//체간 깎기

					if (GetHP() == 0)
						SetPosture(GetPostureMax());
					else
						SetPosture(GetPosture() + 30);


					if (!(STATE_HAVE(MonsterState_SuperArmor)))
					{
						//앞에서 맞는 상황
						if (theta <90 && theta > -90)
						{
							ADD_STATE(MonsterState_OnHit);
							ADD_STATE(MonsterState_OnHitFront);
						}

						//뒤에서 맞는 상황
						else
						{
							ADD_STATE(MonsterState_OnHit);
							RM_STATE(MonsterState_OnHitFront);
						}
						mAnimationName = L"";

						RM_STATE(MonsterState_Move);
						RM_STATE(MonsterState_Attack);
						RM_STATE(MonsterState_Defense);
						RM_STATE(MonsterState_Groggy);
						RM_STATE(MonsterState_Trace);
						mbAnimReset = true;
						mMeshData->GetAnimator()->SetAnimationChangeTime(0.1f);
					}
				}


			}
		}
	}

	void AshinaSoldier::OnCollisionStay(Collider2D* collider)
	{
	}
	void AshinaSoldier::OnCollisionExit(Collider2D* collider)
	{
	}
	void AshinaSoldier::AddMonsterState(eMonsterState state)
	{
		ADD_STATE(state);
	}
	void AshinaSoldier::KatanaColliderInit()
	{
		{


			BoneCollider* Lkatana = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile, GetScene());
			Lkatana->SetMeshAndBone(mMeshData, L"Kodachi", this);
			Lkatana->SetBCOwner(this);
			mSwordScript = Lkatana->AddComponent<SoldierSwordScript>();
			
			Lkatana->SetColliderActiveFrame(L"SwordAttack_1", 20, 23);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_2", 14, 17);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_3", 29, 31);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_4", 17, 19);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_5", 34, 37);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_5", 46, 49);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_5", 58, 61);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_5", 73, 76);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_5", 98, 101);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_6", 21, 25);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_6", 54, 56);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_7", 23, 27);
			


		}
	}
	void AshinaSoldier::SetStyle(UINT style)
	{
		// 낭인 스타일
		if (style == 0)
		{
			for (size_t i = 0; i < mHat.size(); i++)
			{
				mHat[i]->SetScale(Vector3::One);
			}
			for (size_t i = 0; i < mArmor.size(); i++)
			{
				mArmor[i]->SetScale(Vector3::One);
			}
			for (size_t i = 0; i < mStraw.size(); i++)
			{
				mStraw[i]->SetScale(Vector3::One);
			}
			for (size_t i = 0; i < mAccessories.size(); i++)
			{
				mAccessories[i]->SetScale(Vector3::One);
			}
		}
		// 제일 약한 잡몹 스타일
		else if (style == 1)
		{
			for (size_t i = 0; i < mHat.size(); i++)
			{
				mHat[i]->SetScale(Vector3::Zero);
			}
			for (size_t i = 0; i < mArmor.size(); i++)
			{
				mArmor[i]->SetScale(Vector3::Zero);
			}
			for (size_t i = 0; i < mStraw.size(); i++)
			{
				mStraw[i]->SetScale(Vector3::Zero);
			}
			for (size_t i = 0; i < mAccessories.size(); i++)
			{
				mAccessories[i]->SetScale(Vector3::Zero);
			}
		}

		// 강한몹 스타일
		else
		{
			for (size_t i = 0; i < mHat.size(); i++)
			{
				mHat[i]->SetScale(Vector3::One);
			}
			for (size_t i = 0; i < mArmor.size(); i++)
			{
				mArmor[i]->SetScale(Vector3::One);
			}
			for (size_t i = 0; i < mStraw.size(); i++)
			{
				mStraw[i]->SetScale(Vector3::Zero);
			}
			for (size_t i = 0; i < mAccessories.size(); i++)
			{
				mAccessories[i]->SetScale(Vector3::Zero);
			}
		}
	}
}