#include "yaScene.h"
#include "PhysXManager.h"

namespace ya
{
	Scene::Scene(eSceneType type)
		: mType(type)
		, mbThreadLoad(false)
		, mLoadStatus(eLoadStatus::NotLoaded)
		, mPlayer(nullptr)

	{
		mLayers.resize((UINT)eLayerType::End);
	}

	Scene::~Scene()
	{
	}

	void Scene::Initialize()
	{
		for (Layer& layer : mLayers)
		{
			layer.Initialize();
		}
		
		
		mLoadStatus = eLoadStatus::LoadComplete;
	}

	void Scene::ThreadedInitialize()
	{
		mLoadStatus = eLoadStatus::Loading;

		Initialize();
	}

	void Scene::Update()
	{
		for (Layer& layer : mLayers)
		{
			layer.Update();
		}
	}
	void Scene::FixedUpdate()
	{
		for (Layer& layer : mLayers)
		{
			layer.FixedUpdate();
		}
	}
	void Scene::Render()
	{
		for (Layer& layer : mLayers)
		{
			layer.Render();
		}

		//모든 레이어 렌더링이 끝나면 FrameEnd 호출
		for (Layer& layer : mLayers)
		{
			layer.FrameEnd();
		}
	}
	void Scene::Destroy()
	{
		for (Layer& layer : mLayers)
		{
			layer.Destroy();
		}
	}

	void Scene::OnEnter()
	{
	}

	void Scene::OnExit()
	{
		for (size_t i = 0; i < mLayers.size(); ++i)
		{
			mLayers[i].Reset();
		}
		mPlayer = nullptr;
		mLoadStatus = eLoadStatus::NotLoaded;
	}

	void Scene::CreatePhysXScene()
	{
		PhysicsManager::createPhysicScene(this);
		PhysicsManager::changePhysicScene(this);
	}

	void Scene::AddGameObject(GameObject* gameObj, const eLayerType type)
	{
		mLayers[(UINT)type].AddGameObject(gameObj);
		gameObj->SetLayerType(type);
	}
	
	std::vector<GameObject*> Scene::GetDontDestroyGameObjects()
	{
		std::vector<GameObject*> gameObjects;
		for (Layer& layer : mLayers)
		{
			std::vector<GameObject*> dontGameObjs
				= layer.GetDontDestroyGameObjects();

			gameObjects.insert(gameObjects.end()
			, dontGameObjs.begin()
			, dontGameObjs.end());
		}

		return gameObjects;
	}
	const std::vector<GameObject*>& Scene::GetGameObjects(const eLayerType type)
	{
		return mLayers[(UINT)type].GetGameObjects();
	}
}
