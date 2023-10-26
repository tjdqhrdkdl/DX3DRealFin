#include "PhysXDebugObj.h"
#include "PhysXPlayerScript.h"

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

}
