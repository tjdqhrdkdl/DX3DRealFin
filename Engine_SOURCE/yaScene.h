#pragma once
#include "yaEntity.h"
#include "yaLayer.h"
#include "yaGameObject.h"

namespace ya
{
	using namespace ya::enums;

	enum class eLoadStatus
	{
		NotLoaded,
		Loading,
		LoadComplete
	};

	class Player;
	class Scene : public Entity
	{
	public:
		Scene(eSceneType type);
		virtual ~Scene();


		virtual void Initialize();
		void ThreadedInitialize();

		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		virtual void Destroy();

		virtual void OnEnter();
		virtual void OnExit();

		void CreatePhysXScene();

		eSceneType GetSceneType() { return mType; }
		void AddGameObject(GameObject* gameObj, const eLayerType type);
		Layer& GetLayer(eLayerType type) { return mLayers[(UINT)type]; }
		std::vector<GameObject*> GetDontDestroyGameObjects();
		const std::vector<GameObject*>& GetGameObjects(const eLayerType type);
		
		void SetPlayer(Player* player) { mPlayer = player; }
		Player* GetPlayer() { return mPlayer; }

		void SetThreadLoad(bool thread) { mbThreadLoad = thread; }
		bool IsThreadLoad() { return mbThreadLoad; }
		eLoadStatus GetLoadStatus() const { return mLoadStatus; }
		bool IsLoadComplete() const { return eLoadStatus::LoadComplete == mLoadStatus; }


	private:
		std::vector<Layer> mLayers;
		eSceneType mType;
		Player* mPlayer;

		bool mbThreadLoad;							// 스레드로 로드할지 여부
		eLoadStatus mLoadStatus;
	};
}
