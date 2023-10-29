#include "yaAshinaSpearMan.h"
#include "yaMeshData.h"

//test
#include "yaInput.h"
#include "yaCollider2D.h"
#include "yaBoneAnimator.h"
#include "yaBoneCollider.h"
#include "yaActionScript.h"
#include "yaPlayer.h"
#include "yaMonsterCollisionScript.h"
#include "yaSpearManSwordScript.h"

#include "yaNavMesh.h"


#define STATE_HAVE(STATE) (mState & STATE) == STATE
#define ADD_STATE(STATE) mState |= STATE
#define RM_STATE(STATE) mState &= ~STATE

#define BEFORE_STATE_HAVE(STATE) (mBeforeState & STATE) == STATE

namespace ya
{
	float AshinaSpearManEyeSightAngleCos = 0.2f;
	float AshinaSpearManWalkSpeed = 60;
	float AshinaSpearManBaseSpeed = 70;
	AshinaSpearMan::AshinaSpearMan()
		: MonsterBase()
		, mAlertTime(10)
	{
	}
	AshinaSpearMan::~AshinaSpearMan()
	{
	}
	void AshinaSpearMan::Initialize()
	{
		MonsterBase::Initialize();
		SetName(L"AshinaSpearManObject");

		////fbx 로드
		//mMeshData = MeshData::LoadFromFbx(L"Monster\\AshinaSpearMan\\Mesh\\c1010.fbx");

		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_000000.fbx", L"Idle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_000600.fbx", L"LookAround");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_001040.fbx", L"DrawSword");

		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_003000.fbx", L"SwordAttack_1"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_003001.fbx", L"SwordAttack_2"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_003003.fbx", L"SwordAttack_3"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_003007.fbx", L"SwordAttack_4"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_003008.fbx", L"SwordAttack_5"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_003012.fbx", L"SwordAttack_6"); // 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_003013.fbx", L"SwordAttack_7"); // 

		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_005000.fbx", L"WalkWithNoSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_405000.fbx", L"WalkFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_405001.fbx", L"WalkBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_405002.fbx", L"WalkLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_405003.fbx", L"WalkRight");

		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_405010.fbx", L"Run");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_005211.fbx", L"RunBack");

		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008010.fbx", L"HitFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008011.fbx", L"HitBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008012.fbx", L"HitLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008013.fbx", L"HitRight");

		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008300.fbx", L"GrogyDownFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008301.fbx", L"GrogyDownBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008550.fbx", L"GrogyDownParried");

		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008500.fbx", L"GuardLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008501.fbx", L"GuardRight");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008600.fbx", L"ParriedLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_008601.fbx", L"ParriedRight");

		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_010000.fbx", L"Death");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_012000.fbx", L"DeathBlow2");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_012001.fbx", L"DeathBlow2_Death");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_012100.fbx", L"GroggyParried");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_012110.fbx", L"DeathBlow1");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_012111.fbx", L"DeathBlow1_Death");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_012200.fbx", L"DeathBlowAssasinated");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_012201.fbx", L"DeathBlowAssasinated_Death");

		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_500000.fbx", L"Defense");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_501040.fbx", L"UnDefense");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_505000.fbx", L"DefenseMoveFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_505001.fbx", L"DefenseMoveBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_505002.fbx", L"DefenseMoveLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a000_505003.fbx", L"DefenseMoveRight");


		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_000000.fbx", L"Spear_Idle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_400600.fbx", L"Spear_LookAround");
		// 
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
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_005000.fbx", L"Spear_WalkWithNoSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_405000.fbx", L"Spear_WalkFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_405001.fbx", L"Spear_WalkBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_405002.fbx", L"Spear_WalkLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_405003.fbx", L"Spear_WalkRight");
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_405010.fbx", L"Spear_Run");
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_008500.fbx", L"Spear_GuardLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_008501.fbx", L"Spear_GuardRight");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_008550.fbx", L"Spear_GrogyDownParried");
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_008600.fbx", L"Spear_ParriedLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_008601.fbx", L"Spear_ParriedRight");
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_500000.fbx", L"Spear_Defense");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_501040.fbx", L"Spear_UnDefense");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_505000.fbx", L"Spear_DefenseMoveFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_505001.fbx", L"Spear_DefenseMoveBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_505002.fbx", L"Spear_DefenseMoveLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\AshinaSpearMan\\Animation\\a200_505003.fbx", L"Spear_DefenseMoveRight");

		//mMeshData->AnimationSave(L"Monster\\AshinaSpearMan\\AnimationData\\AshinaSpearMan.animationdata");
		//// 

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
		animator->SetAnimationSelfChange(L"Spear_Run", false);
		animator->SetAnimationSelfChange(L"Spear_WalkFront", false);
		//애니메이션 끝부분 이상한 것 잘라내는 시간
		animator->SetAnimationTailTime(0.1f);


		std::vector<GameObject*> childObjects = mMeshData->GetChildObjects();
		std::vector<std::vector<std::shared_ptr<Material>>> materials = mMeshData->GetMaterialsVec();
		for (size_t i = 0; i < materials.size(); i++)
		{
			if (materials[i][0]->GetName() == L"#22#.001")
				mHat.push_back(childObjects[i]->GetComponent<Transform>());
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
			if (materials[i][0]->GetName() == L"#30#")//Scabbard
				childObjects[i]->GetComponent<Transform>()->SetScale(Vector3::Zero);
		}
		for (size_t i = 0; i < mSpear.size(); i++)
		{
			mBaseSword[i]->SetScale(Vector3::Zero);
		}
		for (size_t i = 0; i < mBaseSwordInScabbard.size(); i++)
		{
			mBaseSwordInScabbard[i]->SetScale(Vector3::Zero);
		}

		//SetPlayerObject(dynamic_cast<Player*>(SceneManager::GetActiveScene()->GetPlayer()));
		SetPlayerObject(dynamic_cast<Player*>(GetScene()->GetPlayer()));

		//몬스터 스테이트
		SetSpeed(AshinaSpearManBaseSpeed);


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
	}
	void AshinaSpearMan::Update()
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
	void AshinaSpearMan::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void AshinaSpearMan::Render()
	{
		MonsterBase::Render();
	}
	void AshinaSpearMan::DeathBlow()
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
				+ GetComponent<Transform>()->GetFinalScale().z / 2 + 1));
		SetPosture(0);
		SetResurrectionCount(GetResurrectionCount() - 1);
		RM_STATE(MonsterState_Move);
		RM_STATE(MonsterState_Attack);
		RM_STATE(MonsterState_Defense);
		RM_STATE(MonsterState_Trace);
		RM_STATE(MonsterState_Idle);
		mCamScript->SetLockOnFree();
	}
	void AshinaSpearMan::Idle()
	{
		if (STATE_HAVE(MonsterState_Idle))
		{
			mAnimationName = L"Spear_Idle";
			RM_STATE(MonsterState_LookAt);
		}
	}
	void AshinaSpearMan::Alert()
	{
		if (mAnimationName != L"DeathBlow1" && mAnimationName != L"DeathBlow1_Death"
			&& mAnimationName != L"DeathBlowAssasinated" && mAnimationName != L"DeathBlowAssasinated_Death")
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
				if (cosTheta > AshinaSpearManEyeSightAngleCos)
				{
					float dist = GetDistanceToPlayer();
					float alert = GetAlertnessCount();

					if (dist < 15)
						SetAlertnessCount(alert + 50 * Time::DeltaTime());
					else if (dist < 40)
						SetAlertnessCount(alert + 20 * Time::DeltaTime());

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
				mAnimationName = L"Spear_WalkWithNoSword";
				float cosTheta = EyeSightCheck();
				Vector3 dir = mPlayerLastPosition - mTransform->GetPosition();
				dir.y = 0;

				if (dir.Length() < 5)
				{
					mAnimationName = L"Spear_LookAround";
					RM_STATE(MonsterState_Move);
				}
				else
				{
					dir.Normalize();
					mMoveDir = dir;
					ADD_STATE(MonsterState_Move);
					mActionScript->Velocity(10);
					SetSpeed(AshinaSpearManWalkSpeed);
				}


				RotateForwardTo(dir);


				float dist = GetDistanceToPlayer();
				mAlertTimeChecker += Time::DeltaTime();

				if (mAlertTimeChecker > mAlertTime)
				{
					RM_STATE(MonsterState_Alert);
					RM_STATE(MonsterState_Move);
					SetSpeed(AshinaSpearManWalkSpeed);

					ADD_STATE(MonsterState_Idle);
					mAlertTimeChecker = 0;
					SetAlertnessCount(0);
				}
				if (cosTheta > AshinaSpearManEyeSightAngleCos && dist < 20)
				{
					mActionScript->Velocity(18);
					ADD_STATE(MonsterState_Recognize);
				}
			}
		}
	}
	void AshinaSpearMan::Recognize()
	{
		if (STATE_HAVE(MonsterState_Recognize))
		{
			RM_STATE(MonsterState_Idle);
			SetRecognize(true);

			

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

		else
			SetRecognize(false);


	}
	void AshinaSpearMan::Attack()
	{
		//어택 초이스
		if (STATE_HAVE(MonsterState_Attack) && !(BEFORE_STATE_HAVE(MonsterState_Attack)))
		{
			//if (curName.substr(0, 11) == L"SwordAttack")
			//	return;

			int choice = rand() % 5;
			switch (choice)
			{
			case 0:
				mAnimationName = L"Spear_SwordAttack_1";
				break;
			case 1:
				mAnimationName = L"Spear_SwordAttack_2";
				break;
			case 2:
				mAnimationName = L"Spear_SwordAttack_3";
				break;
			case 3:
				mAnimationName = L"Spear_SwordAttack_4";
				break;
			case 4:
				mAnimationName = L"Spear_SwordAttack_5";
				break;

			}

		}

	}
	void AshinaSpearMan::Defense()
	{

	}
	void AshinaSpearMan::Trace()
	{
		if (STATE_HAVE(MonsterState_Trace))
		{
			mAnimationName = L"Spear_Run";


			//ADD_STATE(MonsterState_Move);
			mMoveDir = mTransform->Forward();
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

		else
		{
			GetComponent<NavMesh>()->SetTraceOn(false);
		}
	}

	void AshinaSpearMan::Guard()
	{
		BoneAnimator* animator = mMeshData->GetAnimator();
		if (animator->GetPlayAnimationName().substr(0, 20) == L"Spear_SwordAttack")
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
				mAnimationName = L"Spear_GuardLeft";
			else
				mAnimationName = L"Spear_GuardRight";
			RM_STATE(MonsterState_OnHit);
		}
	}

	void AshinaSpearMan::OnHit()
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

	void AshinaSpearMan::Groggy()
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
			mAnimationName = L"Spear_GrogyDownParried";
		}
	}

	void AshinaSpearMan::Move()
	{
		if (STATE_HAVE(MonsterState_Move))
		{
			mMoveDir = mTransform->Forward();

			mActionScript->Move(mMoveDir, GetSpeed());
		}
	}

	void AshinaSpearMan::LookAtPlayer()
	{
		if (STATE_HAVE(MonsterState_LookAt))
		{
			RotateForwardTo(GetMonster2PlayerNormalize());
		}
	}

	void AshinaSpearMan::SetAnimationEvent()
	{
		//mSwordScript->SetAttackLeft(false);
		//mSwordScript->SetAttackLeft(false);
		//mActionScript->SetJumpTime(0.2f);
		////애니메이션 이벤트 추가

		// 
		// 
		////complete event는 애니메이션이 루프를 돌때, 한번 애니메이션을 완료할때 생기는 이벤트
		////End Event 는 애니메이션이 종료 또는 Complete될 때 생기는 이벤트
		////Start Event 는 애니메이션이 다른 애니메이션으로 전환되며 새로운 애니메이션이 시작할 때 생기는 이벤트

		//공격
		//// 우좌 베기 3000
		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_1", 5) = [this]() {SetAttackUnGuardable(true); };

		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_1", 12) = [this]() {ADD_STATE(MonsterState_Move);  SetSpeed(AshinaSpearManBaseSpeed * (float)1.3); mSwordScript->SetBlock(false); };
		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_1", 23) = [this]() {  RM_STATE(MonsterState_Move);  RM_STATE(MonsterState_LookAt); };

		//// 좌우 베기 3001
		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_2", 15) = [this]() {SetAttackUnGuardable(true); };

		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_2", 20) = [this]() {ADD_STATE(MonsterState_Move);  SetSpeed(AshinaSpearManBaseSpeed * (float)2); mSwordScript->SetBlock(false);  };
		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_2", 32) = [this]() { RM_STATE(MonsterState_Move); RM_STATE(MonsterState_LookAt);  };


		//// 전진 우좌 옆베기 3003
		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_3", 15) = [this]() {SetAttackUnGuardable(true); };

		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_3", 30) = [this]() { ADD_STATE(MonsterState_Move); SetSpeed(AshinaSpearManBaseSpeed * (float)1.5); mSwordScript->SetBlock(false); RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_3", 60) = [this]() { RM_STATE(MonsterState_Move);};
		
		
		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_4", 1) = [this]() {SetAttackUnGuardable(true); };




		// 5연베기 3008
		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_5", 15) = [this]() {SetAttackUnGuardable(true); };

		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_5", 29) = [this]() {  ADD_STATE(MonsterState_Move); SetSpeed(AshinaSpearManBaseSpeed * 2); mSwordScript->SetBlock(false); };
		mMeshData->GetAnimationFrameEvent(L"Spear_SwordAttack_5", 42) = [this]() { RM_STATE(MonsterState_Move); RM_STATE(MonsterState_LookAt);  };






		mMeshData->GetAnimationEndEvent(L"Spear_SwordAttack_1") = std::bind(&AshinaSpearMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"Spear_SwordAttack_2") = std::bind(&AshinaSpearMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"Spear_SwordAttack_3") = std::bind(&AshinaSpearMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"Spear_SwordAttack_4") = std::bind(&AshinaSpearMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"Spear_SwordAttack_5") = std::bind(&AshinaSpearMan::AttackEndEvent, this);

		mMeshData->GetAnimationStartEvent(L"Spear_SwordAttack_1") = std::bind(&AshinaSpearMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"Spear_SwordAttack_2") = std::bind(&AshinaSpearMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"Spear_SwordAttack_3") = std::bind(&AshinaSpearMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"Spear_SwordAttack_4") = std::bind(&AshinaSpearMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"Spear_SwordAttack_5") = std::bind(&AshinaSpearMan::AttackStartEvent, this);

		////mMeshData->GetAnimationEndEvent(L"Defense") = std::bind(&AshinaSpearMan::DefenseEndEvent, this);

		mMeshData->GetAnimationFrameEvent(L"Spear_GuardLeft", 17) = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"Spear_GuardRight", 17) = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"Spear_GuardLeft") = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"Spear_GuardRight") = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationStartEvent(L"Spear_GuardLeft") = [this]() { ADD_STATE(MonsterState_GuardSuccess); };
		mMeshData->GetAnimationStartEvent(L"Spear_GuardRight") = [this]() { ADD_STATE(MonsterState_GuardSuccess); };


		mMeshData->GetAnimationStartEvent(L"Spear_ParriedLeft") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationStartEvent(L"Spear_ParriedRight") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationFrameEvent(L"Spear_ParriedLeft", 20) = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"Spear_ParriedRight", 20) = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };

		mMeshData->GetAnimationEndEvent(L"Spear_ParriedLeft") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"Spear_ParriedRight") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };



		mMeshData->GetAnimationFrameEvent(L"HitFront", 18) = [this]() { RM_STATE(MonsterState_OnHit);  mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"HitBack", 18) = [this]() { RM_STATE(MonsterState_OnHit); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"HitFront") = [this]() { RM_STATE(MonsterState_OnHit);  mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"HitBack") = [this]() { RM_STATE(MonsterState_OnHit); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationStartEvent(L"HitFront") = [this]() { ADD_STATE(MonsterState_OnHit); };
		mMeshData->GetAnimationStartEvent(L"HitBack") = [this]() { ADD_STATE(MonsterState_OnHit); };


		mMeshData->GetAnimationEndEvent(L"Spear_GrogyDownParried") = [this]() { RM_STATE(MonsterState_OnHit); RM_STATE(MonsterState_Groggy); SetPosture(80); SetDeathBlow(false); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationStartEvent(L"Spear_GrogyDownParried") = [this]() { SetDeathBlow(true); RM_STATE(MonsterState_Guard); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) - (float)0.2 * mPlayerObject->GetComponent<Transform>()->Right()); mCamScript->SetCameraZoomDistance(1.5); };
		mMeshData->GetAnimationFrameEvent(L"Spear_GrogyDownParried", 70) = [this]() { SetDeathBlow(false); SetPosture(80); };

		mMeshData->GetAnimationEndEvent(L"DeathBlow1") = [this]() { RM_STATE(MonsterState_OnHit); ADD_STATE(MonsterState_Recognize); SetPosture(0); SetHp(GetMaxHP()); mMonsterUI->UIOn(); };
		mMeshData->GetAnimationStartEvent(L"DeathBlow1") = [this]() { ADD_STATE(MonsterState_OnHit); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 37) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlow1_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 58) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 95) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };

		mMeshData->GetAnimationEndEvent(L"DeathBlow1_Death") = [this]() { mMeshData->GetAnimator()->SetStop(true); mState = 0; ADD_STATE(MonsterState_Dead); };
		mMeshData->GetAnimationStartEvent(L"DeathBlow1_Death") = [this]() {ADD_STATE(MonsterState_OnHit); SetHp(0); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 3) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 40) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };

		mMeshData->GetAnimationEndEvent(L"DeathBlowAssasinated") = [this]() { RM_STATE(MonsterState_OnHit); ADD_STATE(MonsterState_Recognize); SetPosture(0); SetHp(GetMaxHP()); mMonsterUI->UIOn(); };
		mMeshData->GetAnimationStartEvent(L"DeathBlowAssasinated") = [this]() { ADD_STATE(MonsterState_OnHit); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 45) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlowAssasinated_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 58) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 95) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };

		mMeshData->GetAnimationEndEvent(L"DeathBlowAssasinated_Death") = [this]() { mMeshData->GetAnimator()->SetStop(true); mState = 0; ADD_STATE(MonsterState_Dead); };
		mMeshData->GetAnimationStartEvent(L"DeathBlowAssasinated_Death") = [this]() {ADD_STATE(MonsterState_OnHit); SetHp(0); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 3) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 40) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };


	}

	float AshinaSpearMan::EyeSightCheck()
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

	void AshinaSpearMan::AttackStartEvent()
	{
		ADD_STATE(MonsterState_Attack);
		mBeforeState |= MonsterState_Attack;

	}

	float AshinaSpearMan::GetDistanceToPlayer()
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

	void AshinaSpearMan::RotateForwardTo(Vector3 dir)
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

	void AshinaSpearMan::DrawSwordEndEvent()
	{
		ADD_STATE(MonsterState_DrawSword);

		//ADD_STATE(MonsterState_Guard);
	}
	void AshinaSpearMan::DefenseEndEvent()
	{
		RM_STATE(MonsterState_Defense);
	}
	void AshinaSpearMan::AttackEndEvent()
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
	void AshinaSpearMan::TraceEndEvent()
	{
		RM_STATE(MonsterState_Trace);
	}

	void AshinaSpearMan::OnCollisionEnter(Collider2D* collider)
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
						SetHp(GetHP() - 5);

						if (!(STATE_HAVE(MonsterState_SuperArmor)))
						{
							ADD_STATE(MonsterState_OnHit);
							RM_STATE(MonsterState_OnHitFront);
						}
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
						SetPosture(GetPosture() + 10);


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

	void AshinaSpearMan::OnCollisionStay(Collider2D* collider)
	{
	}
	void AshinaSpearMan::OnCollisionExit(Collider2D* collider)
	{
	}
	void AshinaSpearMan::AddMonsterState(eMonsterState state)
	{
		ADD_STATE(state);
	}
	void AshinaSpearMan::KatanaColliderInit()
	{
		{


			BoneCollider* Lkatana = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile, GetScene());
			Lkatana->SetMeshAndBone(mMeshData, L"Spear", this);
			Lkatana->SetBCOwner(this);
			mSwordScript = Lkatana->AddComponent<SpearManSwordScript>();

			Lkatana->SetColliderActiveFrame(L"Spear_SwordAttack_1", 23, 25);
			Lkatana->SetColliderActiveFrame(L"Spear_SwordAttack_2", 30, 33);
			Lkatana->SetColliderActiveFrame(L"Spear_SwordAttack_3", 32, 69);
			Lkatana->SetColliderActiveFrame(L"Spear_SwordAttack_4", 16, 19);
			Lkatana->SetColliderActiveFrame(L"Spear_SwordAttack_5", 37, 40);



		}
	}
	void AshinaSpearMan::SetStyle(UINT style)
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