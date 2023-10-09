#include "yaLoadingScene.h"
#include "yaCamera.h"
#include "yaCameraScript.h"
#include "yaObject.h"
#include "yaInput.h"

namespace ya
{
	LoadingScene::LoadingScene()
		: Scene(eSceneType::Loading)
	{

	}

	LoadingScene::~LoadingScene()
	{
	}

	void LoadingScene::Initialize()
	{
		/*GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();*/

		Scene::Initialize();
	}

	void LoadingScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Tilte);
		}

		Scene::Update();
	}

	void LoadingScene::FixedUpdate()
	{
		Scene::FixedUpdate();
	}

	void LoadingScene::Render()
	{
		Scene::Render();
	}

	void LoadingScene::OnEnter()
	{

	}

	void LoadingScene::OnExit()
	{

	}

}
