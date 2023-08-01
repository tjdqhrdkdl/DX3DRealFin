#include "yaHookTargetScript.h"
#include "yaGrappleHookScript.h"
#include "yaRigidbody.h"

namespace ya
{
	HookTargetScript::HookTargetScript()
		: Script()
	{
	}

	HookTargetScript::~HookTargetScript()
	{
	}

	void HookTargetScript::Initalize()
	{
	}

	void HookTargetScript::Update()
	{
	}

	void HookTargetScript::Render()
	{
	}

	void HookTargetScript::OnCollisionEnter(Collider2D* collider)
	{
		GameObject* obj = collider->GetOwner();
		Rigidbody* rigidbody = obj->GetComponent<Rigidbody>();

		GrappleHookScript* grap = obj->GetScript<GrappleHookScript>();
		if (grap != nullptr)
		{
			grap->SetGrappleHook(false);
			//grap.set
		}
		
	}

	void HookTargetScript::OnCollisionStay(Collider2D* collider)
	{
	}

	void HookTargetScript::OnCollisionExit(Collider2D* collider)
	{
	}
}
