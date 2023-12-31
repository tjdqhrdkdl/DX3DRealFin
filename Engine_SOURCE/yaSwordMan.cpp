#include "yaSwordMan.h"
#include "yaMeshData.h"

//test
#include "yaInput.h"
#include "yaCollider2D.h"
#include "yaBoneAnimator.h"
#include "yaBoneCollider.h"
#include "yaActionScript.h"
#include "yaPlayer.h"
#include "yaMonsterCollisionScript.h"
#include "yaSwordManSwordScript.h"

#include "yaNavMesh.h"
#include "yaAudioClip.h"


#define STATE_HAVE(STATE) (mState & STATE) == STATE
#define ADD_STATE(STATE) mState |= STATE
#define RM_STATE(STATE) mState &= ~STATE

#define BEFORE_STATE_HAVE(STATE) (mBeforeState & STATE) == STATE

namespace ya
{
	float SwordManEyeSightAngleCos = 0.1f;
	float SwordManWalkSpeed = 71;
	float SwordManBaseSpeed = 200;
	SwordMan::SwordMan()
		: MonsterBase()
		, mAlertTime(10)
	{
	}
	SwordMan::~SwordMan()
	{
	}
	void SwordMan::Initialize()
	{
		MonsterBase::Initialize();
		SetName(L"SwordManObject");

		////fbx 로드
		//mMeshData = MeshData::LoadFromFbx(L"Monster\\SwordMan\\Mesh\\c1700_SwordMan.fbx");
		//
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_400000.fbx", L"Idle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_400600.fbx", L"LookAround");
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003000.fbx", L"SwordAttack_1"); // 전진하며, 양손 가운데 내려베기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003001.fbx", L"SwordAttack_2"); // 전진하며, 양손 x자 베기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003010.fbx", L"SwordAttack_3"); // 오른손 내려베기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003011.fbx", L"SwordAttack_4"); // 왼손 내려베기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003006.fbx", L"SwordAttack_5"); // 좌측 에서 우측으로 두번 베기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003007.fbx", L"SwordAttack_6"); // 양손 찌르기 , 못 막는 공격
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003012.fbx", L"SwordAttack_7"); // 양손 x자 베기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003016.fbx", L"SwordAttack_8"); // 양손 우에서 좌로 베기
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405000.fbx", L"WalkFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405001.fbx", L"WalkBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405002.fbx", L"WalkLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405003.fbx", L"WalkRight");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405010.fbx", L"Run");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a100_005211.fbx", L"RunBack");

		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008010.fbx", L"HitFront"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008011.fbx", L"HitBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008012.fbx", L"HitLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008013.fbx", L"HitRight"); 

		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008300.fbx", L"GrogyDownFront"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008301.fbx", L"GrogyDownBack"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008550.fbx", L"GrogyDownParried");

		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008500.fbx", L"GuardLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008501.fbx", L"GuardRight"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008600.fbx", L"ParriedLeft"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008601.fbx", L"ParriedRight"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008602.fbx", L"ParriedBoth"); 


		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_010000.fbx", L"Death"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_012100.fbx", L"GroggyParried"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_012110.fbx", L"DeathBlow1"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_012111.fbx", L"DeathBlow1_Death"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_012200.fbx", L"DeathBlowAssasinated"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_012201.fbx", L"DeathBlowAssasinated_Death"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_500000.fbx", L"Defense");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_501040.fbx", L"UnDefense");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505000.fbx", L"DefenseMoveFront");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505001.fbx", L"DefenseMoveBack");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505002.fbx", L"DefenseMoveLeft");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505003.fbx", L"DefenseMoveRight");
		//mMeshData->AnimationSave(L"Monster\\SwordMan\\AnimationData\\SwordManAnimation_1.animationdata");
		// 
		// 
		mMeshData = std::make_shared<MeshData>();
		mMeshData->Load(L"Monster\\SwordMan\\MeshData\\c1700_SwordMan.meshdata");
		mMeshData->AnimationLoad(L"Monster\\SwordMan\\AnimationData\\SwordManAnimation_1.animationdata");
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
		animator->SetAnimationTailTime(0.1f);



		//SetPlayerObject(dynamic_cast<Player*>(SceneManager::GetActiveScene()->GetPlayer()));
		SetPlayerObject(dynamic_cast<Player*>(GetScene()->GetPlayer()));

		//몬스터 스테이트
		SetSpeed(SwordManBaseSpeed);


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
		Resources::Load<AudioClip>(L"swordman_v_recognize", L"..\\Resources\\Sound\\swordman\\Recognize\\c170008800c.wav");
		Resources::Load<AudioClip>(L"swordman_v_hit", L"..\\Resources\\Sound\\swordman\\Hit\\c170008100.wav");
		Resources::Load<AudioClip>(L"swordman_v_attack_1", L"..\\Resources\\Sound\\swordman\\Attack\\c170008500.wav");
		Resources::Load<AudioClip>(L"swordman_v_attack_2", L"..\\Resources\\Sound\\swordman\\Attack\\c170008500b.wav");
		Resources::Load<AudioClip>(L"swordman_v_attack_3", L"..\\Resources\\Sound\\swordman\\Attack\\c170008500c.wav");
		Resources::Load<AudioClip>(L"swordman_v_attack_4", L"..\\Resources\\Sound\\swordman\\Attack\\c170008500d.wav");
		Resources::Load<AudioClip>(L"swordman_v_attack_swing_1", L"..\\Resources\\Sound\\swordman\\Attack\\c170006000.wav");
		Resources::Load<AudioClip>(L"swordman_v_attack_swing_2", L"..\\Resources\\Sound\\swordman\\Attack\\c170006000b.wav");
		Resources::Load<AudioClip>(L"swordman_v_attack_swing_3", L"..\\Resources\\Sound\\swordman\\Attack\\c170006000c.wav");
		Resources::Load<AudioClip>(L"swordman_v_attack_swing_4", L"..\\Resources\\Sound\\swordman\\Attack\\c170006000d.wav");


	}

	void SwordMan::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N_1))
			ADD_STATE(MonsterState_Alert);
		if (Input::GetKeyDown(eKeyCode::N_2))
			ADD_STATE(MonsterState_Recognize);
		if (Input::GetKeyDown(eKeyCode::N_3))
			ADD_STATE(MonsterState_Idle);
		if (Input::GetKeyDown(eKeyCode::N_4))
			mState = 0;

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
	void SwordMan::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void SwordMan::Render()
	{
		MonsterBase::Render();
	}
	void SwordMan::DeathBlow()
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
				+ GetComponent<Transform>()->GetFinalScale().z / 2 + 1 ));
		SetPosture(0);
		SetResurrectionCount(GetResurrectionCount() - 1);
		RM_STATE(MonsterState_Move);
		RM_STATE(MonsterState_Attack);
		RM_STATE(MonsterState_Defense);
		RM_STATE(MonsterState_Trace);
		RM_STATE(MonsterState_Idle);
		mCamScript->SetLockOnFree();
	}
	void SwordMan::Idle()
	{
		if (STATE_HAVE(MonsterState_Idle))
		{
			mAnimationName = L"Idle";
			RM_STATE(MonsterState_LookAt);
		}
	}
	void SwordMan::Alert()
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
				if (cosTheta > SwordManEyeSightAngleCos)
				{
					float dist = GetDistanceToPlayer();
					float alert = GetAlertnessCount();

					if (mPlayerObject->IsStateFlag(ePlayerState::Crouch))
					{
						if (cosTheta > SwordManEyeSightAngleCos * 4)
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
					SetSpeed(SwordManWalkSpeed);
				}

				float dist = GetDistanceToPlayer();
				mAlertTimeChecker += Time::DeltaTime();

				if (mAlertTimeChecker > mAlertTime)
				{
					RM_STATE(MonsterState_Alert);
					RM_STATE(MonsterState_Move);
					mbNavOn = false;

					SetSpeed(SwordManWalkSpeed);

					ADD_STATE(MonsterState_Idle);
					mAlertTimeChecker = 0;
					SetAlertnessCount(0);
				}
				else if (cosTheta > SwordManEyeSightAngleCos && dist < 12)
				{
					mActionScript->Velocity(10);
					ADD_STATE(MonsterState_Recognize);
					ADD_STATE(MonsterState_LookAt	);
					Resources::Find<AudioClip>(L"recognize_sound")->Play();
					Resources::Find<AudioClip>(L"ashinasoldier_v_recognize")->Play();
					mbNavOn = false;

				}
			}
		}
	}
	void SwordMan::Recognize()
	{
		if (mPlayerObject->IsStateFlag(ePlayerState::Death))
		{
			SetRecognize(false);
			mState = 0;
			ADD_STATE(MonsterState_Idle);
			return;
		}
		if (STATE_HAVE(MonsterState_Recognize))
		{
			SetAlertnessCount(0);
			RM_STATE(MonsterState_Idle);
			SetRecognize(true);


			if (!(STATE_HAVE(MonsterState_Attack)) && !(STATE_HAVE(MonsterState_Defense))
				&& !(STATE_HAVE(MonsterState_Trace)) && !(STATE_HAVE(MonsterState_GuardSuccess))
				&& !(STATE_HAVE(MonsterState_OnHit)) && !(STATE_HAVE(MonsterState_AttackBlocked))
				&& !(STATE_HAVE(MonsterState_Groggy)))
			{
				//bool a = STATE_HAVE(MonsterState_Attack);
				//bool b = STATE_HAVE(MonsterState_Trace);
				//bool c = STATE_HAVE(MonsterState_Groggy);
				//bool d = STATE_HAVE(MonsterState_OnHit);
				//bool e = STATE_HAVE(MonsterState_AttackBlocked);
				//bool f = STATE_HAVE(MonsterState_Defense);
				//bool g = STATE_HAVE(MonsterState_GuardSuccess);

				Vector3 pos = mTransform->GetPosition();
				Vector3 playerPos = GetPlayerPos();

				//플레이어 거리가 너무 멀어졌을 때
				float traceDist = 7;
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
	void SwordMan::Attack()
	{
		//어택 초이스
		if (STATE_HAVE(MonsterState_Attack) && !(BEFORE_STATE_HAVE(MonsterState_Attack)))
		{
			//if (curName.substr(0, 11) == L"SwordAttack")
			//	return;
			if (GetDistanceToPlayer() > 6)
			{
				int choice = rand() % 2;
				switch (choice)
				{
				case 0:
					mAnimationName = L"SwordAttack_1";
					break;
				case 1:
					mAnimationName = L"SwordAttack_2";
					break;
				}
			}
			else
			{
				int choice = rand() % 6;

				switch (choice)
				{
				case 0:
					mAnimationName = L"SwordAttack_3";
					break;
				case 1:
					mAnimationName = L"SwordAttack_4";
					break;
				case 2:
					mAnimationName = L"SwordAttack_5";
					break;
				case 3:
					mAnimationName = L"SwordAttack_6";
					break;
				case 4:
					mAnimationName = L"SwordAttack_7";
					break;
				case 5:
					mAnimationName = L"SwordAttack_8";
					break;
				}
			}

		}

	}
	void SwordMan::Defense()
	{

	}
	void SwordMan::Trace()
	{
		if (STATE_HAVE(MonsterState_Trace))
		{
			mAnimationName = L"Run";


			//ADD_STATE(MonsterState_Move);
			mMoveDir = mTransform->Forward();
			SetSpeed(200);
			Vector3 pos = mTransform->GetPosition();
			Vector3 playerPos = GetPlayerPos();
			GetComponent<NavMesh>()->RenewPath(playerPos);
			RM_STATE(MonsterState_LookAt);
			RotateForwardTo(GetComponent<NavMesh>()->GetDir());
			if (GetDistanceToPlayer() < 5)
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

	void SwordMan::Guard()
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

	void SwordMan::OnHit()
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

	void SwordMan::Groggy()
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


	void SwordMan::Move()
	{
		if (STATE_HAVE(MonsterState_Move))
		{
			if (mbMoveForward)
				mMoveDir = mTransform->Forward();

			mActionScript->Move(mMoveDir, GetSpeed());
		}
	}

	void SwordMan::LookAtPlayer()
	{
		if (STATE_HAVE(MonsterState_LookAt))
		{
			RotateForwardTo(GetMonster2PlayerNormalize());
		}
	}

	void SwordMan::SetAnimationEvent()
	{
		//mRSwordScript->SetAttackLeft(false);
		//mLSwordScript->SetAttackLeft(false);
		//mActionScript->SetJumpTime(0.2f);
		////애니메이션 이벤트 추가
		////complete event는 애니메이션이 루프를 돌때, 한번 애니메이션을 완료할때 생기는 이벤트
		////End Event 는 애니메이션이 종료 또는 Complete될 때 생기는 이벤트
		////Start Event 는 애니메이션이 다른 애니메이션으로 전환되며 새로운 애니메이션이 시작할 때 생기는 이벤트
		//mMeshData->GetAnimationEndEvent(L"DrawSword") = std::bind(&SwordMan::DrawSwordEndEvent, this);

		////공격
		//// 중거리 근접기 3000
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 1) = [this]() {  mbMoveForward = true; ADD_STATE(MonsterState_Move);  SetSpeed(SwordManBaseSpeed * (float)1.5); mLSwordScript->SetBlock(false); mLSwordScript->SetAttackDir(2); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 28) = [this]() {  RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 40) = [this]() {  RM_STATE(MonsterState_LookAt); };

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 80) = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 6) = std::bind(&SwordMan::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 39) = std::bind(&SwordMan::AttackSwingSoundEvent, this);
		//// 중거리 근접기 2 3001
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 1) = [this]() { mbMoveForward = true; ADD_STATE(MonsterState_Move);  SetSpeed(SwordManBaseSpeed * (float)0.7); mLSwordScript->SetBlock(true); mLSwordScript->SetAttackDir(0); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 50) = [this]() { RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 55) = [this]() {  RM_STATE(MonsterState_LookAt); };		
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 92) = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 27) = std::bind(&SwordMan::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 53) = std::bind(&SwordMan::AttackSwingSoundEvent, this);


		//// 왼손 내려베기 3010
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 1) = [this]() {mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(SwordManBaseSpeed); mRSwordScript->SetBlock(true); mRSwordScript->SetAttackDir(1); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 25) = [this]() { RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 36) = [this]() { RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 60) = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 22) = std::bind(&SwordMan::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 33) = std::bind(&SwordMan::AttackSwingSoundEvent, this);


		//// 오른손 내려베기 3011
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 5) = [this]() { mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(SwordManBaseSpeed); mLSwordScript->SetBlock(true); mLSwordScript->SetAttackDir(0);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 25) = [this]() { RM_STATE(MonsterState_Move);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 30) = [this]() { RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 68) = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 22) = std::bind(&SwordMan::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 26) = std::bind(&SwordMan::AttackSwingSoundEvent, this);

		//// 좌측 에서 우측으로 두번 베기 3006
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 15) = [this]() {mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(SwordManBaseSpeed ); mLSwordScript->SetBlock(true); mLSwordScript->SetAttackDir(0); mRSwordScript->SetBlock(true); mRSwordScript->SetAttackDir(1); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 28) = [this]() { RM_STATE(MonsterState_Move);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 32) = [this]() { RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 75) = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 22) = std::bind(&SwordMan::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 19) = std::bind(&SwordMan::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 34) = std::bind(&SwordMan::AttackSwingSoundEvent, this);

		//// 양손 찌르기 못막는 3007
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 1) = [this]() { mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(SwordManBaseSpeed * (float)3); mRSwordScript->SetBlock(false); mRSwordScript->SetAttackDir(2);  mAttackParams.damage = 30; SetAttackUnGuardable(true); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 26) = [this]() { RM_STATE(MonsterState_Move);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 32) = [this]() { RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 60) = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 22) = [this]() {AttackSwingSoundEvent(); AttackSoundEvent(); };

		//// 양손 x자 베기 제자리 3012
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 5) = [this]() {mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(SwordManBaseSpeed * (float)1.5); mLSwordScript->SetBlock(true); mLSwordScript->SetAttackDir(0);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 26) = [this]() { RM_STATE(MonsterState_Move);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 32) = [this]() { RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 65) = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 22) = std::bind(&SwordMan::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 25) = std::bind(&SwordMan::AttackSwingSoundEvent, this);

		//// 양손 우에서 좌로 베기 3016
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 1) = [this]() { mbMoveForward = true; ADD_STATE(MonsterState_Move); SetSpeed(SwordManBaseSpeed * (float)1.5); mLSwordScript->SetBlock(false); mLSwordScript->SetAttackDir(2);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 22) = [this]() { RM_STATE(MonsterState_Move);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 32) = [this]() { RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 60) = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 23) = std::bind(&SwordMan::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 30) = std::bind(&SwordMan::AttackSwingSoundEvent, this);





		mMeshData->GetAnimationEndEvent(L"SwordAttack_1") = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_2") = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_3") = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_4") = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_5") = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_6") = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_7") = std::bind(&SwordMan::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_8") = std::bind(&SwordMan::AttackEndEvent, this);

		mMeshData->GetAnimationStartEvent(L"SwordAttack_1") = std::bind(&SwordMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_2") = std::bind(&SwordMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_3") = std::bind(&SwordMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_4") = std::bind(&SwordMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_5") = std::bind(&SwordMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_6") = std::bind(&SwordMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_7") = std::bind(&SwordMan::AttackStartEvent, this);
		mMeshData->GetAnimationStartEvent(L"SwordAttack_8") = std::bind(&SwordMan::AttackStartEvent, this);

		//mMeshData->GetAnimationEndEvent(L"Defense") = std::bind(&SwordMan::DefenseEndEvent, this);

		mMeshData->GetAnimationFrameEvent(L"GuardLeft", 18) = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"GuardRight", 18) = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"GuardLeft") = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"GuardRight") = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationStartEvent(L"GuardLeft") = [this]() { ADD_STATE(MonsterState_GuardSuccess); };
		mMeshData->GetAnimationStartEvent(L"GuardRight") = [this]() { ADD_STATE(MonsterState_GuardSuccess); };


		mMeshData->GetAnimationStartEvent(L"ParriedLeft") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationStartEvent(L"ParriedRight") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationStartEvent(L"ParriedBoth") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationFrameEvent(L"ParriedLeft", 20) = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"ParriedRight", 20) = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };

		mMeshData->GetAnimationEndEvent(L"ParriedLeft") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"ParriedRight") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"ParriedBoth") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };


		mMeshData->GetAnimationFrameEvent(L"HitFront", 5) = [this]() {  RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"HitBack", 5) = [this]() {  RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"HitFront", 18) = [this]() { RM_STATE(MonsterState_OnHit);  mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"HitBack", 18) = [this]() { RM_STATE(MonsterState_OnHit); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"HitFront") = [this]() { RM_STATE(MonsterState_OnHit);  mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"HitBack") = [this]() { RM_STATE(MonsterState_OnHit); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationStartEvent(L"HitFront") = [this]() { Resources::Find<AudioClip>(L"swordman_v_hit")->Play();  ADD_STATE(MonsterState_OnHit);mbMoveForward = false;  ADD_STATE(MonsterState_Move); mMoveDir = -mTransform->Forward();  };
		mMeshData->GetAnimationStartEvent(L"HitBack") = [this]() {  Resources::Find<AudioClip>(L"swordman_v_hit")->Play(); ADD_STATE(MonsterState_OnHit); mbMoveForward = false; ADD_STATE(MonsterState_Move); mMoveDir = -mTransform->Forward();  };


		mMeshData->GetAnimationEndEvent(L"GrogyDownParried") = [this]() { RM_STATE(MonsterState_OnHit); RM_STATE(MonsterState_Groggy); /*SetPosture(80); SetDeathBlow(false);*/ };
		mMeshData->GetAnimationStartEvent(L"GrogyDownParried") = [this]() { SetDeathBlow(true); RM_STATE(MonsterState_Guard); };
		mMeshData->GetAnimationFrameEvent(L"GrogyDownParried", 70) = [this]() { SetDeathBlow(false); SetPosture(80); };

		mMeshData->GetAnimationEndEvent(L"DeathBlow1") = [this]() { RM_STATE(MonsterState_OnHit); ADD_STATE(MonsterState_Recognize); SetPosture(0); SetHp(GetMaxHP()); mMonsterUI->UIOn();  mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationStartEvent(L"DeathBlow1") = [this]() { ADD_STATE(MonsterState_OnHit); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 55) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlow1_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 56) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlow1_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 57) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlow1_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 58) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 95) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 96) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 97) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };

		mMeshData->GetAnimationEndEvent(L"DeathBlow1_Death") = [this]() { mMeshData->GetAnimator()->SetStop(true); mState = 0; ADD_STATE(MonsterState_Dead); mCamScript->SetCameraZoomDistance(3.5);  };
		mMeshData->GetAnimationStartEvent(L"DeathBlow1_Death") = [this]() {ADD_STATE(MonsterState_OnHit); SetHp(0); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 3) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 4) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 40) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 41) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 42) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		
		mMeshData->GetAnimationEndEvent(L"DeathBlowAssasinated") = [this]() { RM_STATE(MonsterState_OnHit); ADD_STATE(MonsterState_Recognize); SetPosture(0); SetHp(GetMaxHP()); mMonsterUI->UIOn(); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationStartEvent(L"DeathBlowAssasinated") = [this]() { ADD_STATE(MonsterState_OnHit); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 55) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlowAssasinated_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 54) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlowAssasinated_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 53) = [this]() { if (GetResurrectionCount() <= 0) { mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlowAssasinated_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 58) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 59) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 95) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 96) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated", 97) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		
		mMeshData->GetAnimationEndEvent(L"DeathBlowAssasinated_Death") = [this]() { mMeshData->GetAnimator()->SetStop(true); mState = 0; ADD_STATE(MonsterState_Dead);  mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationStartEvent(L"DeathBlowAssasinated_Death") = [this]() {ADD_STATE(MonsterState_OnHit); SetHp(0); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 3) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 4) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 40) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 41) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlowAssasinated_Death", 42) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };

		
	}

	float SwordMan::EyeSightCheck()
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

	void SwordMan::AttackStartEvent()
	{
		ADD_STATE(MonsterState_Attack);
		mBeforeState |= MonsterState_Attack;

	}

	float SwordMan::GetDistanceToPlayer()
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

	void SwordMan::RotateForwardTo(Vector3 dir)
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

	void SwordMan::DrawSwordEndEvent()
	{
		ADD_STATE(MonsterState_DrawSword);
		mKatanaObjectTr->SetScale(Vector3(1, 1, 1));
		mKatanaHandleObjectTr->SetScale(Vector3(0, 0, 0));
		//ADD_STATE(MonsterState_Guard);
	}
	void SwordMan::DefenseEndEvent()
	{
		RM_STATE(MonsterState_Defense);
	}
	void SwordMan::AttackEndEvent()
	{
		RM_STATE(MonsterState_Attack);
		mBeforeState &= ~MonsterState_Attack;
		ADD_STATE(MonsterState_LookAt);
		mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f);
		mRSwordScript->SetBlock(true);
		mRSwordScript->SetAttackDir(true);
		mLSwordScript->SetBlock(true);
		mLSwordScript->SetAttackDir(true);

		mAttackParams.damage = 10;
		mAttackParams.unGuardable = false;
		mAttackParams.special = SpecialAttack::None;
	}
	void SwordMan::TraceEndEvent()
	{
		RM_STATE(MonsterState_Trace);
	}

	void SwordMan::AttackSoundEvent()
	{
		std::shared_ptr<AudioClip> clip;

		switch (rand() % 4)
		{
		case 0:
			clip = Resources::Find<AudioClip>(L"swordman_v_attack_1");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 1:
			clip = Resources::Find<AudioClip>(L"swordman_v_attack_2");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 2:
			clip = Resources::Find<AudioClip>(L"swordman_v_attack_3");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 3:
			clip = Resources::Find<AudioClip>(L"swordman_v_attack_4");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		}
	}

	void SwordMan::AttackSwingSoundEvent()
	{
		std::shared_ptr<AudioClip> clip;

		switch (rand() % 4)
		{
		case 0:
			clip = Resources::Find<AudioClip>(L"swordman_v_attack_swing_1");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 1:
			clip = Resources::Find<AudioClip>(L"swordman_v_attack_swing_2");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 2:
			clip = Resources::Find<AudioClip>(L"swordman_v_attack_swing_3");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		case 3:
			clip = Resources::Find<AudioClip>(L"swordman_v_attack_swing_4");
			clip->Set3DAttributes(mTransform->GetPosition(), Vector3::Zero);
			clip->Play();
			break;
		}
	}

	void SwordMan::OnCollisionEnter(Collider2D* collider)
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
						SetHp(GetHP() - 10);

						if (!(STATE_HAVE(MonsterState_SuperArmor)))
						{
							ADD_STATE(MonsterState_OnHit);
							RM_STATE(MonsterState_OnHitFront);
						}
						SetPosture(GetPosture() + 10);

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
						SetPosture(GetPosture() + 20);


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

	void SwordMan::OnCollisionStay(Collider2D* collider)
	{
	}
	void SwordMan::OnCollisionExit(Collider2D* collider)
	{
	}
	void SwordMan::AddMonsterState(eMonsterState state)
	{
		ADD_STATE(state);
	}
	void SwordMan::KatanaColliderInit()
	{
		{


			BoneCollider* Lkatana = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile , GetScene());
			Lkatana->SetMeshAndBone(mMeshData, L"R_Katana", this);
			Lkatana->SetBCOwner(this);
			mLSwordScript = Lkatana->AddComponent<SwordManSwordScript>();

			BoneCollider* Rkatana = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile, GetScene());
			Rkatana->SetMeshAndBone(mMeshData, L"L_Katana", this);
			Rkatana->SetBCOwner(this);
			mRSwordScript = Rkatana->AddComponent<SwordManSwordScript>();
			//2729
			Lkatana->SetColliderActiveFrame(L"SwordAttack_1", 43, 45);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_2", 55, 58);
			Rkatana->SetColliderActiveFrame(L"SwordAttack_3", 29, 32);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_4", 27, 29);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_5", 20, 22);
			Rkatana->SetColliderActiveFrame(L"SwordAttack_5", 36, 39);
			Rkatana->SetColliderActiveFrame(L"SwordAttack_6", 28, 30);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_7", 27, 30);
			Lkatana->SetColliderActiveFrame(L"SwordAttack_8", 31, 33);


		}
	}
}