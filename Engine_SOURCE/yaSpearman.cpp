#include "yaSpearman.h"


namespace ya
{
	void Spearman::Initialize()
	{


		//몬스터 할일 있나
		MonsterBase::Initialize();
	}


	void Spearman::FixedUpdate()
	{
		Transform* tr = GetComponent<Transform>();
		Vec3 rot = tr->GetRotation();
		
		Vec3 playerPos = GetPlayerPos();
		Vec3 monsterPos = GetComponent<Transform>()->GetPosition();
		


		Quaternion quater = Quaternion::FromToRotation(tr->Forward(), playerPos);
		Vec3 monDirection = quater.ToEuler();
		

		tr->SetRotation(Vec3(0.0f, rot.y + monDirection.y, 0.0f));



		GameObject::FixedUpdate();
	}

	void Spearman::Render()
	{



		GameObject::Render();
	}

}