#include "yaSceneManager.h"
#include "yaTransform.h"
#include "yaMeshRenderer.h"
#include "yaRenderer.h"
#include "yaResources.h"
#include "yaTexture.h"
#include "yaPlayerScript.h"
#include "yaCamera.h"
#include "yaCameraScript.h"
#include "yaSpriteRenderer.h"
#include "yaGridScript.h"
#include "yaTitleScene.h"
#include "yaPlayScene.h"
#include "yaLoadingScene.h"
#include "ThreadPool.h"
#include "yaInput.h"

namespace ya
{
	std::vector<Scene*> SceneManager::mScenes = {};
	Scene* SceneManager::mActiveScene = nullptr;

	std::vector<std::future<std::function<void()>>> futures = {};

	void SceneManager::Initialize()
	{
		mScenes.resize((UINT)eSceneType::End);

		mScenes[(UINT)eSceneType::Tilte] = new TitleScene();
		mScenes[(UINT)eSceneType::Tilte]->SetName(L"TitleScene");
		mScenes[(UINT)eSceneType::Tilte]->SetThreadLoad(true);
		mScenes[(UINT)eSceneType::Tilte]->GetThreadCallBack() = std::bind(SceneManager::LoadScene, eSceneType::Tilte);

		mScenes[(UINT)eSceneType::Play] = new PlayScene();
		mScenes[(UINT)eSceneType::Play]->SetName(L"PlayScene");

		mScenes[(UINT)eSceneType::Loading] = new LoadingScene();
		mScenes[(UINT)eSceneType::Loading]->SetName(L"LoadingScene");

		for (Scene* scene : mScenes)
		{
			mActiveScene = scene;

			if (scene->IsThreadLoad())
			{
				futures.emplace_back(ThreadPool::EnqueueJob([scene]() -> std::function<void()> {
					printf("scene : %d \n", scene->GetSceneType());
					scene->Initialize();
					return scene->GetThreadCallBack();
				}));
			}
			else
			{
				scene->Initialize();
				printf("result : %d \n", (UINT)scene->GetSceneType());
			}
		}

		mActiveScene = mScenes[(UINT)eSceneType::Loading];

		for (auto& f : futures)
		{
			std::function<void()> result = f.get();
			result();
		}

	}

	void SceneManager::Update()
	{
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
	void SceneManager::LoadScene(eSceneType type)
	{
		if (mActiveScene)
			mActiveScene->OnExit();

		// 바뀔때 dontDestory 오브젝트는 다음씬으로 같이 넘겨줘야한다.
		std::vector<GameObject*> gameObjs 
			= mActiveScene->GetDontDestroyGameObjects();
		mActiveScene = mScenes[(UINT)type];
		
		for (GameObject* obj : gameObjs)
		{
			eLayerType type = obj->GetLayerType();
			mActiveScene->AddGameObject(obj, type);
		}

		mActiveScene->OnEnter();
	}
}