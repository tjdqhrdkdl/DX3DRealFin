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
	void MeshObject::Render()
	{
		GameObject::Render();

	}
	void MeshObject::FixedUpdate()
	{
		GameObject::FixedUpdate();

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
	void MeshObject::SetChildRender(bool render)
	{
		for (size_t i = 0; i < mGameObjects.size(); i++)
		{
			mGameObjects[i]->SetRender(render);

		}
	}
}