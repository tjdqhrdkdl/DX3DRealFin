#include "yaTenzen.h"
#include "yaMeshData.h"

//test
#include "yaInput.h"
#include "yaCollider2D.h"
#include "yaBoneAnimator.h"
#include "yaBoneCollider.h"
#include "yaActionScript.h"
#include "yaPlayer.h"
#include "yaMonsterCollisionScript.h"
#include "yaTenzenSwordScript.h"

#include "yaNavMesh.h"
#include "yaAudioClip.h"

#define STATE_HAVE(STATE) (mState & STATE) == STATE
#define ADD_STATE(STATE) mState |= STATE
#define RM_STATE(STATE) mState &= ~STATE

#define BEFORE_STATE_HAVE(STATE) (mBeforeState & STATE) == STATE

namespace ya
{
	float eyeSightAngleCos = 0.1f;
	float tenzenWalkSpeed = 71;
	float tenzenBaseSpeed = 200;
	Tenzen::Tenzen()
		: MonsterBase()
		, mAlertTime(10)
	{
	}
	Tenzen::~Tenzen()
	{
	}
	void Tenzen::Initialize()
	{
		MonsterBase::Initialize();

		//안쓰는 애들
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000401.fbx", L"SitIdle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000402.fbx", L"SitToGetUp");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000410.fbx", L"ArmCrossStart");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000411.fbx", L"ArmCrossIdle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000412.fbx", L"ArmCrossEnd");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000700.fbx", L"Surprised");
		//3천번대 공격
		//5천번대 이동ㅁ
		//8천번대 피격
		// 8400~1 공격자세 방어성공
		// 8500~3 방어자세 방어성공
		// 이후 전부 방어 및 무너짐관련
		// 
		//1만번대 사망
		//500000 방어ㅁ자세
		// 501400 방어자세 해제
		// 505000~3 방어자세 이동
		//  505400~3 방어자세 회전
		SetName(L"TenzenObject");
		//mMeshData = MeshData::LoadFromFbx(L"Monster\\Boss_tenzen\\Mesh\\c1020.fbx");
		//
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000000.fbx", L"Idle");
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
		mMeshData = std::make_shared<MeshData>();
		mMeshData->Load(L"Monster\\Boss_tenzen\\Mesh\\c1020.fbx");
		mMeshData->AnimationLoad(L"Monster\\Boss_tenzen\\AnimationData\\tenzen.animationdata");
		MeshObject* object = mMeshData->Instantiate(eLayerType::Monster, GetScene());


		//칼과 손잡이 찾아두기
		std::vector<GameObject*> childObjects = mMeshData->GetChildObjects();
		std::vector<std::vector<std::shared_ptr<Material>>> materials = mMeshData->GetMaterialsVec();
		for (size_t i = 0; i < materials.size(); i++)
		{
			if (materials[i][0]->GetName() == L"#04#")
				mKatanaObjectTr = childObjects[i]->GetComponent<Transform>();
			if (materials[i][0]->GetName() == L"#06#")
				mKatanaHandleObjectTr = childObjects[i]->GetComponent<Transform>();
		}
		if(mKatanaObjectTr)
			mKatanaObjectTr->SetScale(Vector3(0, 0, 0));
		

		//오디오 클립 로드
		Resources::Load<AudioClip>(L"tenzen_v_recognize", L"..\\Resources\\Sound\\tenzen\\Recognize\\c102008500.wav");
		Resources::Load<AudioClip>(L"tenzen_v_hit", L"..\\Resources\\Sound\\tenzen\\Hit\\c102008100.wav");
		Resources::Load<AudioClip>(L"tenzen_v_attack_1", L"..\\Resources\\Sound\\tenzen\\Attack\\c102008701.wav");
		Resources::Load<AudioClip>(L"tenzen_v_attack_2", L"..\\Resources\\Sound\\tenzen\\Attack\\c102008701b.wav");
		Resources::Load<AudioClip>(L"tenzen_v_attack_3", L"..\\Resources\\Sound\\tenzen\\Attack\\c102008701c.wav");
		Resources::Load<AudioClip>(L"tenzen_v_attack_4", L"..\\Resources\\Sound\\tenzen\\Attack\\c102008701d.wav");
		Resources::Load<AudioClip>(L"tenzen_v_attack_swing_1", L"..\\Resources\\Sound\\tenzen\\Attack\\c102006000.wav");
		Resources::Load<AudioClip>(L"tenzen_v_attack_swing_2", L"..\\Resources\\Sound\\tenzen\\Attack\\c102006000b.wav");
		Resources::Load<AudioClip>(L"tenzen_v_attack_swing_3", L"..\\Resources\\Sound\\tenzen\\Attack\\c102006000c.wav");
		Resources::Load<AudioClip>(L"tenzen_v_attack_swing_4", L"..\\Resources\\Sound\\tenzen\\Attack\\c102006000d.wav");
		


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
		BoneCollider* katana =  object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile, GetScene());
		katana->SetMeshAndBone(mMeshData, L"R_Katana_long", this);
		mSwordScript = katana->AddComponent<TenzenSwordScript>();
		//애니메이션 별로 오프셋과 발동 프레임 정해주기. 
		//발동 프레임은 기본이 false로 발동하고싶으면 조정해주면 된다.
		katana->SetColliderActiveFrame(L"SwordAttack_1", 17, 24);
		katana->SetColliderActiveFrame(L"SwordAttack_2", 19, 24);
		katana->SetColliderActiveFrame(L"SwordAttack_3", 20, 30);
		katana->SetColliderActiveFrame(L"SwordAttack_4", 32, 39);
		katana->SetColliderActiveFrame(L"SwordAttack_5", 24, 27);
		katana->SetColliderActiveFrame(L"SwordAttack_6", 22, 27);
		katana->SetColliderActiveFrame(L"SwordAttack_6", 49, 54);
		katana->SetColliderActiveFrame(L"SwordAttack_7", 8, 11);
		katana->SetColliderActiveFrame(L"SwordAttack_8", 7, 11);
		katana->SetColliderActiveFrame(L"SwordAttack_8", 20, 23);
		katana->SetColliderActiveFrame(L"SwordAttack_8", 37, 40);
		katana->SetColliderActiveFrame(L"SwordAttack_9", 7, 11);

		//칼의 크기 정해주기
		katana->SetScale(Vector3(2.f, 0.3f, 0.3f));
		mKatanaCollider = katana;

		//피격 콜라이더
		mCollider = AddComponent<Collider2D>();
		mCollider->SetType(eColliderType::Box);
		mCollider->SetSize(Vector3(0.9f, 2.f, 0.5f));
		mCollider->SetCenter(Vector3(0.f, 0.f, 0.f));

		//리지드 바디 , 액션 스크립트
		AddComponent<Rigidbody>()->SetFriction(180);
		AddComponent<MonsterCollisionScript>();
		mActionScript = AddComponent<ActionScript>();

		//애니메이션 이벤트
		SetAnimationEvent();


		//동일 애니메이션 반복시 보간을 하지 않고 싶은 경우. (기본은 보간을 하도록 되어있음.)
		BoneAnimator* animator = mMeshData->GetAnimator();
		animator->SetAnimationSelfChange(L"RunWithSword", false);
		//애니메이션 끝부분 이상한 것 잘라내는 시간
		animator->SetAnimationTailTime(0.1f);



		//SetPlayerObject(dynamic_cast<Player*>(SceneManager::GetActiveScene()->GetPlayer()));
		SetPlayerObject(dynamic_cast<Player*>(GetScene()->GetPlayer()));
		
		//몬스터 스테이트
		SetSpeed(tenzenBaseSpeed);


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
		SetResurrectionCount(2);
		SetResurrectionCountMax(2);

		SetOriginState(GetState());
		SetOriginPosition(mTransform->GetPosition());
	}
	void Tenzen::Update()
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
			Guard();
			OnHit();
			Groggy();

			Move();
			LookAtPlayer();
			mCollider->Active(true);

		}

		else
		{
			mCollider->Active(false);
			mMonsterUI->UIOff();
			mBossUI->UIOff();
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
		if (Input::GetKeyDown(eKeyCode::B))
			ParryEffectOn();
		mBeforeState = mState;
	}
	void Tenzen::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void Tenzen::Render()
	{
		MonsterBase::Render();
	}
	void Tenzen::DeathBlow()
	{
		//recognize 상태가 아니면 암살이다.

		//recognize 상태이면 인살이다.
		ADD_STATE(MonsterState_OnHit);
		RM_STATE(MonsterState_Groggy);
		SetDeathBlow(false);
		mAnimationName = L"DeathBlow1";

		GetComponent<Transform>()->SetPosition(GetPlayerPos() + mPlayerObject->GetComponent<Transform>()->Forward()
			* (mPlayerObject->GetComponent<Transform>()->GetFinalScale().z / 2
				+ GetComponent<Transform>()->GetFinalScale().z / 2 +(float) 0.5)
			- mPlayerObject->GetComponent<Transform>()->Right() * (float)0.8);
		SetPosture(0);
		SetResurrectionCount(GetResurrectionCount() - 1);
		RM_STATE(MonsterState_Move);
		RM_STATE(MonsterState_Attack);
		RM_STATE(MonsterState_Defense);
		RM_STATE(MonsterState_Trace);
		ADD_STATE(MonsterState_LookAt);
		RM_STATE(MonsterState_Idle);
		mCamScript->SetLockOnFree();

	}
	void Tenzen::Reset()
	{
		RM_STATE(MonsterState_DrawSword);
		mKatanaObjectTr->SetScale(Vector3(0, 0, 0));
		mKatanaHandleObjectTr->SetScale(Vector3(1, 1, 1));
		MonsterBase::Reset();
	}
	void Tenzen::Idle()
	{
		if (STATE_HAVE(MonsterState_Idle))
		{
			mAnimationName = L"Idle";
			RM_STATE(MonsterState_LookAt);
		}
	}
	void Tenzen::Alert()
	{
		if (mAnimationName != L"DeathBlow1" && mAnimationName != L"DeathBlow1_Death"
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
				if (cosTheta > eyeSightAngleCos)
				{
					float dist = GetDistanceToPlayer();
					float alert = GetAlertnessCount();


					if (mPlayerObject->IsStateFlag(ePlayerState::Crouch))
					{
						if (cosTheta > eyeSightAngleCos * 4)
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
				mAnimationName = L"WalkNoSword";
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
					SetSpeed(tenzenWalkSpeed);
				}

				float dist = GetDistanceToPlayer();
				mAlertTimeChecker += Time::DeltaTime();

				if (mAlertTimeChecker > mAlertTime)
				{
					RM_STATE(MonsterState_Alert);
					RM_STATE(MonsterState_Move);
					mbNavOn = false;

					SetSpeed(tenzenWalkSpeed);

					ADD_STATE(MonsterState_Idle);
					mAlertTimeChecker = 0;
					SetAlertnessCount(0);
				}
				else if (cosTheta > eyeSightAngleCos && dist < 12)
				{
					mActionScript->Velocity(10);
					ADD_STATE(MonsterState_Recognize);
					Resources::Find<AudioClip>(L"recognize_sound")->Play();
					Resources::Find<AudioClip>(L"tenzen_v_recognize")->Play();
					mbNavOn = false;
					SetAlertnessCount(0);
					Vector3 dir = mPlayerObject->GetComponent<Transform>()->GetPosition() - mTransform->GetPosition();
					dir.Normalize();
					RotateForwardTo(dir);
				}
			}
		}
	}
	void Tenzen::Recognize()
	{
		if (mPlayerObject->IsStateFlag(ePlayerState::Death))
		{
			SetRecognize(false);
			mState = 0;
			ADD_STATE(MonsterState_Idle);
			mKatanaObjectTr->SetScale(Vector3(0, 0, 0));
			mKatanaHandleObjectTr->SetScale(Vector3(1, 1, 1));
			return;
		}

		if (STATE_HAVE(MonsterState_Recognize))
		{
			std::shared_ptr<AudioClip> bgmTenzen = Resources::Find<AudioClip>(L"bgm-boss");
			if (!bgmTenzen->isPlaying())
			{
				bgmTenzen->Play();
				Resources::Find<AudioClip>(L"bgm-usual")->Stop();
			}
			mBossUI->UIOn();
			mMonsterUI->UIOff();
			RM_STATE(MonsterState_Idle);
			SetRecognize(true);
			if (!(STATE_HAVE(MonsterState_DrawSword)))
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
					float traceDist = 10;
					if (Vector3::Distance(pos, playerPos) > traceDist)
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
		{
			std::shared_ptr<AudioClip> bgmTenzen = Resources::Find<AudioClip>(L"bgm-boss");
			if (bgmTenzen->isPlaying())
			{
				bgmTenzen->Stop();
				Resources::Find<AudioClip>(L"bgm-usual")->Play();
			}
			SetRecognize(false);
			mBossUI->UIOff();
		}

		
	}
	void Tenzen::Attack()
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
				mAnimationName = L"SwordAttack_9";
				break;
			}

		}

	}
	void Tenzen::Defense()
	{

	}
	void Tenzen::Trace()
	{
		if (STATE_HAVE(MonsterState_Trace))
		{
			mAnimationName = L"RunWithSword";
			
			
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

		else if (!mbNavOn)
		{
			GetComponent<NavMesh>()->SetTraceOn(false);
		}
	}

	void Tenzen::Guard()
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

	void Tenzen::OnHit()
	{
		if (STATE_HAVE(MonsterState_OnHit))
		{
			if (mAnimationName == L"DeathBlow1" || mAnimationName == L"DeathBlow1_Death")
				;
			else if (STATE_HAVE(MonsterState_OnHitFront))
				mAnimationName = L"Hit1";
			else
				mAnimationName = L"Hit2";
		}
	}

	void Tenzen::Groggy()
	{
		if (!(STATE_HAVE(MonsterState_Groggy)) && GetPosture() >=100
			&& mAnimationName != L"DeathBlow1" && mAnimationName != L"DeathBlow1_Death")
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
			mAnimationName = L"GrogyDownFront";
		}
	}

	void Tenzen::Move()
	{
		if (STATE_HAVE(MonsterState_Move))
		{
			mActionScript->Move(mMoveDir, GetSpeed());
		}
	}

	void Tenzen::LookAtPlayer()
	{
		if (STATE_HAVE(MonsterState_LookAt))
		{
			RotateForwardTo(GetMonster2PlayerNormalize());
		}
	}

	void Tenzen::SetAnimationEvent()
	{
		mSwordScript->SetAttackLeft(false);
		mActionScript->SetJumpTime(0.2f);
		//애니메이션 이벤트 추가
		//complete event는 애니메이션이 루프를 돌때, 한번 애니메이션을 완료할때 생기는 이벤트
		//End Event 는 애니메이션이 종료 또는 Complete될 때 생기는 이벤트
		//Start Event 는 애니메이션이 다른 애니메이션으로 전환되며 새로운 애니메이션이 시작할 때 생기는 이벤트
		mMeshData->GetAnimationFrameEvent(L"DrawSword", 20) = [this]() {
		mKatanaObjectTr->SetScale(Vector3(1, 1, 1));
		mKatanaHandleObjectTr->SetScale(Vector3(0, 0, 0));
		};

		mMeshData->GetAnimationEndEvent(L"DrawSword") = std::bind(&Tenzen::DrawSwordEndEvent, this);
		//
		// 3000
		// 3001
		// 3003
		// 3004
		// 3005
		// 3006
		// 
		// 
		// 3051
		// 3054
		// 3056
		// 
		// 칼을 우상단에서 우하단으로 크게 휘두르고 제자리로.
		//mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 1) = [this]() {ADD_STATE(MonsterState_SuperArmor); };

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 12) = [this]() { mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move);  SetSpeed(tenzenBaseSpeed * 4); mSwordScript->SetBlock(true); mSwordScript->SetAttackLeft(false); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 13) = std::bind(&Tenzen::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 14) = [this]() { RM_STATE(MonsterState_Move); RM_STATE(MonsterState_LookAt); SetSpeed(tenzenBaseSpeed); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 19) = std::bind(&Tenzen::AttackSwingSoundEvent, this);
		// 칼을 좌하단에서 우상단으로. 한걸음 내딛으며.
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 1) = [this]() {ADD_STATE(MonsterState_SuperArmor); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 11) = std::bind(&Tenzen::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 17) = std::bind(&Tenzen::AttackSwingSoundEvent, this);

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 12) = [this]() { mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move);};
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_2", 18) = [this]() { RM_STATE(MonsterState_Move);  RM_STATE(MonsterState_LookAt); };
		// 전진 점프 하며, 칼을 우상단에서 좌하단으로. 한걸음 내딛으며.
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 1) = [this]() {ADD_STATE(MonsterState_SuperArmor); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 2) = std::bind(&Tenzen::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 25) = std::bind(&Tenzen::AttackSwingSoundEvent, this);

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 3) = [this]() { mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move); SetSpeed(tenzenBaseSpeed); mActionScript->Jump(110); GetComponent<Rigidbody>()->SetJumping(true); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_3", 14) = [this]() { RM_STATE(MonsterState_Move); RM_STATE(MonsterState_LookAt); };

		// 못막는 공격, 하단 베기
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 1) = std::bind(&Tenzen::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 23) = std::bind(&Tenzen::AttackSwingSoundEvent, this);

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 10) = [this]() { SetAttackUnGuardable(true); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 27) = [this]() { mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_4", 32) = [this]() { RM_STATE(MonsterState_Move);  RM_STATE(MonsterState_LookAt); };

		// 못막는 공격, 찌르기. 전진
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 1) = [this]() {ADD_STATE(MonsterState_SuperArmor); };

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 10) = std::bind(&Tenzen::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 21) = std::bind(&Tenzen::AttackSwingSoundEvent, this);

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 5) = [this]() { SetAttackUnGuardable(true); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 18) = [this]() { mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move); SetSpeed(tenzenBaseSpeed * 4); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 20) = [this]() { RM_STATE(MonsterState_Move); SetSpeed(tenzenBaseSpeed);  RM_STATE(MonsterState_LookAt);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 52) = [this]() { mMoveDir = -mTransform->Forward(); ADD_STATE(MonsterState_Move); SetSpeed(tenzenBaseSpeed * 0.5f);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_5", 60) = [this]() { RM_STATE(MonsterState_Move); SetSpeed(tenzenBaseSpeed);  };

		// 양옆으로 휘두르기, 2회 연속공격, 2회전진
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 1) = [this]() {ADD_STATE(MonsterState_SuperArmor); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 3) = [this]() { mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move); RM_STATE(MonsterState_LookAt); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 12) = [this]() { RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 40) = [this]() { ADD_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 46) = [this]() { RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 10) = std::bind(&Tenzen::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 33) = std::bind(&Tenzen::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 22) = std::bind(&Tenzen::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_6", 48) = std::bind(&Tenzen::AttackSwingSoundEvent, this);


		// 콤보 공격 5베기
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 1) = [this]() { mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move); ADD_STATE(MonsterState_SuperArmor);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 4) = std::bind(&Tenzen::AttackSwingSoundEvent, this);

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 7) = [this]() { RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_7", 12) = [this]() {RM_STATE(MonsterState_LookAt); };

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 1) = [this]() { mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move); ADD_STATE(MonsterState_SuperArmor); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 5) = std::bind(&Tenzen::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 31) = std::bind(&Tenzen::AttackSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 3) = std::bind(&Tenzen::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 16) = std::bind(&Tenzen::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 34) = std::bind(&Tenzen::AttackSwingSoundEvent, this);

		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 7) = [this]() { RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 17) = [this]() {mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move); SetSpeed(tenzenBaseSpeed); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 20) = [this]() {RM_STATE(MonsterState_Move); SetSpeed(tenzenBaseSpeed); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 24) = [this]() {mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 33) = [this]() {RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_8", 40) = [this]() {mAnimationName = L"SwordAttack_7"; };
		mMeshData->GetAnimationEndEvent(L"SwordAttack_8") = [this]() {if (mMeshData->GetAnimator()->GetCurrentFrameIdx() <= 38) AttackEndEvent(); };


		mMeshData->GetAnimationFrameEvent(L"SwordAttack_9", 1) = [this]() { mMoveDir = mTransform->Forward(); ADD_STATE(MonsterState_Move);  };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_9", 6) = std::bind(&Tenzen::AttackSwingSoundEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_9", 7) = [this]() { RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_9", 25) = [this]() {mAnimationName = L"SwordAttack_8"; };
		mMeshData->GetAnimationEndEvent(L"SwordAttack_9") = [this]() {if (mMeshData->GetAnimator()->GetCurrentFrameIdx() <= 23) AttackEndEvent(); };




		mMeshData->GetAnimationEndEvent(L"SwordAttack_1") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_2") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_3") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_4") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_5") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_6") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_7") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"Defense") = std::bind(&Tenzen::DefenseEndEvent, this);

		mMeshData->GetAnimationFrameEvent(L"GuardLeft", 27) = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"GuardRight", 27) = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"GuardLeft") = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"GuardRight") = [this]() { RM_STATE(MonsterState_GuardSuccess); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationStartEvent(L"GuardLeft") = [this]() { ADD_STATE(MonsterState_GuardSuccess); };
		mMeshData->GetAnimationStartEvent(L"GuardRight") = [this]() { ADD_STATE(MonsterState_GuardSuccess); };



		mMeshData->GetAnimationStartEvent(L"BlockedLeft") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationStartEvent(L"BlockedRight") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationFrameEvent(L"BlockedLeft", 20) = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"BlockedRight", 20) = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"BlockedLeft") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"BlockedRight") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };

		mMeshData->GetAnimationStartEvent(L"ParriedLeft") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationStartEvent(L"ParriedRight") = [this]() { ADD_STATE(MonsterState_AttackBlocked); };
		mMeshData->GetAnimationFrameEvent(L"ParriedLeft", 38) = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"ParriedRight", 38) = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"ParriedLeft") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"ParriedRight") = [this]() { RM_STATE(MonsterState_AttackBlocked); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };



		mMeshData->GetAnimationFrameEvent(L"Hit1", 5) = [this]() {  RM_STATE(MonsterState_Move); };
		mMeshData->GetAnimationFrameEvent(L"Hit2", 5) = [this]() {  RM_STATE(MonsterState_Move); };

		mMeshData->GetAnimationFrameEvent(L"Hit1", 18) = [this]() { RM_STATE(MonsterState_OnHit);  mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationFrameEvent(L"Hit2", 18) = [this]() { RM_STATE(MonsterState_OnHit);  mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"Hit1") = [this]() { RM_STATE(MonsterState_OnHit); RM_STATE(MonsterState_Move); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
		mMeshData->GetAnimationEndEvent(L"Hit2") = [this]() { RM_STATE(MonsterState_OnHit); RM_STATE(MonsterState_Move); mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f); };
					
		mMeshData->GetAnimationStartEvent(L"Hit1") = [this]() { Resources::Find<AudioClip>(L"tenzen_v_hit")->Play();  ADD_STATE(MonsterState_OnHit); ADD_STATE(MonsterState_Move); mMoveDir = -mTransform->Forward(); };
		mMeshData->GetAnimationStartEvent(L"Hit2") = [this]() { Resources::Find<AudioClip>(L"tenzen_v_hit")->Play(); ADD_STATE(MonsterState_OnHit); ADD_STATE(MonsterState_Move); mMoveDir = -mTransform->Forward();  };


		mMeshData->GetAnimationEndEvent(L"GrogyDownFront") = [this]() { RM_STATE(MonsterState_OnHit); RM_STATE(MonsterState_Groggy); SetPosture(80); SetDeathBlow(false); };
		mMeshData->GetAnimationStartEvent(L"GrogyDownFront") = [this]() { SetDeathBlow(true); RM_STATE(MonsterState_Guard); };
		mMeshData->GetAnimationFrameEvent(L"GrogyDownFront", 70) = [this]() { SetDeathBlow(false); SetPosture(80); };

		mMeshData->GetAnimationEndEvent(L"DeathBlow1") = [this]() { RM_STATE(MonsterState_OnHit); ADD_STATE(MonsterState_Recognize); SetPosture(0); SetHp(GetMaxHP()); mMonsterUI->UIOn(); };
		mMeshData->GetAnimationStartEvent(L"DeathBlow1") = [this]() { ADD_STATE(MonsterState_OnHit); mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + mPlayerObject->GetComponent<Transform>()->Right() * 1.7f); mCamScript->SetCameraZoomDistance(1.5); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 55) = [this]() { if (GetResurrectionCount() <= 0) {mMeshData->GetAnimator()->SetAnimationChangeTime(0.01f); mAnimationName = L"DeathBlow1_Death"; } };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 58) = [this]() { mCamScript->SetDestinationDir(mPlayerObject->GetComponent<Transform>()->Right() * 3.0f); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1", 95) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward())+Vector3(0,0.5,0)); mCamScript->SetCameraZoomDistance(3.5); };

		mMeshData->GetAnimationEndEvent(L"DeathBlow1_Death") = [this]() { mMeshData->GetAnimator()->SetStop(true); mState = 0; ADD_STATE(MonsterState_Dead); };
		mMeshData->GetAnimationStartEvent(L"DeathBlow1_Death") = [this]() {ADD_STATE(MonsterState_OnHit); SetHp(0); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 3) = [this]() { mCamScript->SetDestinationDir( mPlayerObject->GetComponent<Transform>()->Right() * 3.0f); mCamScript->SetCameraZoomDistance(1); };
		mMeshData->GetAnimationFrameEvent(L"DeathBlow1_Death", 40) = [this]() { mCamScript->SetDestinationDir(-(mPlayerObject->GetComponent<Transform>()->Forward()) + Vector3(0, 0.5, 0)); mCamScript->SetCameraZoomDistance(3.5); };

	}

	float Tenzen::EyeSightCheck()
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

	float Tenzen::GetDistanceToPlayer()
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

	void Tenzen::RotateForwardTo(Vector3 dir)
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

	void Tenzen::DrawSwordEndEvent()
	{
		ADD_STATE(MonsterState_DrawSword);
		mKatanaObjectTr->SetScale(Vector3(1, 1, 1));
		mKatanaHandleObjectTr->SetScale(Vector3(0, 0, 0));
		//ADD_STATE(MonsterState_Guard);
	}
	void Tenzen::DefenseEndEvent()
	{
		RM_STATE(MonsterState_Defense);
	}
	void Tenzen::AttackEndEvent()
	{
		RM_STATE(MonsterState_Attack);
		RM_STATE(MonsterState_SuperArmor);
		mBeforeState &= ~MonsterState_Attack;
		ADD_STATE(MonsterState_LookAt);
		mMeshData->GetAnimator()->SetAnimationChangeTime(0.2f);
		mSwordScript->SetBlock(true);
		mSwordScript->SetAttackLeft(true);
		SetAttackUnGuardable(false);
		mAttackParams.damage = 10;

	}
	void Tenzen::TraceEndEvent()
	{
		RM_STATE(MonsterState_Trace);
	}

	void Tenzen::AttackSoundEvent()
	{
		std::shared_ptr<AudioClip> clip;
		switch (rand()%4)
		{
		case 0:
			clip = Resources::Find<AudioClip>(L"tenzen_v_attack_1");
			clip->Set3DAttributes(mTransform->GetPosition(), GetComponent<Rigidbody>()->GetVelocity());
			clip->Play();
			break;
		case 1:
			clip = Resources::Find<AudioClip>(L"tenzen_v_attack_2");
			clip->Set3DAttributes(mTransform->GetPosition(), GetComponent<Rigidbody>()->GetVelocity());
			clip->Play();
			break;
		case 2:
			clip = Resources::Find<AudioClip>(L"tenzen_v_attack_3");
			clip->Set3DAttributes(mTransform->GetPosition(), GetComponent<Rigidbody>()->GetVelocity());
			clip->Play();
			break;
		case 3:
			clip = Resources::Find<AudioClip>(L"tenzen_v_attack_4");
			clip->Set3DAttributes(mTransform->GetPosition(), GetComponent<Rigidbody>()->GetVelocity());
			clip->Play();
			break;
		}
	}

	void Tenzen::AttackSwingSoundEvent()
	{
		std::shared_ptr<AudioClip> clip;

		switch (rand() % 4)
		{
		case 0:
			clip = Resources::Find<AudioClip>(L"tenzen_v_attack_swing_1");
			clip->Set3DAttributes(mTransform->GetPosition(), GetComponent<Rigidbody>()->GetVelocity());
			clip->Play();
			break;										
		case 1:											
			clip = Resources::Find<AudioClip>(L"tenzen_v_attack_swing_2");
			clip->Set3DAttributes(mTransform->GetPosition(), GetComponent<Rigidbody>()->GetVelocity());
			clip->Play();
			break;										 
		case 2:											 
			clip = Resources::Find<AudioClip>(L"tenzen_v_attack_swing_3");
			clip->Set3DAttributes(mTransform->GetPosition(), GetComponent<Rigidbody>()->GetVelocity());
			clip->Play();
			break;										
		case 3:											
			clip = Resources::Find<AudioClip>(L"tenzen_v_attack_swing_4");
			clip->Set3DAttributes(mTransform->GetPosition(), GetComponent<Rigidbody>()->GetVelocity());
			clip->Play();
			break;
		}
	}

	void Tenzen::OnCollisionEnter(Collider2D* collider)
	{
		GameObject* colObj = collider->GetOwner();
		eLayerType layer = colObj->GetLayerType();
		if (layer == eLayerType::PlayerProjectile)
		{
			BoneCollider* boneColObj = dynamic_cast<BoneCollider*>(colObj);
			if (boneColObj!= nullptr 
				&& false == boneColObj->CheckHitObjects(this)
				&& mAnimationName != L"DeathBlow1"
				&& mAnimationName != L"DeathBlow1_Death")
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
						SetPosture(GetPosture() + 5);

					}


					RM_STATE(MonsterState_Move);
					RM_STATE(MonsterState_Attack);
					RM_STATE(MonsterState_Defense);
					RM_STATE(MonsterState_Trace);

					mbAnimReset = true;
					if (GetHP() == 0)
						SetPosture(GetPostureMax());
					else
						SetPosture(GetPosture() + 5);
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

	void Tenzen::OnCollisionStay(Collider2D* collider)
	{
	}
	void Tenzen::OnCollisionExit(Collider2D* collider)
	{
	}
	void Tenzen::AddMonsterState(eMonsterState state)
	{
		ADD_STATE(state);
	}
}