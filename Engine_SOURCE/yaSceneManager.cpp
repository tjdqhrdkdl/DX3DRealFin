#include "yaSceneManager.h"
#include "yaTitleScene.h"
#include "yaPlayScene.h"
#include "yaLoadingScene.h"
#include "ThreadPool.h"

namespace ya
{
	std::vector<Scene*> SceneManager::mScenes = {};
	Scene* SceneManager::mActiveScene = nullptr;

	void SceneManager::Initialize()
	{
		mScenes.resize((UINT)eSceneType::End);

		mScenes[(UINT)eSceneType::Title] = new TitleScene();
		mScenes[(UINT)eSceneType::Title]->SetName(L"TitleScene");

		mScenes[(UINT)eSceneType::Loading] = new LoadingScene();
		mScenes[(UINT)eSceneType::Loading]->SetName(L"LoadingScene");

		mScenes[(UINT)eSceneType::Play] = new PlayScene();
		mScenes[(UINT)eSceneType::Play]->SetName(L"PlayScene");
		//mScenes[(UINT)eSceneType::Play]->SetThreadLoad(true);
		mScenes[(UINT)eSceneType::Play]->GetCallBack() = std::bind(SceneManager::LoadScene, eSceneType::Play);

		for (Scene* scene : mScenes)
		{
			mActiveScene = scene;

			if (scene->IsThreadLoad())
			{
				ThreadPool::EnqueueJob([scene]() {
					scene->Initialize();
				});
			}
			else
			{
				scene->Initialize();
			}
		}

		mActiveScene = mScenes[(UINT)eSceneType::Play];
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