#include "yaTitleScene.h"

#include "yaObject.h"
#include "yaResources.h"
#include "yaRenderer.h"
#include "yaTexture.h"
#include "yaInput.h"

#include "yaCamera.h"
#include "yaCameraScript.h"

#include "yaTransform.h"
#include "yaSpriteRenderer.h"
#include "yaMeshRenderer.h"
#include "yaGridScript.h"
#include "yaAudioListener.h"
#include "yaAudioClip.h"
#include "yaTime.h"

namespace ya
{
	TitleScene::TitleScene()
		: Scene(eSceneType::Title)
		, mTime(0)
	{

	}

	TitleScene::~TitleScene()
	{
	}

	void TitleScene::Initialize()
	{
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, this);
		cameraObj->SetName(L"TitleSceneCamera");
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraComp->DisableLayerMasks();
		cameraComp->TurnLayerMask(eLayerType::UI, true);
		cameraObj->AddComponent<CameraScript>();
		cameraObj->AddComponent<AudioListener>();

		std::shared_ptr<AudioClip> bgm = Resources::Load<AudioClip>(L"TitleBGM", L"..\\Resources\\Sound\\bgm\\01. Sekiro, the One Armed Wolf.mp3");
		bgm->SetLoop(true);
		bgm->Play();
		{

			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionalLight->SetName(L"Title_directionalLight");

			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			directionalLight->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, 0.0f));

			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
		}

		std::shared_ptr<Texture> titleCopyTexture = Resources::Load<Texture>(L"SB_Title_copy", L"Texture\\Menu\\Common\\SB_Title_copy.png");
		//std::shared_ptr<Texture> titleCopy2Texture = Resources::Load<Texture>(L"SB_Title_copy_2", L"Texture\\Menu\\Common\\SB_Title_copy_2.png");
		std::shared_ptr<Texture> titleCopy3Texture = Resources::Load<Texture>(L"SB_Title_copy_2", L"Texture\\Menu\\Common\\SB_Title_copy_3.png");
		std::shared_ptr<Texture> bgBlackTexture = Resources::Find<Texture>(L"bgblack");
		//std::shared_ptr<Texture> titleTexture = Resources::Load<Texture>(L"SB_Title", L"Texture\\Menu\\Common\\SB_Title.png");
		//std::shared_ptr<Texture> title2Texture = Resources::Load<Texture>(L"SB_Title_02", L"Texture\\Menu\\Common\\SB_Title_02.png");

		{
			GameObject* bgblack = object::Instantiate<GameObject>(eLayerType::UI, this);
			bgblack->SetName(L"TitleScene_background");

			Transform* titleCopy2Tr = bgblack->GetComponent<Transform>();
			titleCopy2Tr->SetPosition(Vector3(0.0f, 0.f, 0.f));
			titleCopy2Tr->SetScale(Vector3(1600.0f, 900.0f, 0.0f));

			std::shared_ptr<Material> bgblackMaterial = std::make_shared<Material>();
			bgblackMaterial->SetRenderingMode(eRenderingMode::Transparent);
			bgblackMaterial->SetShader(Resources::Find<Shader>(L"SpriteShader"));
			bgblackMaterial->SetTexture(eTextureSlot::Albedo, bgBlackTexture);
			Resources::Insert<Material>(L"bgblackMaterial", bgblackMaterial);

			MeshRenderer* bgblackRd = bgblack->AddComponent<MeshRenderer>();
			bgblackRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			bgblackRd->SetMaterial(bgblackMaterial, 0);
		}

		{
			GameObject* titleCopy2 = object::Instantiate<GameObject>(eLayerType::UI, this);
			titleCopy2->SetName(L"TitleScene_text");

			Transform* titleCopy2Tr = titleCopy2->GetComponent<Transform>();
			titleCopy2Tr->SetPosition(Vector3(0.0f, 0.f, 0.f));
			titleCopy2Tr->SetScale(Vector3(760.0f, 440.0f, 0.0f));

			std::shared_ptr<Material> titleCopy2tMaterial = std::make_shared<Material>();
			titleCopy2tMaterial->SetRenderingMode(eRenderingMode::Transparent);
			titleCopy2tMaterial->SetShader(Resources::Find<Shader>(L"SpriteShader"));
			titleCopy2tMaterial->SetTexture(eTextureSlot::Albedo, titleCopy3Texture);
			Resources::Insert<Material>(L"titleCopy2tMaterial", titleCopy2tMaterial);

			MeshRenderer* titleCopy2Rd = titleCopy2->AddComponent<MeshRenderer>();
			titleCopy2Rd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			titleCopy2Rd->SetMaterial(titleCopy2tMaterial, 0);
		}

		{
			GameObject* titleCopy = object::Instantiate<GameObject>(eLayerType::UI, this);
			titleCopy->SetName(L"TitleScene_Sekiro_shadowsdietwice");

			Transform* titleCopyTr = titleCopy->GetComponent<Transform>();
			titleCopyTr->SetPosition(Vector3(0.0f, 0.f, 0.f));
			titleCopyTr->SetScale(Vector3(1000.0f, 250.0f, 0.0f));

			std::shared_ptr<Material> titleCopytMaterial = std::make_shared<Material>();
			titleCopytMaterial->SetRenderingMode(eRenderingMode::Transparent);
			titleCopytMaterial->SetShader(Resources::Find<Shader>(L"SpriteShader"));
			titleCopytMaterial->SetTexture(eTextureSlot::Albedo, titleCopyTexture);
			Resources::Insert<Material>(L"titleCopytMaterial", titleCopytMaterial);

			MeshRenderer* meshRenderer = titleCopy->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			meshRenderer->SetMaterial(titleCopytMaterial, 0);
		}

		Scene::Initialize();
	}

	void TitleScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::SPACE) || Input::GetKeyDown(eKeyCode::ENTER) || mTime >= 7)
		{
			SceneManager::LoadScene(eSceneType::Loading);
		}

		mTime += Time::DeltaTime();
		Scene::Update();
	}

	void TitleScene::FixedUpdate()
	{
		Scene::FixedUpdate();
	}

	void TitleScene::Render()
	{
		Scene::Render();
	}

	void TitleScene::OnEnter()
	{

	}

	void TitleScene::OnExit()
	{
		Resources::Find<AudioClip>(L"TitleBGM")->Stop();

	}

}
