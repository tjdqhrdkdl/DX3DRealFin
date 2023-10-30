#include "yaLoadingScene.h"
#include "yaCamera.h"
#include "yaCameraScript.h"
#include "yaObject.h"
#include "yaInput.h"

#include "yaLight.h"

#include "yaMeshRenderer.h"
#include "yaResources.h"

#include "yaTime.h"
#include "yaMath.h"

namespace ya
{
	LoadingScene::LoadingScene()
		: Scene(eSceneType::Loading)
		, mLoadingTime(1.0f)
		, mLoader(nullptr)
	{

	}

	LoadingScene::~LoadingScene()
	{
	}

	void LoadingScene::Initialize()
	{
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, this);
		cameraObj->SetName(L"LoadCamera");
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraComp->DisableLayerMasks();
		cameraComp->TurnLayerMask(eLayerType::UI, true);
		cameraObj->AddComponent<CameraScript>();

		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionalLight->SetName(L"Loading_directionalLight");

			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			directionalLight->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, 0.0f));

			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
		}

		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00000.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00001.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00002.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00003.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00004.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00005.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00006.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00007.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00008.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00009.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00010.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00011.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00012.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00013.png");
		//Resources::Load<Texture>(L"MENU_Load_00014", L"Texture\\Menu\\Load\\MENU_Load_00014.png");

		{
			GameObject* background = object::Instantiate<GameObject>(eLayerType::UI, this);
			background->SetName(L"Load_background");

			Transform* backgroundTr = background->GetComponent<Transform>();
			backgroundTr->SetPosition(Vector3(0.0f, 0.f, 0.f));
			//backgroundTr->SetPosition(Vector3(-652.0f, -400.f, -10.f));
			backgroundTr->SetScale(Vector3(1600.0f, 900.0f, 50.0f));

			std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"MENU_Load_00004", L"Texture\\Menu\\Load\\MENU_Load_00004.png");

			std::shared_ptr<Material> backgroundtMaterial = std::make_shared<Material>();
			backgroundtMaterial->SetRenderingMode(eRenderingMode::Transparent);
			backgroundtMaterial->SetShader(Resources::Find<Shader>(L"SpriteShader"));
			backgroundtMaterial->SetTexture(eTextureSlot::Albedo, texture);
			Resources::Insert<Material>(L"backgroundtMaterial", backgroundtMaterial);

			MeshRenderer* meshRenderer = background->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			meshRenderer->SetMaterial(backgroundtMaterial, 0);
		}

		{
			mLoader = object::Instantiate<GameObject>(eLayerType::UI, this);
			mLoader->SetName(L"Load_loader");

			Transform* loaderTr = mLoader->GetComponent<Transform>();
			loaderTr->SetPosition(Vector3(-700.0f, -320.f, 0.f));
			loaderTr->SetScale(Vector3(50.0f, 50.0f, 50.0f));

			std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"loader", L"Texture\\Menu\\Common\\loader_w.png");

			std::shared_ptr<Material> loadertMaterial = std::make_shared<Material>();
			loadertMaterial->SetRenderingMode(eRenderingMode::Transparent);
			loadertMaterial->SetShader(Resources::Find<Shader>(L"SpriteShader"));
			loadertMaterial->SetTexture(eTextureSlot::Albedo, texture);
			Resources::Insert<Material>(L"loadertMaterial", loadertMaterial);

			MeshRenderer* meshRenderer = mLoader->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			meshRenderer->SetMaterial(loadertMaterial, 0);
		}

		Scene::Initialize();
	}

	void LoadingScene::Update()
	{
		/*if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Play);
		}*/

		{ // loader
			Transform* loaderTr = mLoader->GetComponent<Transform>();
			float rot = loaderTr->GetRotation().z;
			rot -= Time::DeltaTime() * 200.0f;

			if (rot < -360.0f)
				rot += 360.0f;

			mLoader->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, rot));
		}

		// scene change
		// 일정 시간 대기 후 scene init이 완료되었다면 다음 씬을 불러온다.
		if (mLoadingTime > 0.0f)
		{
			mLoadingTime -= Time::DeltaTime();
		}
		else
		{
			Scene* playScene = SceneManager::GetScene(eSceneType::Play);
			if (playScene->IsInitcomplete())
				SceneManager::LoadScene(eSceneType::Play);
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
