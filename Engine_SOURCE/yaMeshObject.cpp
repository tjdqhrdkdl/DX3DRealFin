#include "yaMeshObject.h"


namespace ya
{
	MeshObject::MeshObject()
	{
	}
	MeshObject::~MeshObject()
	{
	}
	void MeshObject::Update()
	{
		Transform* tr = GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		Vector3 rot = tr->GetRotation();
		Vector3 scale = tr->GetScale();
		for (size_t i = 0; i < mGameObjects.size(); i++)
		{
			GameObject* object = mGameObjects[i];
			Transform* objTr = object->GetComponent<Transform>();
			objTr->SetSubPosition(pos);
			objTr->SetSubRotation(rot);
			objTr->SetSubScale(scale);
		}
		GameObject::Update();
	}
}