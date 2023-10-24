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
		Collider3D* col3d = AddComponent<Collider3D>();
		col3d->setType(eColliderType::Box);
		AddComponent<PhysXPlayerScript>();
	}

}
