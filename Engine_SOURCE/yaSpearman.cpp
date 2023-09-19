#include "yaSpearman.h"
#include "yaSpearman_Sting.h"
#include "yaMonsterScript.h"

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

		

		Collider2D* spearmancol = AddComponent <Collider2D>();
		spearmancol->SetType(eColliderType::Box);
		spearmancol->SetSize(Vector3(1.0, 2.0f, 1.0f));
		AddComponent<MonsterScript>();
	

		Transform* tr = GetComponent<Transform>();
		{
			Transform* tr = GetComponent<Transform>();
			GameObject* face = object::Instantiate<GameObject>(eLayerType::Player, tr);
			face->SetName(L"TEST");
			TEST = face->GetComponent<Transform>();
			TEST->SetPosition(Vector3(100.0f, 100.0f, 100.0f));
			TEST->SetScale(Vector3(1.3f, 1.3f, 0.3f));
			MeshRenderer* faceRenderer = face->AddComponent<MeshRenderer>();
			faceRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			faceRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
		}

		

		mMeshData = std::make_shared<MeshData>();
		mMeshData->Load(L"Monster\\SwordMan\\MeshData\\c1700_SwordMan.meshdata");
		
		mMeshData->AnimationLoad(L"Monster\\SwordMan\\AnimationData\\SwordManAnimation_1.animationdata");
		
		mMeshObject = mMeshData->Instantiate(eLayerType::Monster);

		Transform* meshobjtr = mMeshObject->GetComponent<Transform>();
		meshobjtr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		meshobjtr->SetRotation(Vector3(180.f, 0.0f, 0.0f));
		//meshobjtr->SetRotationOffset(Vector3(-2.0f, 1.0f, 0.0f));
		meshobjtr->SetParent(GetComponent<Transform>());
		
		KatanaColliderInit();
		
		Animation_Event();
		
		
		mMeshData->Play(L"a000_000401");

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

		//Transform* meshobjtr = mMeshObject->GetComponent<Transform>();
		//meshobjtr->SetPosition(tr->GetPosition());
		
		//meshobjtr->SetRotation(tr->GetRotation());

		if (IsDeathBlow())
		{
			TEST->SetPosition(Vector3(0.0f, 2.0f, 0.0f));
		}
		else
		{
			TEST->SetPosition(Vector3(100.0f, 100.0f, 100.0f));
		}


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
					//TurnToPlayer();
					//OnceAniamtion(L"a000_000402");
					//SetSituation(enums::eSituation::Battle, true);
				}
			}
			//플레이어가 시야각에서 벗어나 있는 경우?
			else
			{
				if (IsPlayerFront())
				{
					if (NavigationPlayer(20.0f))
					{						
						//OnceAniamtion(L"a000_000200");
						//SetSituation(enums::eSituation::Boundary, true);
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
			OnceAniamtion(L"a000_400000");
			mTime += Time::DeltaTime();
			if (mTime >= 3.0f)
			{
				SetSituation(enums::eSituation::Battle, true);
				mTime = 0.f;
			}



		}
		break;
		case ya::enums::eSituation::Boundary:
		{
			//두리번 거리는 애니메이션

		}
			break;
		case ya::enums::eSituation::Chase:
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
				OnceAniamtion(L"a000_405010");
				SetSituation(enums::eSituation::Run, true);
			}
			else if (mRandom <= 3)
			{
				TurnToPlayer();
				SetSituation(enums::eSituation::Defense, true);
			}
			else if (mRandom > 3)
			{
				TurnToPlayer();
				OnceAniamtion(L"a000_405000");
				SetSituation(enums::eSituation::Attack, true);
				mRandom = RandomNumber(1, 3);
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
			OnceAniamtion(L"a000_500000");


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
			mTime += Time::DeltaTime();
			if (mTime >= 1.0f)
			{
				mTime = 0.f;
			}

			if (WalkToPlayer(3.0f))
			{
				
				//Attack_sting();
				if (mRandom == 1)
				{
					OnceAniamtion(L"a000_003006");
					

				}

				if (mRandom == 2)
				{
					OnceAniamtion(L"a000_003010");					
					
				}

				if (mRandom == 3)
				{
					OnceAniamtion(L"a000_003015");					
					//OnceAniamtion(L"a000_003001");
				}				
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
		OnceAniamtion(L"a000_400000");
		SetSituation(enums::eSituation::Idle, true);
	}

	void Spearman::Link_attack()
	{
		SetOnceAnimation(true);


		if (mMeshData->GetPlayAnimationName() == L"a000_003006")
		{			
			OnceAniamtion(L"a000_003007");
		}
		if (mMeshData->GetPlayAnimationName() == L"a000_003007")
		{
			Idle_Stand();
		}
		if (mMeshData->GetPlayAnimationName() == L"a000_003010")
		{			
			OnceAniamtion(L"a000_003011");
		}
		if (mMeshData->GetPlayAnimationName() == L"a000_003011")
		{			
			OnceAniamtion(L"a000_003012");
		}
		if (mMeshData->GetPlayAnimationName() == L"a000_003012")
		{
			Idle_Stand();
		}
		if (mMeshData->GetPlayAnimationName() == L"a000_003015")
		{			
			OnceAniamtion(L"a000_003016");
		}
		if (mMeshData->GetPlayAnimationName() == L"a000_003016")
		{
			Idle_Stand();
		}


	}

	void Spearman::Link_Defense()
	{
		SetOnceAnimation(true);

		if (mMeshData->GetPlayAnimationName() == L"a000_500000")
		{
			OnceAniamtion(L"a000_501040");
		}
		if (mMeshData->GetPlayAnimationName() == L"a000_501040")
		{
			mTime = 0.f;
			Idle_Stand();
		}
	}

	void Spearman::KatanaColliderInit()
	{

#pragma region	Attack_1
		
			BoneCollider* Rkatana = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile);
			Rkatana->SetMeshAndBone(mMeshData, L"R_Katana");
			Rkatana->SetAnimOffSet(L"a000_003006", Vector3(2.0f, 0.5f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"a000_003006", 20, 23);		
		
			BoneCollider* Lkatana = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile);
			Lkatana->SetMeshAndBone(mMeshData, L"L_Katana");
			Lkatana->SetAnimOffSet(L"a000_003006", Vector3(2.0f, 0.5f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"a000_003006", 36, 38);
		
		{			
			Rkatana->SetAnimOffSet(L"a000_003007", Vector3(0.7f, 0.8f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"a000_003007", 28, 31);
		}
		{			
			Lkatana->SetAnimOffSet(L"a000_003007", Vector3(0.7, 0.8, 0));
			Lkatana->SetColliderActiveFrame(L"a000_003007", 28, 31);
		}
#pragma endregion

#pragma region	Attack_2

		{			
			Lkatana->SetAnimOffSet(L"a000_003010", Vector3(1.8f, 0.5f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"a000_003010", 33, 37);
		}


		{			
			Rkatana->SetAnimOffSet(L"a000_003011", Vector3(1.0f, 0.5f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"a000_003011", 27, 30);
		}

		{			
			Rkatana->SetAnimOffSet(L"a000_003012", Vector3(0.9f, 1.0f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"a000_003012", 27, 29);
		}
		{			
			Lkatana->SetAnimOffSet(L"a000_003012", Vector3(0.9f, 1.0f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"a000_003012", 27, 29);
		}
#pragma endregion

#pragma region	Attack_3

		{			
			Lkatana->SetAnimOffSet(L"a000_003015", Vector3(0.9f, 1.0f, 0.0f));
			Lkatana->SetColliderActiveFrame(L"a000_003015", 45, 47);
		}


		{
			Rkatana->SetAnimOffSet(L"a000_003016", Vector3(0.9f, 0.9f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"a000_003016", 30, 32);
		}
#pragma endregion

#pragma region	Start_Attack_1

		{
			Rkatana->SetAnimOffSet(L"a000_003000", Vector3(0.9f, 0.9f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"a000_003000", 43, 46);
		}

#pragma endregion

#pragma region	Start_Attack_2

		{
			Rkatana->SetAnimOffSet(L"a000_003001", Vector3(0.7f, 0.8f, 0.0f));
			Rkatana->SetColliderActiveFrame(L"a000_003001", 55, 58);
		}
		{
			Lkatana->SetAnimOffSet(L"a000_003001", Vector3(0.7, 0.8, 0));
			Lkatana->SetColliderActiveFrame(L"a000_003001", 55, 58);
		}

#pragma endregion
	}

	void Spearman::Animation_Event()
	{

		mMeshData->GetAnimationCompleteEvent(L"a000_500000")
			= std::bind(&Spearman::Link_Defense, this);
		mMeshData->GetAnimationCompleteEvent(L"a000_501040")
			= std::bind(&Spearman::Link_Defense, this);
		mMeshData->GetAnimationCompleteEvent(L"a000_003006")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"a000_003007")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"a000_003010")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"a000_003011")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"a000_003012")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"a000_003015")
			= std::bind(&Spearman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"a000_003016")
			= std::bind(&Spearman::Link_attack, this);



	}




}