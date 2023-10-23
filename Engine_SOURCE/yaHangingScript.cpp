#include "yaHangingScript.h"
#include "yaRigidbody.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaCollisionManager.h"
#include "yaHangingObjectScript.h"
#include "yaInput.h"

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

void ya::HangingScript::DoHanging(Collider2D* collider)
{
	GameObject* obj = GetOwner();
	GameObject* colObj = collider->GetOwner();

	Transform* objTransform = GetOwner()->GetComponent<Transform>();
	Transform* colTransform = collider->GetOwner()->GetComponent<Transform>();
	
	if (!objTransform)
		assert(nullptr);

	Vector3 objPos = objTransform->GetLocalPosition();
	Vector3 colPos = colTransform->GetLocalPosition();
	
	Vector3 direction = objTransform->Forward();

	std::vector<eLayerType> layers = {};
	layers.push_back(eLayerType::HangingObject);

	RayHit Hit = CollisionManager::RayCast(obj, objPos, direction, layers);

	if (Hit.isHit)
	{
		float distance = Hit.length;

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

void ya::HangingScript::OnCollisionEnter(Collider2D* collider)
{
	GameObject* colObj = collider->GetOwner();
	HangingObjectScript* hanging = colObj->GetScript<HangingObjectScript>();

	if (Input::GetKey(eKeyCode::F) && !hanging)
	{
		InitHanging();
		DoHanging(collider);
	}
}

void ya::HangingScript::OnCollisionStay(Collider2D* collider)
{
	GameObject* colObj = collider->GetOwner();
	HangingObjectScript* hanging = colObj->GetScript<HangingObjectScript>();

	if (Input::GetKey(eKeyCode::F) && !hanging)
	{
		InitHanging();
		DoHanging(collider);
	}

	else if (mbHanging)
		DoHanging(collider);

	//XMQuaternionAng
}

void ya::HangingScript::OnCollisionExit(Collider2D* collider)
{
}
