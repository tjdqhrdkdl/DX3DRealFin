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
		virtual void Render() override;
		virtual void FixedUpdate() override;


		void PushBackObject(GameObject* obj) { mGameObjects.push_back(obj); }
		void SetObjectsVector(std::vector<GameObject*> vec) { mGameObjects = vec; }

		void SetParent();
		void SetChildRender(bool render);
	private:
		std::vector<GameObject*> mGameObjects;
	};
}
