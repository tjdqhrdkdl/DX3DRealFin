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

		GameObject::Update();
	}
	void MeshObject::SetParent()
	{
		Transform* tr = GetComponent<Transform>();
		for (size_t i = 0; i < mGameObjects.size(); i++)
		{
			GameObject* object = mGameObjects[i];
			Transform* objTr = object->GetComponent<Transform>();
			objTr->SetParent(tr);
		}
	}
}