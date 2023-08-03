#include "yaGrappleHookScript.h"

#include "yaTime.h"

#include "yaRigidbody.h"
#include "yaTransform.h"

#include <assert.h>

namespace ya
{
	GrappleHookScript::GrappleHookScript()
		: Script()
		, mGrappleHookTarget(nullptr)
		, mSpeed(100.0f)
		, mDirection(Vector3::Zero)
		, mDistance(0.0f)
		, mCurrentDistance(0.0f)
		, mbGrappleHook(false)
	{
	}
	GrappleHookScript::~GrappleHookScript()
	{
	}
	void GrappleHookScript::Initialize()
	{
	}

	void GrappleHookScript::Update()
	{
		if (mbGrappleHook)
		{
			if (mCurrentDistance > 4.0f)
			{
				GameObject* owner = GetOwner();

				Rigidbody* rigidbody = owner->GetComponent<Rigidbody>();
				//rigidbody->SetGround(false);

				Transform* tr = owner->GetComponent<Transform>();
				Vector3 pos = tr->GetPosition();

				float speed = mSpeed * Time::DeltaTime();

				pos += mDirection * speed;
				tr->SetPosition(pos);

				mCurrentDistance -= speed;
			}
			else
			{
				mbGrappleHook = false;
			}
		}
	}

	void GrappleHookScript::FixedUpdate()
	{
	}

	void GrappleHookScript::Render()
	{
	}

	void GrappleHookScript::GrappleHook()
	{
		if (mGrappleHookTarget == nullptr)
		{
			return;
		}

		mbGrappleHook = true;

		Transform* targetTr = mGrappleHookTarget->GetComponent<Transform>();
		mHookTargetPosition = targetTr->GetPosition() + Vector3(0.0f, 15.0f, 0.0f);

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		
		// 거리 구하기
		mDistance = pos.Distance(pos, mHookTargetPosition);
		mCurrentDistance = mDistance;

		// 방향 구하기
		Vector3 dir = mHookTargetPosition - pos;
		dir.Normalize();
		mDirection = dir;

		tr->SetRotation(mDirection);
	}
}