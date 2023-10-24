#include "yaSceneManager.h"
#include "yaTitleScene.h"
#include "yaPlayScene.h"
#include "yaLoadingScene.h"
#include "ThreadPool.h"
#include "yaRenderer.h"

#include "PhysXManager.h"
#include "yaTime.h"

namespace ya
{
	std::vector<Scene*> SceneManager::mScenes = {};
	Scene* SceneManager::mActiveScene = nullptr;
	Scene* SceneManager::mNextScene = nullptr;

	void SceneManager::Initialize()
	{
		mScenes.resize((UINT)eSceneType::End);

		mScenes[(UINT)eSceneType::Title] = new TitleScene();

		mScenes[(UINT)eSceneType::Loading] = new LoadingScene();

		mScenes[(UINT)eSceneType::Play] = new PlayScene();
		mScenes[(UINT)eSceneType::Play]->SetThreadLoad(true);
		//mScenes[(UINT)eSceneType::Play]->GetCallBack() = std::bind(SceneManager::LoadScene, eSceneType::Play);

		ChangeScene(eSceneType::Title);
		//mActiveScene = mScenes[(UINT)eSceneType::Title];
	}

	void SceneManager::Update()
	{
		if (mNextScene)
		{
			if (mNextScene->IsThreadLoad() && eLoadStatus::NotLoaded == mNextScene->GetLoadStatus())
			{
				ThreadPool::EnqueueJob([](Scene* nextScene) {
					nextScene->ThreadedInitialize();
					}, mNextScene);
			}
			else
			{
				mNextScene->Initialize();
			}


			if (mNextScene->IsLoadComplete())
			{
				Scene* prevScene = mActiveScene;
				mActiveScene = mNextScene;

				//mNextScene은 OnEnter 함수에서 새로 등록될 수도 있으므로 미리 비워준다.
				mNextScene = nullptr;

				//renderer 클리어
				renderer::ClearLights();

				// 바뀔때 dontDestory 오브젝트는 다음씬으로 같이 넘겨줘야한다.
				std::vector<GameObject*> gameObjs{};

				if (prevScene)
				{
					gameObjs = prevScene->GetDontDestroyGameObjects();
				}

				PhysicsManager::changePhysicScene(mActiveScene);
				for (GameObject* obj : gameObjs)
				{
					auto* col3D = obj->GetComponent<Collider3D>();
					if (col3D)
					{
						col3D->SceneChanged();
					}
					eLayerType type = obj->GetLayerType();
					mActiveScene->AddGameObject(obj, type);
				}

				if (prevScene)
					prevScene->OnExit();
				mActiveScene->OnEnter();
			}
		}

		mActiveScene->Update();
	}

	void SceneManager::FixedUpdate()
	{
		mActiveScene->FixedUpdate();
	}

	void SceneManager::Render()
	{
		mActiveScene->Render();
	}
	
	void SceneManager::Destroy()
	{
		mActiveScene->Destroy();
	}

	void SceneManager::Release()
	{
		for (Scene* scene : mScenes)
		{
			delete scene;
			scene = nullptr;
		}
	}

}