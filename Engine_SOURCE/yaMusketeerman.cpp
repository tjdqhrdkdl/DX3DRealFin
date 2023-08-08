#include "yaMusketeerman.h"
#include "yaMusketeerman_Almost.h"
#include "yaMusketeerman_Shooting.h"


namespace ya
{


	void Musketeerman::Initialize()
	{


		CreateMonsterState();
		SetSituation(enums::eSituation::None);


		mAttackRange = 5.0f;
		mTime = 0.f;

		MonsterBase::Initialize();
	}

	void Musketeerman::FixedUpdate()
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
			
			if(NavigationPlayer(3.0f))			
			{	
				SetSituation(enums::eSituation::Attack);
				mbAlmostAttack = true;
				TurnToPlayer();
			}
			else if (NavigationPlayer(5.5f))
			{
				//SetSituation(enums::eSituation::Run);				
			}		

			else
			{
				SetSituation(enums::eSituation::Attack);
				mbAlmostAttack = false;
				TurnToPlayer();
			}
			
		}
			break;
		case ya::enums::eSituation::Run:
		{

		}
			break;
		case ya::enums::eSituation::Defense:
			break;
		case ya::enums::eSituation::Attack:
		{
			if (mbAlmostAttack)
			{
				Attack_Almost();
			}
			else
			{
				Attack_Shooting();
			}
			SetSituation(enums::eSituation::None);

		}
			break;
		case ya::enums::eSituation::Sit:
			break;
		case ya::enums::eSituation::Death:
			break;
		case ya::enums::eSituation::End:
			break;
		default:
			break;
		}





		GameObject::FixedUpdate();
	}

	void Musketeerman::Render()
	{

		MonsterBase::Render();
	}

	void Musketeerman::Attack_Almost()
	{
		Musketeerman_Almost* attack = object::Instantiate<Musketeerman_Almost>(eLayerType::Monster);

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

	void Musketeerman::Attack_Shooting()
	{

		Musketeerman_Shooting* attack = object::Instantiate<Musketeerman_Shooting>(eLayerType::Monster);

		Transform* tr = GetComponent<Transform>();
		Vec3 rot = tr->GetRotation();

		Transform* attacktr = attack->GetComponent<Transform>();


		attacktr->SetPosition(tr->GetPosition() + tr->Forward() * mAttackRange);
		attacktr->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		attacktr->SetRotation(rot);

		Collider2D* attackcol = attack->AddComponent<Collider2D>();
		attackcol->SetType(eColliderType::Box);
		attackcol->SetSize(Vector3(1.0, 1.0f, 1.0f));


		Rigidbody* rigi = attack->AddComponent<Rigidbody>();
		rigi->SetGround(true);

	}

}