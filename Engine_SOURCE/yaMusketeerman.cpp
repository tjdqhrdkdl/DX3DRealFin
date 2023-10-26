#include "yaMusketeerman.h"
#include "yaMusketeerman_Almost.h"
#include "yaMusketeerman_Shooting.h"
#include "yaMonsterScript.h"
#include "yaInput.h"

namespace ya
{


	void Musketeerman::Initialize()
	{

		GetComponent<Transform>()->SetLocalPosition(Vector3(5.0f, 0.0f, 15.0f));
		//GetComponent<Transform>()->SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));
		GetComponent<Transform>()->SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));

		SetName(L"Musketeerman");

		Collider3D* spearmancol = AddComponent <Collider3D>();
		spearmancol->SetType(eColliderType::Box);
		spearmancol->SetSize(Vector3(1.0, 2.2f, 1.0f));

		AddComponent<MonsterScript>();
		AddComponent<Rigidbody>();
		mActionScript = AddComponent<ActionScript>();

		SetPlayerObject(SceneManager::GetActiveScene()->GetPlayer());

		Transform* tr = GetComponent<Transform>();



		CreateDeathBlowMark();
		SetMonsterHpBarOffSetOffSet(Vector3(0.0f, 3.0f, 0.0f));
		SetDeathBlowMarkOffSet(Vector3(0.0f, 0.5f, 0.0f));
		mMeshData = std::make_shared<MeshData>();

		mMeshData->Load(L"Monster\\Musketeerman\\MeshData\\c1700_Musketeerman.meshdata");

		mMeshData->AnimationLoad(L"Monster\\Musketeerman\\AnimationData\\MusketeermanAnimation_1.animationdata");

		//mMeshData = MeshData::LoadFromFbx(L"Monster\\SwordMan\\Mesh\\c1700_SwordMan.fbx");


		//mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_000401.fbx", L"a000_000000");
		mMeshObject = mMeshData->Instantiate(eLayerType::Monster);

		Transform* meshobjtr = mMeshObject->GetComponent<Transform>();
		meshobjtr->SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));
		meshobjtr->SetLocalRotation(Vector3(180.f, 0.0f, 0.0f));
		meshobjtr->SetRotationOffset(Vector3(0.0f, 1.0f, 0.0f));
		meshobjtr->SetParent(GetComponent<Transform>());

		Animation_Event();


		mMeshData->Play(L"Musketeerman_Idle_Stand");

		CreateMonsterState();
		SetSituation(enums::eSituation::None, true);


		mAttackRange = 5.0f;
		mTime = 0.f;

		MonsterBase::Initialize();
	}

	void Musketeerman::FixedUpdate()
	{
		Transform* tr = GetComponent<Transform>();
		Rigidbody* rigi = GetComponent<Rigidbody>();
		Vec3 rot = tr->GetLocalRotation();

		Vec3 playerPos = GetPlayerPos();
		Vec3 monsterPos = GetComponent<Transform>()->GetLocalPosition();

		Player* player = (Player*)GetPlayerObject();

	

		if (Input::GetKey(eKeyCode::NUM_1))
		{
			AddHp(Time::DeltaTime() * 3);
		}
		if (Input::GetKey(eKeyCode::NUM_2))
		{ 
			AddHp(-Time::DeltaTime() * 3);
		}
		if (Input::GetKeyDown(eKeyCode::NUM_3))
		{
			AddHp(2.0f);
		}
		if (Input::GetKeyDown(eKeyCode::NUM_4))
		{
			AddHp(-2.0f);
		}

		if (Input::GetKeyDown(eKeyCode::U))
		{
			mMeshData->Play(L"Musketeerman_Boundary_Step1");
		}
		if (Input::GetKeyDown(eKeyCode::I))
		{			
			mMeshData->Play(L"Musketeerman_Attack");
		}
		if (Input::GetKeyDown(eKeyCode::O))
		{
			mMeshData->Play(L"Musketeerman_Defense");
		}
		if (Input::GetKeyDown(eKeyCode::P))
		{
			mMeshData->Play(L"Musketeerman_Defense_Parrying");
		}

		eSituation test = GetSituation();

		switch (GetSituation())
		{
		case ya::enums::eSituation::None:
		{
			if (NavigationPlayer(2.5f) && !IsPlayerFront())
			{
				SetDeathBlow(true);
			}
			else
			{
				SetDeathBlow(false);
			}
			
			if (IsPlayerFieldview(45.0f, 135.0f))
			{
				
				if (NavigationPlayer(30.0f))
				{				
					OnceAniamtion(L"Musketeerman_Boundary_Step1");
				}
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
				TurnToPlayer();
				SetSituation(enums::eSituation::Run);				
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
			mTime += Time::DeltaTime();
			if (mTime >= 0.4f)
			{
				mTime = 0.f;
				SetSituation(enums::eSituation::None);
			}
			rigi->AddForce((tr->Forward() * -1) * 100.f);

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
		{



		}
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
		Musketeerman_Almost* attack = object::Instantiate<Musketeerman_Almost>(eLayerType::MonsterProjectile);

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

	void Musketeerman::Attack_Shooting()
	{

		Musketeerman_Shooting* attack = object::Instantiate<Musketeerman_Shooting>(eLayerType::MonsterProjectile);

		Transform* tr = GetComponent<Transform>();
		Vec3 rot = tr->GetLocalRotation();

		Transform* attacktr = attack->GetComponent<Transform>();


		attacktr->SetLocalPosition(tr->GetLocalPosition() + tr->Forward() * mAttackRange);
		attacktr->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
		attacktr->SetLocalRotation(rot);

		Collider3D* attackcol = attack->AddComponent<Collider3D>();
		attackcol->SetType(eColliderType::Box);
		attackcol->SetSize(Vector3(1.0, 1.0f, 1.0f));


		Rigidbody* rigi = attack->AddComponent<Rigidbody>();
		//rigi->SetGround(true);

	}

	void Musketeerman::Animation_Event()
	{

		mMeshData->GetAnimationCompleteEvent(L"Musketeerman_Boundary_Step1")
			= std::bind(&Musketeerman::Link_attack, this);
		mMeshData->GetAnimationCompleteEvent(L"Musketeerman_Attack")
			= std::bind(&Musketeerman::Idle_Stand, this);
		mMeshData->GetAnimationCompleteEvent(L"Musketeerman_Defense")
			= std::bind(&Musketeerman::Idle_Stand, this);



	}

	void Musketeerman::Idle_Stand()
	{
		OnceAniamtion(L"Musketeerman_Boundary_Stand");
		SetSituation(enums::eSituation::Idle, true);
	}

	void Musketeerman::Link_attack()
	{
		SetOnceAnimation(true);

		if (mMeshData->GetPlayAnimationName() == L"Musketeerman_Boundary_Step1")
		{
			Idle_Stand();
		}


	}

}