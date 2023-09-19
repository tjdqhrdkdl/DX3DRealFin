#include "yaWallCheckScript.h"

#include "yaTime.h"

#include "yaGameObject.h"
#include "yaWallCheckObject.h"
#include "yaWallScript.h"

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
	void WallCheckScript::OnCollisionEnter(Collider2D* collider)
	{
		WallCheckObject* obj = (WallCheckObject*)GetOwner();
		GameObject* ownerObj = obj->GetParentObj();
		Transform* objTransform = ownerObj->GetComponent<Transform>();
		Rigidbody* objRigidbody = ownerObj->GetComponent<Rigidbody>();

		GameObject* colObj = collider->GetOwner();
		Transform* colTransform = colObj->GetComponent<Transform>();
		Rigidbody* colRigidbody = colObj->GetComponent<Rigidbody>();

		if (nullptr != colObj->GetScript<WallScript>())
		{
			// 충돌한 벽의 Right 벡터에 이동 벡터를 내적하여 투영된 벡터의 길이를 얻음
			Vector3 wallNormal = colTransform->Right();

			Vector3 objVelocity = objRigidbody->GetVelocity();
			Vector3 objPos = objTransform->GetPosition();

			float projLength = objVelocity.Dot(wallNormal);

			Vector3 projVelocity = wallNormal * projLength;

			//objVelocity += projVelocity;

			//objRigidbody->SetVelocity(projVelocity);

			objPos += projVelocity * Time::DeltaTime();
			objTransform->SetPosition(objPos);
		}
	}
	void WallCheckScript::OnCollisionStay(Collider2D* collider)
	{
		WallCheckObject* obj = (WallCheckObject*)GetOwner();
		GameObject* ownerObj = obj->GetParentObj();
		Transform* objTransform = ownerObj->GetComponent<Transform>();
		Rigidbody* objRigidbody = ownerObj->GetComponent<Rigidbody>();

		GameObject* colObj = collider->GetOwner();
		Transform* colTransform = colObj->GetComponent<Transform>();
		Rigidbody* colRigidbody = colObj->GetComponent<Rigidbody>();


		if (nullptr != colObj->GetScript<WallScript>())
		{
			// 충돌한 벽의 Right 벡터에 이동 벡터를 내적하여 투영된 벡터의 길이를 얻음
			Vector3 wallNormal = colTransform->Right();

			Vector3 objVelocity = objRigidbody->GetVelocity();
			Vector3 objPos = objTransform->GetPosition();

			float projLength = objVelocity.Dot(wallNormal);

			Vector3 projVelocity = wallNormal * projLength;

			objPos += projVelocity * Time::DeltaTime();
			objTransform->SetPosition(objPos);
		}
	}
	void WallCheckScript::OnCollisionExit(Collider2D* collider)
	{
	}
}
