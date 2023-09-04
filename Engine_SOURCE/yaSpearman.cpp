#include "yaSpearman.h"
#include "yaSpearman_Sting.h"

namespace ya
{
	void Spearman::Initialize()
	{

		Transform* tr = GetComponent<Transform>();
		// 플레이어의 forward를 구분하기위한 object
		// 후에 mesh 씌우면 없앨 예정
		GameObject* face = object::Instantiate<GameObject>(eLayerType::Player, tr);
		face->SetName(L"face");
		Transform* faceTr = face->GetComponent<Transform>();
		faceTr->SetPosition(Vector3(0.0f, 0.5f, 0.5f));
		faceTr->SetScale(Vector3(0.4f, 0.4f, 0.4f));
		MeshRenderer* faceRenderer = face->AddComponent<MeshRenderer>();
		faceRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		faceRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);


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
		mMeshData->Load(L"Monster\\IInteriorMinistry_Samurai\\MeshData\\c1700_SpearMan.meshdata");
		
		mMeshData->AnimationLoad(L"Monster\\IInteriorMinistry_Samurai\\AnimationData\\SpearManAnimation_1.animationdata");
		

		mMeshObject = mMeshData->Instantiate(eLayerType::Monster);
		Transform* meshobjtr = mMeshObject->GetComponent<Transform>();
		meshobjtr->SetScale(Vector3(5.0f, 5.0f, 5.0f));
		meshobjtr->SetRotation(Vector3(180.f, 0.0f, 0.0f));
		mAnimationOffSet = Vec3(-5.5f, 0.0f, 4.0f);

		CreateMonsterState();
		SetSituation(enums::eSituation::None);

		//SpearMan_Running
		mMeshData->Play(L"SpearMan_Boundary_Step1");

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
		//meshobjtr->SetPosition(tr->GetPosition() + mAnimationOffSet);

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
						SetSituation(enums::eSituation::Boundary, true);
					}


				}

			}
		}
		break;
		case ya::enums::eSituation::Idle:
		{
			OnceAniamtion(L"SpearMan_Boundary_Step2");
		}
		break;
		case ya::enums::eSituation::Boundary:
		{
			OnceAniamtion(L"SpearMan_Boundary_Step2");
			//mAnimationOffSet = Vec3();


		}
			break;
		case ya::enums::eSituation::Chase:
			break;
		case ya::enums::eSituation::Battle:
		{
			int random = RandomNumber(1, 3);

			random = 1;

			if (random == 1)
			{
				mRandomXY.x = RandomNumber((int)(monsterPos.x - 4), (int)(monsterPos.x + 4));
				mRandomXY.y = RandomNumber((int)(monsterPos.z - 4), (int)(monsterPos.z + 4));


				mRandomFinPos.x = mRandomXY.x;
				mRandomFinPos.z = mRandomXY.y;
				mRandomFinPos.y = monsterPos.y;
				mWlakFixPos = monsterPos;

				MonsterRotation(mRandomFinPos);
				SetSituation(enums::eSituation::Run, true);
			}
			else if (random == 2)
			{
				SetSituation(enums::eSituation::Defense, true);
				TurnToPlayer();
			}
			else if (random == 3)
			{
				SetSituation(enums::eSituation::Attack);
				TurnToPlayer();
			}
		}
		break;
		case ya::enums::eSituation::Run:
		{
			OnceAniamtion(L"SpearMan_Walk");
			//오른쪽 방향
			if (mRandomXY.x > mWlakFixPos.x)
			{
				if (monsterPos.x > mRandomFinPos.x)
				{
					SetSituation(enums::eSituation::Idle, true);
				}
				rigi->AddForce(tr->Forward() * 70.f);

			}
			//왼쪽 방향
			else if (mRandomXY.x <= mWlakFixPos.x)
			{
				if (monsterPos.x <= mRandomFinPos.x)
				{
					SetSituation(enums::eSituation::Idle, true);
				}

				rigi->AddForce(tr->Forward() * 70.f);
			}

		}
		break;
		case ya::enums::eSituation::Defense:
		{
			mTime += Time::DeltaTime();

			if (mTime < 1.0f)
			{
				mbParrying = true;
			}
			else
			{
				mbParrying = false;
			}
			if (mTime >= 3.0f)
			{
				SetSituation(enums::eSituation::Idle, true);
				mTime = 0.f;
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

			if (WalkToPlayer(2.0f))
			{
				TurnToPlayer();
				Attack_sting();
				SetSituation(enums::eSituation::Idle, true);
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

}