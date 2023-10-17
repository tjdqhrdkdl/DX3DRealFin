#include "yaGrappleHookScript.h"

#include "yaTime.h"

#include "yaRigidbody.h"
#include "yaTransform.h"
#include "yaCollisionManager.h"

#include "yaPlayer.h"
#include "yaPlayerMeshScript.h"

#include "yaApplication.h"
#include "yaInput.h"

extern ya::Application application;

namespace ya
{
	GrappleHookScript::GrappleHookScript()
		: Script()
		, mPlayer(nullptr)
		, mPlayerAnim(nullptr)
		, mHookTarget(nullptr)
		, mSpeed(60.0f)
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
		mPlayer = dynamic_cast<Player*>(GetOwner());
		mPlayerAnim = mPlayer->GetScript<PlayerMeshScript>();
	}

	void GrappleHookScript::Update()
	{
		/*GameObject* camera = mPlayer->GetCamera();
		Transform* cameraTr = camera->GetComponent<Transform>();
		Vector3 cameraPos = cameraTr->GetPosition();*/

		if (Input::GetKey(eKeyCode::F))
		{
			GrappleHookScript* grap = GetOwner()->GetScript<GrappleHookScript>();
			if (grap != nullptr)
			{
				grap->GrappleHook();
			}
		}

		if (mbGrappleHook)
		{
			if (mCurrentDistance < mDistance)
			{
				Transform* tr = mPlayer->GetComponent<Transform>();
				Vector3 pos = tr->GetPosition();

				float speed = mSpeed * Time::DeltaTime();
				pos += mDirection * speed;
				mCurrentDistance += speed;
				
				float height = pos.y + 0.2f + ((-mCurrentDistance * mCurrentDistance + mDistance * mCurrentDistance) / (mDistance * mDistance * 1000.0f));
				tr->SetPosition(Vector3(pos.x, height, pos.z));
			}
			else
			{
				mbGrappleHook = false;
				mPlayer->SetStateFlag(ePlayerState::Hook, false);
			}
		}
	}

	void GrappleHookScript::FixedUpdate()
	{
	}

	void GrappleHookScript::Render()
	{
		/*wchar_t szFloat[50] = {};
		swprintf_s(szFloat, 50, (mPlayer->IsStateFlag(ePlayerState::Hook)) ? L"true" : L"false");
		TextOut(application.GetHdc(), 800, 150, szFloat, wcslen(szFloat));*/

		//if (mHookTarget != nullptr)
		//{
		//	wchar_t szFloat[50] = {};
		//	//swprintf_s(szFloat, 50, L"target on");
		//	swprintf_s(szFloat, 50, (mPlayer->IsStateFlag(ePlayerState::Hook))?L"true":L"false");
		//	TextOut(application.GetHdc(), 800, 150, szFloat, wcslen(szFloat));
		//}
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
		mPlayer->SetStateFlag(ePlayerState::Hook, true);

		Rigidbody* rigidbody = mPlayer->GetComponent<Rigidbody>();

		if (rigidbody->IsGrounded())
		{
			mPlayerAnim->Play(L"a000_202000");
		}
		else
		{
			mPlayerAnim->Play(L"a000_202100");
		}

		Transform* playerTr = mPlayer->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();

		Transform* targetTr = mHookTarget->GetComponent<Transform>();
		mHookTargetPosition = targetTr->GetPosition();

		// 거리 구하기
		Vector3 hookVector = mHookTargetPosition - playerPos;
		mDistance = playerPos.Distance(playerPos, mHookTargetPosition);
		mCurrentDistance = 0.0f;

		// 방향 구하기
		Vector3 dir = mHookTargetPosition - playerPos;
		dir.Normalize();
		mDirection = dir;

		playerTr->SetRotation(mDirection);
	}
}