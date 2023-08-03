#include "yaSpearman.h"


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
		faceRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"));
		faceRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));

		CreateMonsterState();
		SetSituation(enums::eSituation::None);


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

		

		switch (GetSituation())
		{
		case ya::enums::eSituation::None:
		{
			mTime += Time::DeltaTime();
			if (mTime >= 3.0f)
			{
				SetSituation(enums::eSituation::Battle);
				mTime = 0.f;
			}
		}
			break;
		case ya::enums::eSituation::Idle:
			break;
		case ya::enums::eSituation::Boundary:
			break;
		case ya::enums::eSituation::Chase:
			break;
		case ya::enums::eSituation::Battle:
		{
			int random = RnadomNumber(1, 3);

			//int random = 1;

			if (random == 1)
			{
				SetSituation(enums::eSituation::None);
				mRandomXY.x = RnadomNumber((int)(monsterPos.x - 4), (int)(monsterPos.x + 4));
				mRandomXY.y = RnadomNumber((int)(monsterPos.z - 4), (int)(monsterPos.z + 4));
				
				
				mRandomFinPos.x = mRandomXY.x;
				mRandomFinPos.z = mRandomXY.y;
				mRandomFinPos.y = monsterPos.y;
				mWlakFixPos = monsterPos;

				MonsterRotation(mRandomFinPos);
				SetSituation(enums::eSituation::Run);
			}
			else if (random == 2)
			{
				SetSituation(enums::eSituation::Defense);		
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
			//오른쪽 방향
			if (mRandomXY.x > mWlakFixPos.x)
			{
				if (monsterPos.x > mRandomFinPos.x)
				{
					SetSituation(enums::eSituation::None);
					int a = 0;
				}
				rigi->AddForce(tr->Forward() * 70.f);

			}
			//왼쪽 방향
			else if (mRandomXY.x <= mWlakFixPos.x)
			{
				if (monsterPos.x <= mRandomFinPos.x)
				{
					SetSituation(enums::eSituation::None);
					int a = 0;
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
				SetSituation(enums::eSituation::None);
				mTime = 0.f;				
			}

		}
			break;
		case ya::enums::eSituation::Attack:
		{
			mTime += Time::DeltaTime();
			if(mTime >= 1.0f)
			{
				SetSituation(enums::eSituation::None);
				mTime = 0.f;
			}
			rigi->AddForce(tr->Forward() * 70.f);
		}
			break;
		case ya::enums::eSituation::Sit:
			break;
		case ya::enums::eSituation::Death:
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

}