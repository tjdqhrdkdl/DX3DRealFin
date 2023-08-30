#pragma once
#include "yaGameObject.h"


namespace ya
{
	class MeshObject : public GameObject
	{
	public:
		MeshObject();
		virtual ~MeshObject();

		virtual void Update() override;


		void PushBackObject(GameObject* obj) { mGameObjects.push_back(obj); }
		void SetObjectsVector(std::vector<GameObject*> vec) { mGameObjects = vec; }

		void SetParent();
	private:
		std::vector<GameObject*> mGameObjects;
	};
}
