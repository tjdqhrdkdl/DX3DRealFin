#include "yaSpearman.h"

namespace ya
{

	Spearman::Spearman()
	{
	}

	Spearman::~Spearman()
	{
	}

	void Spearman::FixedUpdate()
	{
		if(IsPlayerEyesight())
		{
			Vector3 pos = GetComponent<Transform>()->GetPosition();

			Vector3 dir = GetMonsterToPlayerNormalize();

			pos += dir * 10.f * Time::DeltaTime();
			
			GetComponent<Transform>()->SetPosition(pos);
		}



		GameObject::FixedUpdate();
	}

	void Spearman::Render()
	{

		GameObject::Render();
	}

}