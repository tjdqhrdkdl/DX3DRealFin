#include "yaLogBridgeScript.h"
#include "yaRigidbody.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaCollisionManager.h"
#include "yaTime.h"

ya::LogBridgeScript::LogBridgeScript()
{
}

ya::LogBridgeScript::~LogBridgeScript()
{
}

void ya::LogBridgeScript::Initalize()
{
}

void ya::LogBridgeScript::Update()
{
}

void ya::LogBridgeScript::Render()
{
}

void ya::LogBridgeScript::OnCollisionEnter(Collider2D* collider)
{
	GameObject* colObj = collider->GetOwner();
	Rigidbody* colRigidbody = colObj->GetComponent<Rigidbody>();

	if (colRigidbody != nullptr)
	{
		colRigidbody->SetGround(true);
		colRigidbody->SetLogBridge(true);
	}
}

void ya::LogBridgeScript::OnCollisionStay(Collider2D* collider)
{
	GameObject* colObj = collider->GetOwner();
	Transform* colTransform = colObj->GetComponent<Transform>();
	Rigidbody* colRigidbody = colObj->GetComponent<Rigidbody>();

	Vector3 direction = Vector3(0.f, -1.f, 0.f);
	std::vector<eLayerType> layers = {};
	layers.push_back(eLayerType::LogBridge);
	RayHit hit = CollisionManager::RayCast(colObj, direction, layers);
	if (hit.isHit)
	{
		Vector3 velocity = colRigidbody->GetVelocity();
		Vector3 pos = colTransform->GetPosition();

		pos -= velocity * Time::DeltaTime();
		colTransform->SetPosition(pos);
	}
}

void ya::LogBridgeScript::OnCollisionExit(Collider2D* collider)
{
	//GameObject* colObj = collider->GetOwner();
	//Transform* colTransform = colObj->GetComponent<Transform>();
	//Rigidbody* colRigidbody = colObj->GetComponent<Rigidbody>();

	//if (colRigidbody->IsLogBridge())
	//{
	//	Vector3 velocity = colRigidbody->GetVelocity();
	//	Vector3 pos = colTransform->GetPosition();

	//	pos -= velocity * Time::DeltaTime();
	//	colTransform->SetPosition(pos);
	//}

	GameObject* colObj = collider->GetOwner();
	Rigidbody* colRigidbody = colObj->GetComponent<Rigidbody>();

	if (colRigidbody != nullptr)
	{
		colRigidbody->SetGround(false);
		colRigidbody->SetLogBridge(false);
	}

}

void ya::LogBridgeScript::Start()
{
}

void ya::LogBridgeScript::Action()
{
}

void ya::LogBridgeScript::End()
{
}
