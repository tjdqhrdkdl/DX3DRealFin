#include "yaGrappleHookScript.h"

#include "yaTime.h"

#include "yaRigidbody.h"
#include "yaTransform.h"
#include "yaCollisionManager.h"

#include <assert.h>


#include "yaPlayer.h"
#include "yaApplication.h"
#include "yaInput.h"

extern ya::Application application;

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
		Player* player = dynamic_cast<Player*>(GetOwner());

		GameObject* camera = player->GetCamera();
		Transform* cameraTr = camera->GetComponent<Transform>();
		Vector3 cameraPos = cameraTr->GetPosition();


		// find hook target
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		std::vector<eLayerType> layers;
		layers.push_back(eLayerType::Hook);
		RayHit hit = CollisionManager::RayCast(camera, cameraPos, cameraTr->Forward(), layers);
		//RayHit hit = CollisionManager::RayCast(GetOwner(), pos, tr->Forward(), layers);

		GrappleHookScript* action = player->GetScript<GrappleHookScript>();
		if (hit.isHit)
		{
			action->SetGrappleHookTarget(hit.hitObj);
		}
		else
		{
			action->SetGrappleHookTarget(nullptr);
		}


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
		if (mGrappleHookTarget != nullptr)
		{
			wchar_t szFloat[50] = {};
			swprintf_s(szFloat, 50, L"target on");
			TextOut(application.GetHdc(), 800, 150, szFloat, wcslen(szFloat));
		}
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