#include "yaHangingScript.h"
#include "yaRigidbody.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaCollisionManager.h"
#include "yaHangingObjectScript.h"
#include "yaInput.h"
#include "PhysXWrapper.h"

ya::HangingScript::HangingScript()
	:	mbHanging(false)
{
}

ya::HangingScript::~HangingScript()
{
}

void ya::HangingScript::InitHanging()
{
	Rigidbody* rigidbody = GetOwner()->GetComponent<Rigidbody>();

	rigidbody->SetVelocity(Vector3::Zero);

	mbHanging = true;
}

void ya::HangingScript::DoHanging(GameObject* _otherObj, const Vector3& _hitPoint)
{
	GameObject* obj = GetOwner();

	Transform* objTransform = GetOwner()->GetComponent<Transform>();
	Transform* colTransform = _otherObj->GetComponent<Transform>();
	
	if (!objTransform)
		assert(nullptr);

	Vector3 objPos = objTransform->GetLocalPosition();
	Vector3 colPos = colTransform->GetLocalPosition();
	
	Vector3 direction = objTransform->Forward();

	//std::vector<eLayerType> layers = {};
	//layers.push_back(eLayerType::HangingObject);

	//RayHit Hit = CollisionManager::RayCast(obj, objPos, direction, layers);


	RaycastHit hit{};
	

	CollisionManager::Raycast(eLayerType::HangingObject, objPos, direction, 10000.f, &hit);


	if (hit.gameObject)
	{
		float distance = hit.hitDistance;

		objPos.y = colPos.y;
		objPos.z += distance;
	}
}

void ya::HangingScript::Initialize()
{
}

void ya::HangingScript::Update()
{
}

void ya::HangingScript::Render()
{
}

void ya::HangingScript::OnCollisionEnter(GameObject* _otherObj, const Vector3& _hitPoint)
{
	HangingObjectScript* hanging = _otherObj->GetScript<HangingObjectScript>();

	if (Input::GetKey(eKeyCode::F) && !hanging)
	{
		InitHanging();
		DoHanging(_otherObj, _hitPoint);
	}
}

void ya::HangingScript::OnCollisionStay(GameObject* _otherObj, const Vector3& _hitPoint)
{
	HangingObjectScript* hanging = _otherObj->GetScript<HangingObjectScript>();

	if (Input::GetKey(eKeyCode::F) && !hanging)
	{
		InitHanging();
		DoHanging(_otherObj, _hitPoint);
	}

	else if (mbHanging)
		DoHanging(_otherObj, _hitPoint);

	//XMQuaternionAng
}

void ya::HangingScript::OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint)
{
}
