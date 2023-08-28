#include "yaMusketeerman_Shooting.h"
#include "yaTime.h"
#include "yaRigidbody.h"


namespace ya
{


	void Musketeerman_Shooting::Initialize()
	{

		GameObject::Initialize();
	}

	void Musketeerman_Shooting::Update()
	{

		Rigidbody* rigi = GetComponent<Rigidbody>();
		Transform* tr = GetComponent<Transform>();

		rigi->AddForce(tr->Forward() * 70.f);


		mDeathTime += Time::DeltaTime();
		if (mDeathTime >= 5.0f)
		{
			Death();
			mDeathTime = 0.f;
		}

		GameObject::Update();

	}

	void Musketeerman_Shooting::FixedUpdate()
	{


		GameObject::FixedUpdate();
	}

}