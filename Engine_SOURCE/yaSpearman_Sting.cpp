#include "yaSpearman_Sting.h"
#include "yaTime.h"



namespace ya
{

	void Spearman_Sting::Initialize()
	{

		GameObject::Initialize();
	}

	void Spearman_Sting::Update()
	{
		



		mDeathTime += Time::DeltaTime();
		if (mDeathTime >= 3.0f)
		{
			Death();
			mDeathTime = 0.f;
		}

		GameObject::Update();
	
	}

	void Spearman_Sting::FixedUpdate()
	{	
		

		

		GameObject::FixedUpdate();
	}



}
