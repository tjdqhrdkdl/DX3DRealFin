#pragma once

#include "yaGameObject.h"
#include "yaTransform.h"

namespace ya
{
    class WallCheckObject : public GameObject
    {
	public:
		WallCheckObject();
		virtual ~WallCheckObject();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
		GameObject* mObj;

	public:
		void SetParentObj(GameObject* obj) { mObj = obj; }
		GameObject* GetParentObj() { return mObj; }
    };
}

