#include "yaMusketeerman_Almost.h"
#include "yaTime.h"

namespace ya
{

	void Musketeerman_Almost::Initialize()
	{
	}

	void Musketeerman_Almost::Update()
	{
		mDeathTime += Time::DeltaTime();
		if (mDeathTime >= 3.0f)
		{
			Death();
			mDeathTime = 0.f;
		}

		GameObject::Update();

	}

	void Musketeerman_Almost::FixedUpdate()
	{

		GameObject::FixedUpdate();
	}

}