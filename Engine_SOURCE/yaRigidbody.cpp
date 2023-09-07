#include "yaRigidbody.h"
#include "yaTime.h"
#include "yaGameObject.h"

#include "yaInput.h"

namespace ya
{
	Rigidbody::Rigidbody()
		:Component(eComponentType::Rigidbody)
		, mMass(0.6f)
		, mForce(Vector3::Zero)
		, mVelocity(Vector3::Zero)
		, mAccelation(Vector3::Zero)
		, mFriction(180.0f)
	{
		mGravity = Vector3(0.0f, -200.0f, 0.0f);
		mbGround = false;
		mLimitVelocity = Vector3(40.0f, 100.0f, 40.0f);
	}

	Rigidbody::~Rigidbody()
	{

	}

	void Rigidbody::Initialize()
	{
	}

	void Rigidbody::FixedUpdate()
	{
	}

	void Rigidbody::Update()
	{
		// F = M X A
		// A = F / M

		if (mMass == 0.0f)
			return;

		Transform* tr = GetOwner()->GetComponent<Transform>();

		mAccelation = mForce / mMass;

		// 속도에 가속도를 더해준다.
		mVelocity += (mAccelation * Time::DeltaTime());

		if (mbGround)
		{ // 땅
			Vector3 gravity = mGravity;
			gravity.Normalize();

			float dot = gravity.Dot(mVelocity);
			mVelocity -= gravity * dot;

			Vector3 velocity = mVelocity;
			float length = velocity.Length();
		}
		else
		{ // 공중
			mVelocity += mGravity * Time::DeltaTime();
		}

		//mVelocity += mGravity * Time::DeltaTime();

		// 최대 속도 제한
		Vector3 gravity = mGravity;
		gravity.Normalize();
		float dot = gravity.Dot(mVelocity);
		gravity = gravity * dot;

		Vector3 sideVelocity = mVelocity - gravity;
		if (mLimitVelocity.y < gravity.Length())
		{
			gravity.Normalize();
			gravity *= mLimitVelocity.y;
		}

		if (mLimitVelocity.x < sideVelocity.Length())
		{
			sideVelocity.Normalize();
			sideVelocity *= mLimitVelocity.x;
		}

		mVelocity = gravity + sideVelocity;

		// 마찰력 조건 : 적용된 힘이 없고, 속도가 0이 아닐때
		if (!(mVelocity == Vector3::Zero))
		{
			 //속도에 반대 방향
			Vector3 friction = -mVelocity;
			friction.Normalize();
			friction = friction * mFriction * mMass * Time::DeltaTime();

			 //마찰력으로 인한 속도 감소량이 현재 속도보다 더 큰 경우
			if (mVelocity.Length() < friction.Length())
				 //속도를 0 로 만든다.
				mVelocity = Vector3::Zero;
			else
				 //속도에서 마찰력으로 인한 반대방향으로 속도를 차감한다.
				mVelocity += friction;
		}

		// 속도에 맞춰 물체를 이동시킨다.
		Vector3 velo;

		if (mbGround)
		{
			Vector3 dir = mVelocity;
			float length = dir.Length();

			dir.Normalize();
			dir = Vector3::Transform(dir, mRotateDirection);

			// 회전된 속도는 땅에서만 사용하므로 멤버 변수를 건드리지 않는다.
			velo = dir * length;
			mRotateDirection = {};
		}
		else
			velo = mVelocity;

		Vector3 pos = tr->GetPosition();
		pos += velo * Time::DeltaTime();
		tr->SetPosition(pos);

		ClearForce();
	}

	void Rigidbody::Render()
	{
	}

	void Rigidbody::AddForce(Vector3 force)
	{
		mForce += force;
	}
	void Rigidbody::ClearForce()
	{
		mForce = Vector3::Zero;
	}
}