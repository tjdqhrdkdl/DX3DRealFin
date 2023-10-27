#include "PhysXDebugObj.h"
#include "PhysXPlayerScript.h"
#include "yaCollisionManager.h"
#include "PhysXWrapper.h"

namespace ya
{
	PhysXDebugObj::PhysXDebugObj()
	{
	}

	PhysXDebugObj::~PhysXDebugObj()
	{
	}

	void PhysXDebugObj::Initialize()
	{
		Transform* tr = GetComponent<Transform>();
		tr->SetLocalScale(Vector3(100.f));

		Collider3D* col3d = AddComponent<Collider3D>();
		col3d->SetType(eColliderType::Box);
		

		GameObject::Initialize();
	}

	void PhysXDebugObj::FixedUpdate()
	{
		GameObject::FixedUpdate();

		if (GetName() == L"testObj")
		{
			Transform* tr = GetComponent<Transform>();

			RaycastHit outhit{};
			CollisionManager::Raycast(eLayerType::Ground, tr->GetWorldPosition(), tr->Forward(), 1000.f, &outhit);

			int k = 3;
		}
		
	}

}
