#include "yaTitleScene.h"
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
#include "yaObject.h"
#include "yaInput.h"
#include "yaCollider2D.h"
#include "yaPlayer.h"
#include "yaMonster.h"
#include "yaCollisionManager.h"
#include "yaAnimator.h"
#include "yaLight.h"
#include "yaPaintShader.h"
#include "yaParticleSystem.h"
#include "yaRigidbody.h"
#include "yaGroundScript.h"
#include "yaSpearman.h"
#include "yaActionScript.h"
#include "yaGrappleHookScript.h"
#include "yaHookTargetScript.h"
#include "yaLogBridgeScript.h"
#include "yaWallScript.h"

#include "yaSpearman.h"
#include "yaMusketeerman.h"
#include "yaSwordsman.h"

#include "yaCrouchObjectScript.h"

#include "yaMonsterScript.h"

#include "yaFbxLoader.h"
#include "yaMeshData.h"
#include "yaTenzen.h"
#include "yaRedOgre.h"

#include "UICanvas_InGame.h"
#include "MapObjects.h"
#include "yaBoundarySphere.h"

namespace ya
{
	TitleScene::TitleScene()
		: Scene(eSceneType::Tilte)
	{
	}
	TitleScene::~TitleScene()
	{
	}
	void TitleScene::Initialize()
	{
		// Main Camera Game Object
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, this);
		cameraObj->SetName(L"MainCamera");
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(20.0f, .0f, -80.0f));
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Perspective);
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		CameraScript* camScript = cameraObj->AddComponent<CameraScript>();
		mainCamera = cameraComp;


		Player* player = object::Instantiate<Player>(eLayerType::Player, this);
		player->GetComponent<Transform>()->SetPosition(Vector3(30.0f, 0.0f, -30.0f));
		player->GetComponent<Transform>()->SetScale(Vector3(5.0f, 5.0f, 5.0f));

		/*MeshRenderer* mr = player->AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);*/

		BoundarySphere* boundarySphere = player->AddComponent<BoundarySphere>();

		camScript->SetTarget(player);
		player->SetCamera(cameraObj);
		SetPlayer(player);
		

		//{
		//	GameObject* uiCam = object::Instantiate<GameObject>(eLayerType::Camera, this);
		//	uiCam->SetName(L"UICamera");
		//	uiCam->GetComponent<Transform>()->SetPosition(Vector3::Zero);
		//	Camera* cameraComp = uiCam->AddComponent<Camera>();
		//	cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		//	cameraComp->DisableLayerMasks();
		//	cameraComp->TurnLayerMask(eLayerType::UI, true);
		//}

		//{
		//	UICanvas_InGame* ui = object::Instantiate<UICanvas_InGame>(eLayerType::UI, this);
		//	ui->SetName(L"UICanvasObj_InGame");
		//}

		{
			GameObject* wall = object::Instantiate<GameObject>(eLayerType::Wall, this);
			wall->SetName(L"wall");
			Transform* wallTr = wall->GetComponent<Transform>();
			wallTr->SetPosition(Vector3(33.0f, 15.0f, 10.0f));
			wallTr->SetScale(Vector3(50.0f, 50.0f, 1.0f));
			wallTr->SetRotation(Vector3(0.0f, 90.0f, 0.0f));
			MeshRenderer* wallRenderer = wall->AddComponent<MeshRenderer>();
			wallRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			wallRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			Collider2D* wallCollider = wall->AddComponent<Collider2D>();
			wallCollider->SetType(eColliderType::Box);
			wallCollider->SetSize(Vector3(1.0f, 1.0f, 1.0f));
			wall->AddComponent<WallScript>();



			wall = object::Instantiate<GameObject>(eLayerType::Wall, this);
			wall->SetName(L"wall1");
			wallTr = wall->GetComponent<Transform>();
			wallTr->SetPosition(Vector3(16.f, 15.0f, 52.0f));
			wallTr->SetScale(Vector3(50.f, 50.f, 1.f));
			wallTr->SetRotation(Vector3(0.0f, 45.f, 0.0f));
			wallRenderer = wall->AddComponent<MeshRenderer>();
			wallRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			wallRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			wallCollider = wall->AddComponent<Collider2D>();
			wallCollider->SetType(eColliderType::Box);
			wallCollider->SetSize(Vector3(1.0f, 1.0f, 1.0f));
			wall->AddComponent<WallScript>();

			wall = object::Instantiate<GameObject>(eLayerType::Wall, this);
			wall->SetName(L"wall2");
			wallTr = wall->GetComponent<Transform>();
			wallTr->SetPosition(Vector3(-25.0f, 15.0f, 10.0f));
			wallTr->SetScale(Vector3(50.0f, 50.0f, 4.0f));
			wallTr->SetRotation(Vector3(0.0f, 90.0f, 0.0f));
			wallRenderer = wall->AddComponent<MeshRenderer>();
			wallRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			wallRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			wallCollider = wall->AddComponent<Collider2D>();
			wallCollider->SetType(eColliderType::Box);
			wallCollider->SetSize(Vector3(1.0f, 1.0f, 1.0f));
			wall->AddComponent<WallScript>();
		}


		{
			GameObject* shadow = object::Instantiate<GameObject>(eLayerType::None, this);
			shadow->GetComponent<Transform>()->SetPosition(Vector3(-25.0f, 10.0f, 0.0f));
			shadow->GetComponent<Transform>()->SetScale(Vector3(10.0f, 10.0f, 10.0f));
			shadow->SetName(L"PPP");
			MeshRenderer* mr = shadow->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"SpriteMaterial");
			mr->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"ShadowMapTarget"));
		}

		{
			GameObject* skyBox = object::Instantiate<GameObject>(eLayerType::None, this);
			skyBox->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			skyBox->GetComponent<Transform>()->SetScale(Vector3(500.0f, 500.0f, 500.0f));
			skyBox->SetName(L"SkyBox");
			MeshRenderer* mr = skyBox->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"SkyBoxMaterial"), 0);
		}



		{
			GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground, this);
			ground->SetName(L"Ground");
			Transform* groundTr = ground->GetComponent<Transform>();
			groundTr->SetPosition(Vector3(0.0f, -11.0f, 10.0f));
			groundTr->SetScale(Vector3(1000.0f, 4.0f, 1000.0f));
			groundTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			MeshRenderer* groundRenderer = ground->AddComponent<MeshRenderer>();
			groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			Collider2D* groundCollider = ground->AddComponent<Collider2D>();
			groundCollider->SetType(eColliderType::Box);
			groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
			ground->AddComponent<GroundScript>();
		}


		/*{
			GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground, this);
			ground->SetName(L"Ground1");
			Transform* groundTr = ground->GetComponent<Transform>();
			groundTr->SetPosition(Vector3(0.0f, -5.0f, 10.0f));
			groundTr->SetScale(Vector3(20.0f, 1.0f, 30.0f));
			groundTr->SetRotation(Vector3(30.0f, 0.0f, 0.0f));
			MeshRenderer* groundRenderer = ground->AddComponent<MeshRenderer>();
			groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			Collider2D* groundCollider = ground->AddComponent<Collider2D>();
			groundCollider->SetType(eColliderType::Box);
			groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
			ground->AddComponent<GroundScript>();
		}*/

		{
			GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground, this);
			ground->SetName(L"Ground2");
			Transform* groundTr = ground->GetComponent<Transform>();
			groundTr->SetPosition(Vector3(0.0f, -5.0f, -15.0f));
			groundTr->SetScale(Vector3(20.0f, 1.0f, 30.0f));
			groundTr->SetRotation(Vector3(-30.0f, 0.0f, 0.0f));
			MeshRenderer* groundRenderer = ground->AddComponent<MeshRenderer>();
			groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			Collider2D* groundCollider = ground->AddComponent<Collider2D>();
			groundCollider->SetType(eColliderType::Box);
			groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
			ground->AddComponent<GroundScript>();
		}



		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionalLight->SetName(L"directionalLight");

			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 100.0f, 0.0f));
			directionalLight->GetComponent<Transform>()->SetRotation(Vector3(45.0f, 0.0f, 0.0f));

			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
		}

		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Monster, true);
		CollisionManager::CollisionLayerCheck(eLayerType::PlayerProjectile, eLayerType::Monster, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::MonsterProjectile, true);
		CollisionManager::CollisionLayerCheck(eLayerType::PlayerProjectile, eLayerType::MonsterProjectile, true);

		CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Monster, true);

		CollisionManager::CollisionLayerCheck(eLayerType::Wall, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Wall, eLayerType::WallCheckCollision, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Logbridge, eLayerType::Player, true);

		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player, this);
			directionalLight->SetName(L"directionalLight");

			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 100.0f, 0.0f));
			directionalLight->GetComponent<Transform>()->SetRotation(Vector3(45.0f, 0.0f, 0.0f));

			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
		}

		{
			//MapObjects* obj = object::Instantiate<MapObjects>(eLayerType::Player, this);
		}

		//Resources::Load<MeshData>(L"test", L"Player/Mesh/o000100.fbx");
		//object::Instantiate<Tenzen>(eLayerType::Monster, this);
		Scene::Initialize();
	}
	
	void TitleScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Loading);
		}

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
	}
}
