#include "yaGrappleHookScript.h"

#include "yaTime.h"

#include "yaRigidbody.h"
#include "yaTransform.h"
#include "yaCollisionManager.h"

#include "yaPlayer.h"
#include "yaApplication.h"
#include "yaInput.h"

extern ya::Application application;

namespace ya
{
	GrappleHookScript::GrappleHookScript()
		: Script()
		, mHookTarget(nullptr)
		, mSpeed(160.0f)
		, mDirection(Vector3::Zero)
		, mDistance(0.0f)
		, mCurrentDistance(0.0f)
		, mbGrappleHook(false)
		, mHookTargetDistance(1000.0f)
		
		, mHookTargetCount(0)

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
		Player* player = dynamic_cast<Player*>(GetOwner());

		GameObject* camera = player->GetCamera();
		Transform* cameraTr = camera->GetComponent<Transform>();
		Vector3 cameraPos = cameraTr->GetPosition();

		if (mbGrappleHook)
		{
			if (mCurrentDistance > 2.0f)
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
		/*if (mHookTarget != nullptr)
		{
			wchar_t szFloat[50] = {};
			swprintf_s(szFloat, 50, L"target on");
			TextOut(application.GetHdc(), 800, 150, szFloat, wcslen(szFloat));
		}*/
	}

	void GrappleHookScript::SetHookTarget(GameObject* target, float distance)
	{
		if (mHookTargetDistance > distance)
		{
			mHookTargetDistance = distance;
			mHookTarget = target;
		}
	}

	void GrappleHookScript::SubHookTargetCount()
	{
		mHookTargetCount--;

		if(mHookTargetCount <= 0)
		{
			mHookTarget = nullptr;
			mHookTargetDistance = 1000.0f;
		}
	}

	void GrappleHookScript::GrappleHook()
	{
		if (mHookTarget == nullptr)
		{
			return;
		}

		mbGrappleHook = true;

		Transform* targetTr = mHookTarget->GetComponent<Transform>();
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