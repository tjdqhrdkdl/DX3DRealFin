#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Layer : public Entity
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void Initialize();
		virtual void Update();
		void CollisionUpdate();
		void CollisionLateUpdate();
		virtual void FixedUpdate();

		virtual void Render();
		virtual void FrameEnd();
		virtual void Destroy();

		//씬이 종료되었을 떄 상태를 초기화하기 위한 함수
		void Reset();

		void AddGameObject(GameObject* gameObject);
		const std::vector<GameObject*>& GetGameObjects() { return mGameObjects; }
		std::vector<GameObject*> GetDontDestroyGameObjects();


	private:
		std::vector<GameObject*> mGameObjects;
	};

	typedef const std::vector<GameObject*>& GameObjects;
	typedef std::vector<GameObject*>::iterator GameObjectIter;
}
