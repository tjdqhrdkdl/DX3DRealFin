#include "yaSwordsman.h"



namespace ya
{

	void Swordsman::Initialize()
	{

		CreateMonsterState();
		SetSituation(enums::eSituation::None);

		MonsterBase::Initialize();
	}

	void Swordsman::FixedUpdate()
	{
		Transform* tr = GetComponent<Transform>();
		Rigidbody* rigi = GetComponent<Rigidbody>();
		Vec3 rot = tr->GetLocalRotation();

		Vec3 playerPos = GetPlayerPos();
		Vec3 monsterPos = GetComponent<Transform>()->GetLocalPosition();


		switch (GetSituation())
		{
		case ya::enums::eSituation::None:
			break;
		case ya::enums::eSituation::Idle:
			break;
		case ya::enums::eSituation::Boundary:
			break;
		case ya::enums::eSituation::Chase:
			break;
		case ya::enums::eSituation::Battle:
			break;
		case ya::enums::eSituation::Run:
			break;
		case ya::enums::eSituation::Defense:
			break;
		case ya::enums::eSituation::Attack:
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

	void Swordsman::Render()
	{

		MonsterBase::Render();
	}

}