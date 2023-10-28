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

		//개발 로직에 따른(Script 등) 게임오브젝트 상태 변화
		virtual void Update();

		//충돌 처리
		virtual void CollisionUpdate();

		//Update와 CollisionUpdate에서 설정한 데이터를 최종 정리
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
