#include "yaActionScript.h"

#include "yaTime.h"

#include "yaRigidbody.h"
#include "yaTransform.h"

#include <assert.h>

namespace ya
{
	const float defaultJumpForce = 400.0f;

	ActionScript::ActionScript()
		: Script()
		, mRigidbody(nullptr)
		, mTransform(nullptr)
		, mSpeed(100.0f)
		, mDirection(Vector3::Zero)
		, mJumpTimer(0.0f)
		, mJumpForce(0.0f)
		, mTarget(nullptr)
	{
	}
	ActionScript::~ActionScript()
	{
	}
	void ActionScript::Initialize()
	{
		GameObject* obj = GetOwner();
		assert(obj != nullptr);

		mTransform = obj->GetComponent<Transform>();
		mRigidbody = obj->GetComponent<Rigidbody>();
	}

	void ActionScript::Update()
	{
		assert(GetOwner() != nullptr);

		if (mTransform == nullptr || mRigidbody == nullptr)
		{
			return;
		}

		if (mJumpTimer > 0.0f)
		{
			mJumpTimer -= Time::DeltaTime();
			mRigidbody->AddForce(Vector3(0.0f, mJumpForce, 0.0f));
		}
	}

	void ActionScript::FixedUpdate()
	{
	}

	void ActionScript::Render()
	{
	}

	/// <summary>
	/// owner rigidbody를 통한 이동
	/// </summary>
	/// <param name="dir">방향</param>
	/// <param name="speed">속도</param>
	void ActionScript::Move(const Vector3 dir, float speed)
	{
		if (mRigidbody == nullptr)
		{
			assert(mRigidbody != nullptr);
			return;
		}

		if (speed > 0.0f)
			mRigidbody->AddForce(speed * dir);
		else
			mRigidbody->AddForce(mSpeed * dir);
		
	}

	/// <summary>
	/// owner rigidbody를 통한 회전
	/// </summary>
	/// <param name="dir">방향</param>
	/// <param name="speed">속도</param>
	void ActionScript::Rotate(const Vector3 dir, float speed)
	{
		if(mTransform == nullptr)
		{
			assert(mTransform != nullptr);
			return;
		}

		Vector3 rot = mTransform->GetRotation();
		
		if (speed > 0.0f)
		{
			rot += speed * 4.0f * dir * Time::DeltaTime();
		}
		else
		{
			rot += mSpeed * 4.0f * dir * Time::DeltaTime();
		}

		mTransform->SetRotation(rot);
	}

	/// <summary>
	/// rigidbody를 통한 점프
	/// </summary>
	/// <param name="force">점프 force</param>
	void ActionScript::Jump(float force)
	{
		if (mRigidbody == nullptr)
		{
			assert(mRigidbody != nullptr);
			return;
		}

		// force가 지정되었을 때(음수가 아닌경우) force로 사용
		if (force > 0.0f)
		{
			mJumpForce = force;
		}
		// force가 지정되지 않았을 때 기존의 force 사용
		else
		{
			mJumpForce = defaultJumpForce;
		}

		if (mRigidbody->IsGround())
		{
			mRigidbody->SetGround(false);
			mJumpTimer = 0.1f;
		}
	}

	/// <summary>
	/// 공격
	/// </summary>
	void ActionScript::Attack()
	{
	}

	/// <summary>
	/// 막기
	/// </summary>
	void ActionScript::Deflect()
	{
	}

	/// <summary>
	/// 패링
	/// </summary>
	void ActionScript::Parrying()
	{
	}
}