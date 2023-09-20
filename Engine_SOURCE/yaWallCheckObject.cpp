#include "yaWallCheckObject.h"
#include "yaTransform.h"
#include "yaWallCheckScript.h"

namespace ya
{
	ya::WallCheckObject::WallCheckObject()
		: mObj(nullptr)
	{
	}

	ya::WallCheckObject::~WallCheckObject()
	{
	}

	void ya::WallCheckObject::Initialize()
	{
		AddComponent<WallCheckScript>();

		GameObject::Initialize();
	}

	void ya::WallCheckObject::Update()
	{
		Transform* ownerTransform = mObj->GetComponent<Transform>();
		Transform* myTransform = GetComponent<Transform>();
		Vector3 pos = ownerTransform->GetPosition();
		
		myTransform->SetPosition(pos);

		GameObject::Update();
	}

	void ya::WallCheckObject::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void ya::WallCheckObject::Render()
	{
		GameObject::Render();
	}
}
