#include "yaWallCheckScript.h"

#include "yaTime.h"

#include "yaGameObject.h"
#include "yaWallCheckObject.h"
#include "yaWallScript.h"
#include "yaActionScript.h"

#include "yaTransform.h"
#include "yaRigidbody.h"

namespace ya
{
	WallCheckScript::WallCheckScript()
	{
	}
	WallCheckScript::~WallCheckScript()
	{
	}
	void WallCheckScript::Initialize()
	{
	}
	void WallCheckScript::Update()
	{
	}
	void WallCheckScript::FixedUpdate()
	{
	}
	void WallCheckScript::Render()
	{
	}
	void WallCheckScript::OnCollisionEnter(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		WallCheckObject* obj = (WallCheckObject*)GetOwner();
		Transform* checkTransform = obj->GetComponent<Transform>();

		GameObject* ownerObj = obj->GetParentObj();
		Transform* objTransform = ownerObj->GetComponent<Transform>();
		Rigidbody* objRigidbody = ownerObj->GetComponent<Rigidbody>();
		ActionScript* objActionScript = ownerObj->GetScript<ActionScript>();

		Transform* colTransform = _otherObj->GetComponent<Transform>();

		if (nullptr != _otherObj->GetScript<WallScript>())
		{

			Vector3 objVelocity = objRigidbody->GetVelocity();
			Vector3 objPos = objTransform->GetLocalPosition();

			Vector3 wallNormal = colTransform->Right();

			objRigidbody->SetRightWallDir(wallNormal);
			objRigidbody->SetForwardBlocked(true);

			if (objRigidbody->IsJumping())
			{
				objRigidbody->SetVelocity(Vector3::Zero);
			}

			if (!objRigidbody->IsJumping())
			{
				objPos -= objVelocity * Time::DeltaTime();

				objTransform->SetLocalPosition(objPos);
				checkTransform->SetLocalPosition(objPos);
			}
		}
	}
	void WallCheckScript::OnCollisionStay(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		WallCheckObject* obj = (WallCheckObject*)GetOwner();
		Transform* checkTransform = obj->GetComponent<Transform>();

		GameObject* ownerObj = obj->GetParentObj();
		Transform* objTransform = ownerObj->GetComponent<Transform>();
		Rigidbody* objRigidbody = ownerObj->GetComponent<Rigidbody>();
		ActionScript* objActionScript = ownerObj->GetScript<ActionScript>();

		Transform* colTransform = _otherObj->GetComponent<Transform>();

		if (nullptr != _otherObj->GetScript<WallScript>())
		{
			Vector3 objVelocity = objRigidbody->GetVelocity();
			Vector3 objPos = objTransform->GetLocalPosition();

			Vector3 wallNormal = colTransform->Right();

			objRigidbody->SetRightWallDir(wallNormal);
		}
	}
	void WallCheckScript::OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		WallCheckObject* ownerObj = (WallCheckObject*)GetOwner();
		Transform* ownerTransform = ownerObj->GetComponent<Transform>();

		GameObject* obj = ownerObj->GetParentObj();
		Rigidbody* objRigidbody = obj->GetComponent<Rigidbody>();

		objRigidbody->SetForwardBlocked(false);
	}
}
