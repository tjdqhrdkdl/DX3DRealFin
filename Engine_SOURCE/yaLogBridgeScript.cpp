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

void ya::LogBridgeScript::OnCollisionEnter(GameObject* _otherObj, const Vector3& _hitPoint)
{
	Rigidbody* colRigidbody = _otherObj->GetComponent<Rigidbody>();

	if (colRigidbody != nullptr)
	{
		//colRigidbody->SetGround(true);
		//colRigidbody->SetLogBridge(true);
	}
}

void ya::LogBridgeScript::OnCollisionStay(GameObject* _otherObj, const Vector3& _hitPoint)
{
	Transform* colTransform = _otherObj->GetComponent<Transform>();
	Rigidbody* colRigidbody = _otherObj->GetComponent<Rigidbody>();

	Vector3 direction = Vector3(0.f, -1.f, 0.f);
	direction.Normalize();

	//std::vector<eLayerType> layers = {};
	//layers.push_back(eLayerType::Logbridge);

	Vector3 scale = colTransform->GetLocalScale();

	// 절반의 크기를 크기가 1인 기저 벡터에 곱하여 계산
	Vector3 pos = colTransform->GetLocalPosition();

	Vector3 xPos = colTransform->Right() * scale / 2.f;
	Vector3 zPos = colTransform->Forward() * scale / 2.f;

	// 방향좀 이상함
	Vector3 front = pos - zPos;
	Vector3 right = pos - xPos;
	Vector3 back = pos + zPos;
	Vector3 left = pos + xPos;

	RaycastHit multiHit[4] = {};

	CollisionManager::Raycast(_otherObj->GetLayerType(), front, direction, 10000.f, &multiHit[0]);
	CollisionManager::Raycast(_otherObj->GetLayerType(), front, direction, 10000.f, &multiHit[1]);
	CollisionManager::Raycast(_otherObj->GetLayerType(), front, direction, 10000.f, &multiHit[2]);
	CollisionManager::Raycast(_otherObj->GetLayerType(), front, direction, 10000.f, &multiHit[3]);
	//multiHit[0] = CollisionManager::RayCast(_otherObj, front, direction, layers);
	//multiHit[1] = CollisionManager::RayCast(_otherObj, right, direction, layers);
	//multiHit[2] = CollisionManager::RayCast(_otherObj, back, direction, layers);
	//multiHit[3]= CollisionManager::RayCast(_otherObj, left, direction, layers);

	for (int i = 0; i < 4; ++i)
	{
		if (!multiHit[i].gameObject)
		{
			//Vector3 frontDirection = Vector3(0.f, -0.5f, 1.f);
			//frontDirection.Normalize();

			Vector3 frontDirection = colTransform->Forward();
			frontDirection += Vector3(0.f, -0.5f, 0.f);
			frontDirection.Normalize();

			RaycastHit distCheck{};
			CollisionManager::Raycast(_otherObj->GetLayerType(), pos, frontDirection, 10000.f, &distCheck);

			float distance = distCheck.hitDistance;//distCheck.contact.Length() - pos.Length();
			distance = abs(distance);

			if (distance <= scale.z / 2.f)
			{
				Vector3 velocity = colRigidbody->GetVelocity();
				Vector3 pos = colTransform->GetLocalPosition();

				pos -= velocity * Time::DeltaTime();
				colTransform->SetLocalPosition(pos);
			}

			break;
		}
	}
}

void ya::LogBridgeScript::OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint)
{
	Transform* colTransform = _otherObj->GetComponent<Transform>();
	Rigidbody* colRigidbody = _otherObj->GetComponent<Rigidbody>();

	//if (colRigidbody->IsLogBridge())
	//{
	//	Vector3 velocity = colRigidbody->GetVelocity();
	//	Vector3 pos = colTransform->GetLocalPosition();

	//	pos -= velocity * Time::DeltaTime();
	//	colTransform->SetLocalPosition(pos);
	//}

	//GameObject* colObj = collider->GetOwner();
	//Rigidbody* colRigidbody = colObj->GetComponent<Rigidbody>();

	if (colRigidbody != nullptr)
	{
		//colRigidbody->SetGround(false);
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
