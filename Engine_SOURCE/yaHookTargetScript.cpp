#include "yaHookTargetScript.h"
#include "yaGrappleHookScript.h"
#include "yaRigidbody.h"
#include "yaPlayer.h"
#include "yaSceneManager.h"
#include "yaScene.h"
#include "yaPlayer.h"
#include "yaGrappleHookScript.h"
#include "yaTransform.h"

namespace ya
{
	HookTargetScript::HookTargetScript()
		: Script()
		, mbChecked(false)
	{
	}

	HookTargetScript::~HookTargetScript()
	{
	}

	void HookTargetScript::Initialize()
	{
	}

	void HookTargetScript::Update()
	{
		Scene* scene = GetOwner()->GetScene();
		Player* player = scene->GetPlayer();
		if (player == nullptr)
			return;

		Transform* playerTr = player->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetLocalPosition();

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetLocalPosition();

		// 거리
		float dist = playerPos.Distance(playerPos, pos);

		// 각도
		Quaternion quater = Quaternion::FromToRotation(-playerTr->Forward(), Vector3(playerPos.x - pos.x, playerPos.y - pos.y, playerPos.z - pos.z));
		Vector3 quaterToEuler = quater.ToEuler();
		Vector3 theta = quaterToEuler * 180.0f / XM_PI;

		GrappleHookScript* hookScript = player->GetScript<GrappleHookScript>();

		
		if (dist <= 400.0f)
		{
			if (abs(theta.y) <= 60.0f)
			{
				if (!mbChecked)
				{
					mbChecked = true;
					hookScript->AddHookTargetCount();
					hookScript->SetHookTarget(GetOwner(), dist);
				}
			}
			else
			{
				if (mbChecked)
				{
					mbChecked = false;
					hookScript->SubHookTargetCount();
				}
			}
		}
		else
		{
			if (mbChecked)
			{
				mbChecked = false;
				hookScript->SubHookTargetCount();
			}
		}
	}

	void HookTargetScript::Render()
	{
	}

	void HookTargetScript::OnCollisionEnter(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		Rigidbody* rigidbody = _otherObj->GetComponent<Rigidbody>();

		GrappleHookScript* grap = _otherObj->GetScript<GrappleHookScript>();
		if (grap != nullptr)
		{
			grap->SetGrappleHook(false);
		}
		
	}

	void HookTargetScript::OnCollisionStay(GameObject* _otherObj, const Vector3& _hitPoint)
	{
	}

	void HookTargetScript::OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint)
	{
	}
}
