#pragma once
#include "yaScene.h"
#include "yaLayer.h"
#include "yaGameObject.h"
#include "yaSceneManager.h"
#include "yaTransform.h"

namespace ya::object
{
	template <typename T>
	static T* Instantiate(enums::eLayerType type, Scene* scene)
	{
		T* gameObj = new T();

		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObj);
		gameObj->SetScene(scene);
		gameObj->SetLayerType(type);
		gameObj->Initialize();

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType type, Scene* scene, Transform* parent)
	{
		T* gameObj = new T();

		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObj);

		Transform* tr = gameObj->GameObject::GetComponent<Transform>();
		tr->SetParent(parent);
		gameObj->SetScene(scene);
		gameObj->SetLayerType(type);
		gameObj->SetScene(scene);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType type, Scene* scene, Vector3 position, Vector3 rotation)
	{
		T* gameObj = new T();

		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObj);

		Transform* tr = gameObj->GameObject::GetComponent<Transform>();
		tr->SetLocalPosition(position);
		tr->SetLocalRotation(rotation);
		gameObj->SetScene(scene);
		gameObj->SetLayerType(type);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType type, Scene* scene, Vector3 position, Vector3 rotation, Transform* parent)
	{
		T* gameObj = new T();

		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObj);

		Transform* tr = gameObj->GameObject::GetComponent<Transform>();
		tr->SetLocalPosition(position);
		tr->SetLocalRotation(rotation);
		tr->SetParent(parent);
		gameObj->SetScene(scene);
		gameObj->SetLayerType(type);

		return gameObj;
	}


	template <typename T>
	static T* Instantiate(enums::eLayerType type, enums::eSceneType sceneType)
	{
		Scene* scene = SceneManager::GetScene(sceneType);
		return Instantiate<T>(type, scene);
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType type, enums::eSceneType sceneType, Transform* parent)
	{
		Scene* scene = SceneManager::GetScene(sceneType);
		return Instantiate<T>(type, scene, parent);
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType type, enums::eSceneType sceneType, Vector3 position, Vector3 rotation)
	{
		Scene* scene = SceneManager::GetScene(sceneType);
		return Instantiate<T>(type, scene, position, rotation);
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType type, enums::eSceneType sceneType, Vector3 position, Vector3 rotation, Transform* parent)
	{
		Scene* scene = SceneManager::GetScene(sceneType);
		return Instantiate<T>(type, scene, position, rotation, parent);
	}




	static void Destroy(GameObject* gameObject)
	{
		gameObject->Death();
	}

	static void DontDestroyOnLoad(GameObject* gameObject)   //씬 이동시 이 오브젝트는 삭제하지 않는다
	{
		if (gameObject == nullptr)
			return;

		gameObject->DontDestroy(true);
	}
}