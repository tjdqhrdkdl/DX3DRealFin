#include "yaSpearman.h"
#include "yaSpearman_Sting.h"
#include "yaMonsterScript.h"

#include "yaInput.h"

namespace ya
{

	Spearman::Spearman()
	{

	}

	Spearman::~Spearman()
	{
		
	}

	void Spearman::Initialize()
	{

		GetComponent<Transform>()->SetLocalPosition(Vector3(5.0f, 0.0f, 15.0f));
		//GetComponent<Transform>()->SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));
		GetComponent<Transform>()->SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
		
		SetName(L"Spearman");

		Collider3D* spearmancol = AddComponent <Collider3D>();
		spearmancol->SetType(eColliderType::Box);
		spearmancol->SetSize(Vector3(1.0, 2.2f, 1.0f));

		AddComponent<MonsterScript>();
		AddComponent<Rigidbody>();
		mActionScript = AddComponent<ActionScript>();

		//SetPlayerObject(SceneManager::GetActiveScene()->GetPlayer());
		SetPlayerObject(GetScene()->GetPlayer());

		//BoneAnimator* animator = mMeshData->GetAnimator();

		////동일 애니메이션 반복시 보간을 하지 않고 싶은 경우. (기본은 보간을 하도록 되어있음.)
		//animator->SetAnimationSelfChange(L"SwordAttack_2", false);

		////애니메이션 끝부분 이상한 것 잘라내는 시간
		//animator->SetAnimationTailTime(0.1f);

		Transform* tr = GetComponent<Transform>();



		CreateDeathBlowMark();
	
		SetMonsterHpBarOffSetOffSet(Vector3(0.0f, 3.0f, 0.0f));
		SetDeathBlowMarkOffSet(Vector3(0.0f, 0.5f, 0.0f));
		mMeshData = std::make_shared<MeshData>();

		mMeshData->Load(L"Monster\\SwordMan\\MeshData\\c1700_SwordMan.meshdata");

		mMeshData->AnimationLoad(L"Monster\\SwordMan\\AnimationData\\SwordManAnimation_1.animationdata");

		//mMeshData = MeshData::LoadFromFbx(L"Monster\\SwordMan\\Mesh\\c1700_SwordMan.fbx");


		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_000401.fbx", L"a000_000000");
		mMeshObject = mMeshData->Instantiate(eLayerType::Monster, GetScene());

		Transform* meshobjtr = mMeshObject->GetComponent<Transform>();
		meshobjtr->SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));
		meshobjtr->SetLocalRotation(Vector3(180.f, 0.0f, 0.0f));
		meshobjtr->SetRotationOffset(Vector3(0.0f, 1.0f, 0.0f));
		meshobjtr->SetParent(GetComponent<Transform>());

		KatanaColliderInit();

		Animation_Event();


		mMeshData->Play(L"SwordMan_Idle_Stand");

		CreateMonsterState();
		SetSituation(enums::eSituation::None, true);
		
		mAttackRange = 8.0f;
		mTime = 0.f;

		//몬스터 할일 있나


		MonsterBase::Initialize();

	}


	void Spearman::FixedUpdate()
	{

		Transform* tr = GetComponent<Transform>();
		Rigidbody* rigi = GetComponent<Rigidbody>();
		Vec3 rot = tr->GetLocalRotation();

		Vec3 playerPos = GetPlayerPos();
		Vec3 monsterPos = GetComponent<Transform>()->GetLocalPosition();

		Player* player = (Player*)GetPlayerObject();
		


		//if (Input::GetKey(eKeyCode::U))
		//{
		//	AddHp(-Time::DeltaTime() * 5.0f);
		//}
		//if (Input::GetKey(eKeyCode::I))
		//{
		//	AddHp(Time::DeltaTime() * 5.0f);
		//}
		//if (Input::GetKeyDown(eKeyCode::O))
		//{			
		//	AddHp(-2.0f);
		//}
		//if (Input::GetKeyDown(eKeyCode::P))
		//{
		//	AddHp(2.0f);
		//}

		switch (GetSituation())
		{
			//몬스터 처음 상태로 인식을 한번이라도 하면 None상태는 더 안함
		case ya::enums::eSituation::None:
		{
			//플레이어가 가까운 거리에 있으면서 뒤에 있을경우 암살 가능 상태로 변경
			if (NavigationPlayer(2.5f) && !IsPlayerFront())
			{
				SetDeathBlow(true);
			}
			else
			{
				SetDeathBlow(false);
			}

			//플레이어가 시야각안에 있다.
			if (IsPlayerFieldview(45.0f, 135.0f))
			{
				//플레이어가 15범위 안에 들어있을 경우
				if (NavigationPlayer(30.0f))
				{
					//칼을 뽑는 모션후 행동 방향 3가지
					//1. 공격모드로 변경후 다가가서 공격
					//2. 경계하며 4방향으로 랜덤한 위치로 이동 최대 2번 이후는 공격 한다.
					//3. 거리가 약간 띄워져 있으면 달려들어 공격하기.
					//OnceAniamtion(L"SwordMan_Boundary_Start1");
				}
			}
			//플레이어가 시야각에서 벗어나 있는 경우?
			//이건 일단 보류
			else
			{
				if (IsPlayerFront())
				{
					if (!NavigationPlayer(13.0f))
					{
						//OnceAniamtion(L"SwordMan_Boundary_Start2");
					}
				}
			}
		}
		break;
		case ya::enums::eSituation::Idle:
		{
			//공격 or 방어등등 행동이 끝나고 일단 거쳐오는 상태로 딱히 하는일은 없고
			//기본 칼모션을 하는게 거의 끝임.. 
			//일단은 1초뒤에 배틀 모드로 변경
			OnceAniamtion(L"SwordMan_Boundary_Step1");
			mTime += Time::DeltaTime();
			if (mTime >= 1.0f)
			{
				TurnToPlayer();

				SetSituation(enums::eSituation::Battle, true);
				mTime = 0.f;

			}

		}
		break;
		case ya::enums::eSituation::Boundary:
		{
			//이건 아직은 보류 몬스터 좀 더 찍어내면 그때 할예정
			//
			
			if (IsPlayerFront())
			{
				if (IsPlayerFieldview(45.0f, 135.0f))
				{

				}
				else
				{


				}

			}
			else
			{
				if (NavigationPlayer(4.0f))
				{
					SetSituation(enums::eSituation::Battle, true);
				}
			}


		}
		break;
		case ya::enums::eSituation::Chase:
		{
			//경계하며 4방향으로 이동, 연속으로는 최대 2번까지 이동을하고 3번째 들어오면
			//Attack 상태로 변경
			mTime += Time::DeltaTime();

			//Forward
			if (mRandom == 1)
			{
				OnceAniamtion(L"SwordMan_1Default_Defense_Forward");
				if (mTime >= 0.3f)
					mActionScript->Move((tr->Forward()), 70.f);
			}
			//Back
			if (mRandom == 2)
			{
				OnceAniamtion(L"SwordMan_1Default_Defense_Back");
				if (mTime >= 0.3f)
					mActionScript->Move(-(tr->Forward()), 70.f);
			}
			//Left
			if (mRandom == 3)
			{
				OnceAniamtion(L"SwordMan_1Default_Defense_Right");
				if (mTime >= 0.3f)
					mActionScript->Move((tr->Right()), 70.f);
			}
			//Right
			if (mRandom == 4)
			{
				OnceAniamtion(L"SwordMan_1Default_Defense_Left");
				if (mTime >= 0.3f)
					mActionScript->Move(-(tr->Right()), 70.f);
			}

			if (mRandomCount >= 3)
			{
				TurnToPlayer();
				SetSituation(enums::eSituation::Run, true);
				mRandom = RandomNumber(1, 3);
				mRandomCount = 0;
			}

		}
		break;
		case ya::enums::eSituation::Battle:
		{
			//전투 중임 여기서는 플레이어가 어택 상태인지 디펜스 상태인지 확인
			//만약 플레이어가 1초동안 반응이 없으면 물러나거나 Attack모드로 
			//플레이어가 일정
			//랜덤으로 공격 or 경계 나뉘는데 2번이상 경계 안함
			mTime += Time::DeltaTime();
			if (mTime >= 1.0f)
			{
				if(!NavigationPlayer(9.0f) && IsPlayerFront() && NavigationPlayer(11.0f))
				{
					SetOnceAnimation(true);
					TurnToPlayer();
					OnceAniamtion(L"SwordMan_Start_Attack2");
					mRandom = 1;
					SetSituation(enums::eSituation::DashAttack, false);
				}
				else
				{
					mRandom = RandomNumber(1, 2);

					if (mRandom == 1)
					{
						TurnToPlayer();
						SetSituation(enums::eSituation::Run, true);
						mRandom = RandomNumber(1, 3);
					}

					if (mRandom == 2)
					{
						if (NavigationPlayer(4.0f))
						{
							mRandom = RandomNumber(2, 4);
						}
						else
							mRandom = RandomNumber(1, 4);
						SetSituation(enums::eSituation::Chase, true);
						++mRandomCount;
					}
				}
				mSpearmanAction = true;
				mTime = 0.f;
			}
			else
			{
				if(mSpearmanAction)
				{
					if (player->IsStateFlag(ePlayerState::Attack))
					{
						int ran = RandomNumber(1, 2);
						TurnToPlayer();
						//몬스터가 몬스터 인식 + 대기중에 플레이어가 공격시 일단은 막기로
						//추후 다른 옵션을 추가 예정

						if (ran == 1)
							SetSituation(enums::eSituation::Defense, true);

						mSpearmanAction = false;
						mTime = 0.f;
					}
					if (player->IsStateFlag(ePlayerState::Block))
					{
						//몬스터가 대기중 플레이어가 막기하고 있으면 
						//방불공격을 한다??
						int ran = RandomNumber(1, 2);

						if (ran == 1)
						{
							mRandom = 4;
							TurnToPlayer();
							OnceAniamtion(L"SwordMan_Running");
							SetSituation(enums::eSituation::Attack, true);
						}
						mSpearmanAction = false;
						mTime = 0.f;
					}
				}
			}

		}
		break;
		case ya::enums::eSituation::Defense:
		{
			mTime += Time::DeltaTime();
			OnceAniamtion(L"SwordMan_1Default_Defense1");
			SetDefense(true);

			if (mTime < 0.8f)
			{
				mbParrying = true;
			}
			else
			{
				mbParrying = false;
			}


		}
		break;
		case ya::enums::eSituation::Attack:
		{
			//어택 상태일때 
			if (mRandom == 1)
			{
				OnceAniamtion(L"SwordMan_1Default_Attack1");
			}

			if (mRandom == 2)
			{
				OnceAniamtion(L"SwordMan_2Default_Attack1");
			}

			if (mRandom == 3)
			{
				OnceAniamtion(L"SwordMan_3Default_Attack1");
			}

			if (mRandom == 4)
			{
				if (WalkToPlayer(3.0f, 70.0f))
				{
					mRandom = 2;
					TurnToPlayer();
					SetSituation(eSituation::DashAttack, true);
				}
			}

			//if (!NavigationPlayer(8.0f))
			//{
			//	Idle_Stand();
			//}


		}
		break;
		case ya::enums::eSituation::DashAttack:
		{
			if (mRandom == 1)
			{
				BoneAnimator* animator = mMeshData->GetAnimator();
				if (animator->GetCurrentFrameIdx() < 53 && !(NavigationPlayer(2.0f)))
				{
					WalkToPlayer(2.0f, 70.0f);
				}
			}

			if (mRandom == 2)
			{
				OnceAniamtion(L"SwordMan_Disarm_Attack1");
			}
		}
		break;
		case ya::enums::eSituation::Sit:
			break;
		case ya::enums::eSituation::Run:
		{
			OnceAniamtion(L"SwordMan_Walk");
			if (WalkToPlayer(2.0f, 70.0f))
			{
				TurnToPlayer();
				mRandom = RandomNumber(1, 3);
				SetSituation(enums::eSituation::Attack, true);
			}
		}
		break;
		case ya::enums::eSituation::Groggy:
		{

		}
		break;
		case ya::enums::eSituation::Hit:
		{
			if (IsHitRight())
			{
				OnceAniamtion(L"SwordMan_Hit_7");
				SetHitRight(false);
			}
			else if (IsHitLeft())
			{
				OnceAniamtion(L"SwordMan_Hit_6");
				SetHitLeft(false);
			}
		}
		break;
		/*case ya::enums::eSituation::AttackHit:
		{
			BoneAnimator* animator = mMeshData->GetAnimator();

			std::wstring aniname = animator->GetPlayAnimationName();
			std::wstring Finnainame;
			if (aniname == L"SwordMan_3Default_Attack1")
			{
				Finnainame = L"SwordMan_3Default_Attack2_Failed";
			}
			else
			{
				Finnainame = aniname + L"_Failed";
			}
			OnceAniamtion(Finnainame);
		}
			break;*/
		case ya::enums::eSituation::Death:
		{
			if(IsDeathBlowKill())
			OnceAniamtion(L"SwordMan_Death_Deathblow1");
			else
			OnceAniamtion(L"SwordMan_Death");

		}
		break;
		case ya::enums::eSituation::End:
			break;

		}


		GameObject::FixedUpdate();

	}

	void Spearman::Render()
	{



		MonsterBase::Render();
	}

	void Spearman::Attack_sting()
	{

		Spearman_Sting* attack = object::Instantiate<Spearman_Sting>(eLayerType::Monster, eSceneType::Play);

		Transform* tr = GetComponent<Transform>();
		Vec3 rot = tr->GetLocalRotation();

		Transform* attacktr = attack->GetComponent<Transform>();


		attacktr->SetLocalPosition(tr->GetLocalPosition() + tr->Forward() * mAttackRange);
		attacktr->SetLocalScale(Vec3(3.0f, 2.0f, 4.0f));
		attacktr->SetLocalRotation(rot);

		Collider3D* attackcol = attack->AddComponent<Collider3D>();
		attackcol->SetType(eColliderType::Box);
		attackcol->SetSize(Vector3(3.0, 2.0f, 4.0f));



	}

	void Spearman::Idle_Stand()
	{
		OnceAniamtion(L"SwordMan_Boundary_Step1");
		SetSituation(enums::eSituation::Idle, true);
	}

	void Spearman::Link_attack()
	{
		SetOnceAnimation(true);


		if (mMeshData->GetPlayAnimationName() == L"SwordMan_1Default_Attack1")
		{
			if (!NavigationPlayer(5.0f) || !IsPlayerFront())
				Idle_Stand();
			else
			OnceAniamtion(L"SwordMan_1Default_Attack2");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_1Default_Attack2")
		{
			Idle_Stand();
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_2Default_Attack1")
		{
			if (!NavigationPlayer(5.0f) || !IsPlayerFront())
				Idle_Stand();
			else
			OnceAniamtion(L"SwordMan_2Default_Attack2");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_2Default_Attack2")
		{
			if (!NavigationPlayer(5.0f) || !IsPlayerFront())
				Idle_Stand();
			else
			OnceAniamtion(L"SwordMan_2Default_Attack3");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_2Default_Attack3")
		{
			Idle_Stand();
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_3Default_Attack1")
		{
			if (!NavigationPlayer(5.0f) || !IsPlayerFront())
				Idle_Stand();
			else
			OnceAniamtion(L"SwordMan_3Default_Attack2");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_3Default_Attack2")
		{
			Idle_Stand();
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_Boundary_Start1")
		{
			OnceAniamtion(L"SwordMan_Boundary_Step1");

			TurnToPlayer();
			if (NavigationPlayer(5.0f))
			{
				SetSituation(enums::eSituation::Battle, true);
			}
			else if (NavigationPlayer(9.0f))
			{
				mRandom = RandomNumber(1, 4);
				SetSituation(enums::eSituation::Chase, true);
				++mRandomCount;
			}
			else if (NavigationPlayer(11.0f))
			{
				SetOnceAnimation(true);
				TurnToPlayer();
				OnceAniamtion(L"SwordMan_Start_Attack2");
				mRandom = 1;
				SetSituation(enums::eSituation::DashAttack, false);
			}
			else
			{
				//여기가 경계 레벨
				int a = 0;

			}


		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_Boundary_Start2")
		{
			OnceAniamtion(L"SwordMan_Boundary_Step2");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_Start_Attack2")
		{
			Idle_Stand();
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_Disarm_Attack1")
		{
			Idle_Stand();
		}


	}

	void Spearman::Link_Defense()
	{
		SetOnceAnimation(true);

		if (mMeshData->GetPlayAnimationName() == L"SwordMan_1Default_Defense1")
		{
			OnceAniamtion(L"SwordMan_1Default_Defense2");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_1Default_Defense2")
		{
			SetDefense(false);
			mTime = 0.f;
			Idle_Stand();
		}
	}

	void Spearman::Link_Boundary()
	{
		//두리번 거리고 앞인지 뒤인지 확인한다.
		if (IsPlayerFront())
		{
			//지금 상황이 앞에 있고 시야각이면 좀 거리가 있고 시야각이 아닐때 약간
			//떨어져 있는상황이다. 여기서 칼뽑고 두리번 거린게 끝난 상황 



		}
		else
		{
			//뒤에 있다면 이건 아직 예정에 없는 상황이긴함.


		}



	}

	void Spearman::Link_Hit()
	{
		TurnToPlayer();
		SetOnceAnimation(true);
		OnceAniamtion(L"SwordMan_Boundary_Step1");
		SetSituation(enums::eSituation::Battle, true);
		mTime = 0.f;

	}

	void Spearman::Link_Walk()
	{
		OnceAniamtion(L"SwordMan_Boundary_Step1");
		SetSituation(enums::eSituation::Idle, true);
		mTime = 0.f;
	}

	void Spearman::Link_Death()
	{
		SetOnceAnimation(true);
		//OnceAniamtion(L"SwordMan_Death");
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_Death_Deathblow1")
		{
			OnceAniamtion(L"SwordMan_Death");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_Death_Hit1")
		{
			OnceAniamtion(L"SwordMan_Death");
		}
		//뒤로 쓰러져 죽는 모션 고정이 없음
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_Death_Hit2")
		{			

		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_Death_Down_1")
		{			
			OnceAniamtion(L"SwordMan_Death_Down_2");
		}
		

	}

	void Spearman::KatanaColliderInit()
	{

#pragma region	Attack_1

		BoneCollider* Rkatana = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile, eSceneType::Play);
		Rkatana->SetMeshAndBone(mMeshData, L"R_Katana");
		Rkatana->SetAnimOffSet(L"SwordMan_1Default_Attack1", Vector3(0.0f, 0.4f, 0.0f));
		Rkatana->SetColliderActiveFrame(L"SwordMan_1Default_Attack1", 20, 23);
		Rkatana->SetBCOwner(this);
		BoneCollider* Lkatana = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile, eSceneType::Play);
		Lkatana->SetMeshAndBone(mMeshData, L"L_Katana");
		Lkatana->SetAnimOffSet(L"SwordMan_1Default_Attack1", Vector3(0.0f, 0.4f, 0.0f));
		Lkatana->SetColliderActiveFrame(L"SwordMan_1Default_Attack1", 36, 38);
		Lkatana->SetBCOwner(this);
		{
			Rkatana->SetAnimOffSet(L"SwordMan_1Default_Attack2", Vector3(-0.1f, 0.6f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_1Default_Attack2", 28, 31);
		}
		{
			Lkatana->SetAnimOffSet(L"SwordMan_1Default_Attack2", Vector3(-0.1f, 0.6f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"SwordMan_1Default_Attack2", 28, 31);
		}
#pragma endregion

#pragma region	Attack_2

		{
			Lkatana->SetAnimOffSet(L"SwordMan_2Default_Attack1", Vector3(0.5f, 0.5f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"SwordMan_2Default_Attack1", 33, 37);
		}


		{
			Rkatana->SetAnimOffSet(L"SwordMan_2Default_Attack2", Vector3(0.0f, 0.5f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_2Default_Attack2", 27, 30);
		}

		{
			Rkatana->SetAnimOffSet(L"SwordMan_2Default_Attack3", Vector3(0.0f, 0.4f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_2Default_Attack3", 27, 29);
		}
		{
			Lkatana->SetAnimOffSet(L"SwordMan_2Default_Attack3", Vector3(0.0f, 0.4f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"SwordMan_2Default_Attack3", 27, 29);
		}
#pragma endregion

#pragma region	Attack_3

		{
			Lkatana->SetAnimOffSet(L"SwordMan_3Default_Attack1", Vector3(0.2f, 0.4f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"SwordMan_3Default_Attack1", 45, 47);
		}


		{
			Rkatana->SetAnimOffSet(L"SwordMan_3Default_Attack2", Vector3(0.2f, 0.7f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_3Default_Attack2", 30, 32);
		}
#pragma endregion

#pragma region	Start_Attack_1

		{
			Rkatana->SetAnimOffSet(L"SwordMan_Start_Attack1", Vector3(0.5f, 0.9f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_Start_Attack1", 43, 46);
		}

#pragma endregion

#pragma region	Start_Attack_2

		{
			Rkatana->SetAnimOffSet(L"SwordMan_Start_Attack2", Vector3(-0.1f, 0.6f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_Start_Attack2", 55, 58);
		}
		{
			Lkatana->SetAnimOffSet(L"SwordMan_Start_Attack2", Vector3(-0.1f, 0.6f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"SwordMan_Start_Attack2", 55, 58);
		}

#pragma endregion

#pragma region	Disarm_Attack_1

		{
			Rkatana->SetAnimOffSet(L"SwordMan_Disarm_Attack1", Vector3(0.0f, 0.3f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_Disarm_Attack1", 21, 29);
		}

#pragma endregion


	}

	void Spearman::Animation_Event()
	{

		mMeshData->GetAnimationCompleteEvent(L"SwordMan_1Default_Defense1")
			= std::bind(&Spearman::Link_Defense, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_1Default_Defense2")
			= std::bind(&Spearman::Link_Defense, this);


		mMeshData->GetAnimationCompleteEvent(L"SwordMan_1Default_Attack1")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_1Default_Attack2")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_2Default_Attack1")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_2Default_Attack2")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_2Default_Attack3")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_3Default_Attack1")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_3Default_Attack2")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Boundary_Start1")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Boundary_Start2")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Start_Attack2")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Disarm_Attack1")
			= std::bind(&Spearman::Link_attack, this);


		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Boundary_Step2")
			= std::bind(&Spearman::Link_Boundary, this);

		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Hit_6")
			= std::bind(&Spearman::Link_Hit, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Hit_7")
			= std::bind(&Spearman::Link_Hit, this);


		mMeshData->GetAnimationCompleteEvent(L"SwordMan_1Default_Defense_Back")
			= std::bind(&Spearman::Link_Walk, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_1Default_Defense_Forward")
			= std::bind(&Spearman::Link_Walk, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_1Default_Defense_Left")
			= std::bind(&Spearman::Link_Walk, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_1Default_Defense_Right")
			= std::bind(&Spearman::Link_Walk, this);


		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Death_Deathblow1")
			= std::bind(&Spearman::Link_Death, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Death_Hit1")
			= std::bind(&Spearman::Link_Death, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Death_Hit2")
			= std::bind(&Spearman::Link_Death, this);
		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Death_Down_1")
			= std::bind(&Spearman::Link_Death, this);



	}




}