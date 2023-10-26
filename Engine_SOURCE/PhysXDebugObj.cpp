#include "PhysXDebugObj.h"
#include "PhysXPlayerScript.h"
#include "PhysXManager.h"
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
		col3d->setType(eColliderType::Box);
		

		GameObject::Initialize();
	}

	void PhysXDebugObj::FixedUpdate()
	{
		GameObject::FixedUpdate();

		if (GetName() == L"testObj")
		{
			Transform* tr = GetComponent<Transform>();

			RaycastHit outhit{};
			PhysicsManager::raycast((UINT32)eLayerType::Ground, tr->GetWorldPosition(), -tr->Up(), 1000.f, &outhit);

			int k = 3;
		}
		
	}

}
