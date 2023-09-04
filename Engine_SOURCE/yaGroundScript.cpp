#include "yaGroundScript.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaAnimator.h"

#include "yaRigidbody.h"

namespace ya
{
	GroundScript::GroundScript()
		: Script()
	{
	}

	GroundScript::~GroundScript()
	{
	}

	void GroundScript::Initialize()
	{
		
	}

	void GroundScript::Update()
	{
		int a = 0;
	}

	void GroundScript::Render()
	{
	}

	void GroundScript::OnCollisionEnter(Collider2D* collider)
	{
		GameObject* obj = collider->GetOwner();
		Rigidbody* rigidbody = obj->GetComponent<Rigidbody>();

		if(rigidbody != nullptr)
		{
			rigidbody->SetGround(true);
		}
	}

	void GroundScript::OnCollisionStay(Collider2D* collider)
	{
		GameObject* obj = collider->GetOwner();
		Rigidbody* rigidbody = obj->GetComponent<Rigidbody>();

		if (rigidbody != nullptr)
		{
			//rigidbody->SetGround(true);
		}
	}

	void GroundScript::OnCollisionExit(Collider2D* collider)
	{
		GameObject* obj = collider->GetOwner();
		Rigidbody* rigidbody = obj->GetComponent<Rigidbody>();

		if (rigidbody != nullptr)
		{
			//rigidbody->SetGround(false);
		}
	}

	void GroundScript::Start()
	{
	}

	void GroundScript::Action()
	{
	}

	void GroundScript::End()
	{
	}

}
