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

void ya::LogBridgeScript::Initialize()
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
		//colRigidbody->SetLogBridge(true);
	}
}

void ya::LogBridgeScript::OnCollisionStay(Collider2D* collider)
{
	GameObject* colObj = collider->GetOwner();
	Transform* colTransform = colObj->GetComponent<Transform>();
	Rigidbody* colRigidbody = colObj->GetComponent<Rigidbody>();

	Vector3 direction = Vector3(0.f, -1.f, 0.f);
	direction.Normalize();

	std::vector<eLayerType> layers = {};
	layers.push_back(eLayerType::Logbridge);

	Vector3 scale = colTransform->GetScale();

	// 절반의 크기를 크기가 1인 기저 벡터에 곱하여 계산
	Vector3 pos = colTransform->GetPosition();

	Vector3 xPos = colTransform->Right() * scale / 2.f;
	Vector3 zPos = colTransform->Forward() * scale / 2.f;

	// 방향좀 이상함
	Vector3 front = pos - zPos;
	Vector3 right = pos - xPos;
	Vector3 back = pos + zPos;
	Vector3 left = pos + xPos;

	RayHit multiHit[4] = {};

	multiHit[0] = CollisionManager::RayCast(colObj, front, direction, layers);
	multiHit[1] = CollisionManager::RayCast(colObj, right, direction, layers);
	multiHit[2] = CollisionManager::RayCast(colObj, back, direction, layers);
	multiHit[3]= CollisionManager::RayCast(colObj, left, direction, layers);

	for (int i = 0; i < 4; ++i)
	{
		if (!multiHit[i].isHit)
		{
			//Vector3 frontDirection = Vector3(0.f, -0.5f, 1.f);
			//frontDirection.Normalize();

			Vector3 frontDirection = colTransform->Forward();
			frontDirection += Vector3(0.f, -0.5f, 0.f);
			frontDirection.Normalize();

			RayHit distCheck = CollisionManager::RayCast(colObj, pos, frontDirection, layers);

			float distance = distCheck.length;//distCheck.contact.Length() - pos.Length();
			distance = abs(distance);

			if (distance <= scale.z / 2.f)
			{
				Vector3 velocity = colRigidbody->GetVelocity();
				Vector3 pos = colTransform->GetPosition();

				pos -= velocity * Time::DeltaTime();
				colTransform->SetPosition(pos);
			}

			break;
		}
	}
}

void ya::LogBridgeScript::OnCollisionExit(Collider2D* collider)
{
	GameObject* colObj = collider->GetOwner();
	Transform* colTransform = colObj->GetComponent<Transform>();
	Rigidbody* colRigidbody = colObj->GetComponent<Rigidbody>();

	//if (colRigidbody->IsLogBridge())
	//{
	//	Vector3 velocity = colRigidbody->GetVelocity();
	//	Vector3 pos = colTransform->GetPosition();

	//	pos -= velocity * Time::DeltaTime();
	//	colTransform->SetPosition(pos);
	//}

	//GameObject* colObj = collider->GetOwner();
	//Rigidbody* colRigidbody = colObj->GetComponent<Rigidbody>();

	if (colRigidbody != nullptr)
	{
		colRigidbody->SetGround(false);
		//colRigidbody->SetLogBridge(false);
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
