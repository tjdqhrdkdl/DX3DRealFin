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
		int a = 0;
	}

	void Spearman::Initialize()
	{

		GetComponent<Transform>()->SetPosition(Vector3(5.0f, 0.0f, 15.0f));
		GetComponent<Transform>()->SetScale(Vector3(5.0f, 5.0f, 5.0f));
		SetName(L"Spearman");

		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);


		Collider2D* spearmancol = AddComponent <Collider2D>();
		spearmancol->SetType(eColliderType::Box);
		spearmancol->SetSize(Vector3(1.0, 2.2f, 1.0f));

		AddComponent<MonsterScript>();
		AddComponent<Rigidbody>();
		mActionScript = AddComponent<ActionScript>();

		SetPlayerObject(SceneManager::GetActiveScene()->GetPlayer());

		//BoneAnimator* animator = mMeshData->GetAnimator();

		////동일 애니메이션 반복시 보간을 하지 않고 싶은 경우. (기본은 보간을 하도록 되어있음.)
		//animator->SetAnimationSelfChange(L"SwordAttack_2", false);

		////애니메이션 끝부분 이상한 것 잘라내는 시간
		//animator->SetAnimationTailTime(0.1f);

		Transform* tr = GetComponent<Transform>();



		CreateDeathBlowMark();
		SetDeathBlowMarkOffSet(Vector3(60.0f, 80.0f, 1.0f));
		mMeshData = std::make_shared<MeshData>();
		mMeshData->Load(L"Monster\\SwordMan\\MeshData\\c1700_SwordMan.meshdata");

		mMeshData->AnimationLoad(L"Monster\\SwordMan\\AnimationData\\SwordManAnimation_1.animationdata");

		mMeshObject = mMeshData->Instantiate(eLayerType::Monster);

		Transform* meshobjtr = mMeshObject->GetComponent<Transform>();
		meshobjtr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		meshobjtr->SetRotation(Vector3(180.f, 0.0f, 0.0f));
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
		Vec3 rot = tr->GetRotation();

		Vec3 playerPos = GetPlayerPos();
		Vec3 monsterPos = GetComponent<Transform>()->GetPosition();




		//if (Input::GetKeyDown(eKeyCode::U))
		//{
		//	SetDeathBlow(!IsDeathBlow());
		//}


		switch (GetSituation())
		{
			//몬스터 처음 상태로 인식을 한번이라도 하면 None상태는 더 안함
		case ya::enums::eSituation::None:
		{
			//플레이어가 가까운 거리에 있으면서 뒤에 있을경우 은신 가능 상태로 변경
			if (NavigationPlayer(3.0f) && !IsPlayerFront())
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
				if (NavigationPlayer(15.0f))
				{
					//배틀로 상태 변경					
					OnceAniamtion(L"SwordMan_Boundary_Start1");

				}
			}
			//플레이어가 시야각에서 벗어나 있는 경우?
			else
			{
				if (IsPlayerFront())
				{
					if (!NavigationPlayer(13.0f))
					{
						OnceAniamtion(L"SwordMan_Boundary_Start2");
					}
				}
			}
		}
		break;
		case ya::enums::eSituation::Idle:
		{
			//바운더리 or None 상태 둘중 하나로 이동
			//None 상태는 플레이어가 은신 + 거리가 멀면 시간지나고 이동
			//바운더리는 플레이어가 아직 가까이 있음 하지만 안보일때 바운더리 유지
			OnceAniamtion(L"SwordMan_Boundary_Step1");
			mTime += Time::DeltaTime();
			if (mTime >= 1.5f)
			{
				TurnToPlayer();
				mRandom = RandomNumber(1, 3);
				if (mRandom < 3)
				{
					SetSituation(enums::eSituation::Battle, true);
					mTime = 0.f;
				}
				else if (mRandom == 3)
				{
					mRandom = RandomNumber(1, 2);
					SetSituation(enums::eSituation::Chase, true);
					mTime = 0.f;
				}

			}

		}
		break;
		case ya::enums::eSituation::Boundary:
		{
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
			mTime += Time::DeltaTime();
			if (mTime >= 3.0f)
			{
				Idle_Stand();
				mTime = 0.f;
			}
			//Back
			if (mRandom == 1)
			{
				OnceAniamtion(L"SwordMan_1Default_Defense_Back");
				mActionScript->Move(-(tr->Forward()), 68.f);
			}
			//Forward
			if (mRandom == 2)
			{
				OnceAniamtion(L"SwordMan_1Default_Defense_Forward");
				mActionScript->Move((tr->Forward()), 68.f);
			}
			//Left
			if (mRandom == 3)
			{
				OnceAniamtion(L"SwordMan_1Default_Defense_Left");
				mActionScript->Move(-(tr->Right()), 68.f);
			}
			//Right
			if (mRandom == 4)
			{
				OnceAniamtion(L"SwordMan_1Default_Defense_Right");
				mActionScript->Move((tr->Right()), 68.f);
			}
		}
		break;
		case ya::enums::eSituation::Battle:
		{
			mRandom = RandomNumber(2, 8);

			mRandom = 4;

			if (mRandom == 1)
			{
				mRandomXY.x = RandomNumber((int)(monsterPos.x - 4), (int)(monsterPos.x + 4));
				mRandomXY.y = RandomNumber((int)(monsterPos.z - 4), (int)(monsterPos.z + 4));


				mRandomFinPos.x = mRandomXY.x;
				mRandomFinPos.z = mRandomXY.y;
				mRandomFinPos.y = monsterPos.y;
				mWlakFixPos = monsterPos;


				MonsterRotation(mRandomFinPos);
				OnceAniamtion(L"SwordMan_Running");
				SetSituation(enums::eSituation::Run, true);
			}
			else if (mRandom <= 3)
			{
				TurnToPlayer();
				SetSituation(enums::eSituation::Defense, true);
			}
			else if (mRandom > 3)
			{

				OnceAniamtion(L"SwordMan_Walk");
				if (WalkToPlayer(4.0f, 500.0f))
				{
					TurnToPlayer();
					SetSituation(enums::eSituation::Attack, true);
					mRandom = RandomNumber(1, 3);
				}
				//mRandom = 3;
			}
		}
		break;
		case ya::enums::eSituation::Run:
		{

			//오른쪽 방향
			if (mRandomXY.x > mWlakFixPos.x)
			{
				if (monsterPos.x > mRandomFinPos.x)
				{
					SetSituation(enums::eSituation::Idle, true);
				}
				rigi->AddForce(tr->Forward() * 130.f);

			}
			//왼쪽 방향
			else if (mRandomXY.x <= mWlakFixPos.x)
			{
				if (monsterPos.x <= mRandomFinPos.x)
				{
					SetSituation(enums::eSituation::Idle, true);
				}

				rigi->AddForce(tr->Forward() * 130.f);
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

			//Attack_sting();
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
				BoneAnimator* animator = mMeshData->GetAnimator();
				if (animator->GetCurrentFrameIdx() < 53 && !(NavigationPlayer(3.0f)))
				{
					WalkToPlayer(3.0f, 1000.0f);
				}
			}
			if (!NavigationPlayer(8.0f))
			{
				Idle_Stand();
			}

		}
		break;
		case ya::enums::eSituation::Sit:
			break;
		case ya::enums::eSituation::Groggy:
		{

		}
		break;
		case ya::enums::eSituation::Death:
			break;
		case ya::enums::eSituation::End:
			break;

		}

		//if (Input::GetKeyDown(eKeyCode::U))
		//{
		//	mMeshData->Play(L"SpearMan_shout");
		//}
		//if (Input::GetKeyDown(eKeyCode::I))
		//{
		//	mMeshData->Play(L"SpearMan_Boundary_Step1");
		//}


		GameObject::FixedUpdate();

	}

	void Spearman::Render()
	{



		MonsterBase::Render();
	}

	void Spearman::Attack_sting()
	{

		Spearman_Sting* attack = object::Instantiate<Spearman_Sting>(eLayerType::Monster);

		Transform* tr = GetComponent<Transform>();
		Vec3 rot = tr->GetRotation();

		Transform* attacktr = attack->GetComponent<Transform>();


		attacktr->SetPosition(tr->GetPosition() + tr->Forward() * mAttackRange);
		attacktr->SetScale(Vec3(3.0f, 2.0f, 4.0f));
		attacktr->SetRotation(rot);

		Collider2D* attackcol = attack->AddComponent<Collider2D>();
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
			OnceAniamtion(L"SwordMan_1Default_Attack2");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_1Default_Attack2")
		{
			Idle_Stand();
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_2Default_Attack1")
		{
			OnceAniamtion(L"SwordMan_2Default_Attack2");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_2Default_Attack2")
		{
			OnceAniamtion(L"SwordMan_2Default_Attack3");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_2Default_Attack3")
		{
			Idle_Stand();
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_3Default_Attack1")
		{
			OnceAniamtion(L"SwordMan_3Default_Attack2");
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_3Default_Attack2")
		{
			Idle_Stand();
		}
		if (mMeshData->GetPlayAnimationName() == L"SwordMan_Boundary_Start1")
		{
			if (NavigationPlayer(16.0f))
			{
				TurnToPlayer();
				if (NavigationPlayer(8.0f))
				{
					SetSituation(enums::eSituation::Battle, true);
				}
				else if (NavigationPlayer(16.0f))
				{
					mRandom = RandomNumber(1, 2);
					SetSituation(enums::eSituation::Chase, true);
				}
			}
			else if (NavigationPlayer(22.0f))
			{
				TurnToPlayer();
				OnceAniamtion(L"SwordMan_Start_Attack2");
				mRandom = 4;
				SetSituation(enums::eSituation::Attack, false);
			}
			else
			{
				TurnToPlayer();
				OnceAniamtion(L"SwordMan_Boundary_Step2");
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

	void Spearman::KatanaColliderInit()
	{

#pragma region	Attack_1

		BoneCollider* Rkatana = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile);
		Rkatana->SetMeshAndBone(mMeshData, L"R_Katana");
		Rkatana->SetAnimOffSet(L"SwordMan_1Default_Attack1", Vector3(2.0f, 0.5f, 0.0f));
		Rkatana->SetColliderActiveFrame(L"SwordMan_1Default_Attack1", 20, 23);

		BoneCollider* Lkatana = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile);
		Lkatana->SetMeshAndBone(mMeshData, L"L_Katana");
		Lkatana->SetAnimOffSet(L"SwordMan_1Default_Attack1", Vector3(2.0f, 0.5f, 0.0f));
		Lkatana->SetColliderActiveFrame(L"SwordMan_1Default_Attack1", 36, 38);

		{
			Rkatana->SetAnimOffSet(L"SwordMan_1Default_Attack2", Vector3(0.7f, 0.8f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_1Default_Attack2", 28, 31);
		}
		{
			Lkatana->SetAnimOffSet(L"SwordMan_1Default_Attack2", Vector3(0.7, 0.8, 0));
			Lkatana->SetColliderActiveFrame(L"SwordMan_1Default_Attack2", 28, 31);
		}
#pragma endregion

#pragma region	Attack_2

		{
			Lkatana->SetAnimOffSet(L"SwordMan_2Default_Attack1", Vector3(1.8f, 0.5f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"SwordMan_2Default_Attack1", 33, 37);
		}


		{
			Rkatana->SetAnimOffSet(L"SwordMan_2Default_Attack2", Vector3(1.0f, 0.5f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_2Default_Attack2", 27, 30);
		}

		{
			Rkatana->SetAnimOffSet(L"SwordMan_2Default_Attack3", Vector3(0.9f, 1.0f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_2Default_Attack3", 27, 29);
		}
		{
			Lkatana->SetAnimOffSet(L"SwordMan_2Default_Attack3", Vector3(0.9f, 1.0f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"SwordMan_2Default_Attack3", 27, 29);
		}
#pragma endregion

#pragma region	Attack_3

		{
			Lkatana->SetAnimOffSet(L"SwordMan_3Default_Attack1", Vector3(0.9f, 1.0f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"SwordMan_3Default_Attack1", 45, 47);
		}


		{
			Rkatana->SetAnimOffSet(L"SwordMan_3Default_Attack2", Vector3(0.9f, 0.9f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_3Default_Attack2", 30, 32);
		}
#pragma endregion

#pragma region	Start_Attack_1

		{
			Rkatana->SetAnimOffSet(L"SwordMan_Start_Attack1", Vector3(0.9f, 0.9f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_Start_Attack1", 43, 46);
		}

#pragma endregion

#pragma region	Start_Attack_2

		{
			Rkatana->SetAnimOffSet(L"SwordMan_Start_Attack2", Vector3(0.7f, 0.8f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"SwordMan_Start_Attack2", 55, 58);
		}
		{
			Lkatana->SetAnimOffSet(L"SwordMan_Start_Attack2", Vector3(0.7, 0.8, 0));
			Lkatana->SetColliderActiveFrame(L"SwordMan_Start_Attack2", 55, 58);
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


		mMeshData->GetAnimationCompleteEvent(L"SwordMan_Boundary_Step2")
			= std::bind(&Spearman::Link_Boundary, this);



	}




}