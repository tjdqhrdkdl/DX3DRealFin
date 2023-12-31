#pragma once
#include "yaEntity.h"
#include "yaLayer.h"
#include "yaGameObject.h"

namespace ya
{
	using namespace ya::enums;
	class Player;
	class Scene : public Entity
	{
	public:
		Scene(eSceneType type);
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
		virtual void Destroy();

		virtual void OnEnter();
		virtual void OnExit();

		eSceneType GetSceneType() { return mType; }
		void AddGameObject(GameObject* gameObj, const eLayerType type);
		Layer& GetLayer(eLayerType type) { return mLayers[(UINT)type]; }
		std::vector<GameObject*> GetDontDestroyGameObjects();
		const std::vector<GameObject*>& GetGameObjects(const eLayerType type);
		
		void SetPlayer(Player* player) { mPlayer = player; }
		Player* GetPlayer() { return mPlayer; }

		void SetThreadLoad(bool thread) { mbThreadLoad = thread; }
		bool IsThreadLoad() { return mbThreadLoad; }

		bool IsInitcomplete() { return mbInitComplete; }
		bool IsObjectsPosed() { return mbObjectsPosed; }

		std::function<void()>& GetCallBack() { return mInitCallBack; }

	private:
		std::vector<Layer> mLayers;
		eSceneType mType;
		Player* mPlayer;

		bool mbInitComplete;
		bool mbThreadLoad;						// 스레드로 로드할지 여부
		std::function<void()> mInitCallBack;	// initialize 완료 후 callback

		bool mbObjectsPosed;
	};
}
