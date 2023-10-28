#include "yaPlayScene.h"

#include "yaObject.h"
#include "yaResources.h"
#include "yaInput.h"
#include "yaCollisionManager.h"

#include "yaCamera.h"
#include "yaCameraScript.h"
#include "yaLight.h"

#include "yaWallScript.h"
#include "yaGroundScript.h"
#include "yaCrouchObjectScript.h"
#include "yaHookTargetScript.h"

#include "yaPlayer.h"

#include "yaMonster.h"
#include "yaSpearman.h"
#include "yaMusketeerman.h"
#include "yaSwordsman.h"
#include "yaTenzen.h"
#include "yaRedOgre.h"

#include "MapObjects.h"
#include "yaBoundarySphere.h"
#include "yaMapCollider.h"

//#include "yaCollisionManager.h"

#include "PhysXDebugObj.h"
#include "TestCameraScript.h"
#include "PhysXPlayerScript.h"

namespace ya
{

	PlayScene::PlayScene()
		: Scene(eSceneType::Play)
	{
		SetName(L"PlayScene");
	}
	PlayScene::~PlayScene()
	{
	}
	void PlayScene::Initialize()
	{
		CollisionManager::EnableGravity(true, this, Vector3(0.f, -120.f, 0.f));

		CreateRealScene();
		//CreateTestScene();
		Scene::Initialize();
	}
	
	void PlayScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::ChangeScene(eSceneType::Loading);
		}

		Scene::Update();
	}
	void PlayScene::FixedUpdate()
	{
		Scene::FixedUpdate();
	}
	void PlayScene::Render()
	{
		Scene::Render();
	}
	void PlayScene::OnEnter()
	{
	}
	void PlayScene::OnExit()
	{
	}


	void PlayScene::CreateRealScene()
	{
		//CollisionManager::EnableCollision(eLayerType::Player, eLayerType::Player, true);
		CollisionManager::EnableCollision(eLayerType::Player, eLayerType::Monster, true);
		CollisionManager::EnableCollision(eLayerType::PlayerProjectile, eLayerType::Monster, true);
		CollisionManager::EnableCollision(eLayerType::Player, eLayerType::MonsterProjectile, true);
		CollisionManager::EnableCollision(eLayerType::PlayerProjectile, eLayerType::MonsterProjectile, true);

		CollisionManager::EnableCollision(eLayerType::Ground, eLayerType::Player, true);
		CollisionManager::EnableCollision(eLayerType::Ground, eLayerType::Monster, true);

		CollisionManager::EnableCollision(eLayerType::Wall, eLayerType::Player, true);
		CollisionManager::EnableCollision(eLayerType::Wall, eLayerType::WallCheckCollision, true);

		CollisionManager::EnableRaycast(eLayerType::Camera, eLayerType::Ground, true);
		CollisionManager::EnableRaycast(eLayerType::Camera, eLayerType::Wall, true);

		CollisionManager::EnableRaycast(eLayerType::Player, eLayerType::Monster, true);
		//CollisionManager::EnableRaycast(GetOwner()->GetLayerType(), (UINT32)eLayerType::HangingObject, true);


		// Main Camera Game Object
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, this);
		cameraObj->SetName(L"MainCamera");
		cameraObj->GetComponent<Transform>()->SetLocalPosition(Vector3(20.0f, .0f, -80.0f));
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Perspective);
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		CameraScript* camScript = cameraObj->AddComponent<CameraScript>();
		mainCamera = cameraComp;


		Player* player = object::Instantiate<Player>(eLayerType::Player, this);
		player->GetComponent<Transform>()->SetLocalPosition(Vector3(30.0f, 30.f, -30.0f));
		player->GetComponent<Transform>()->SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));



		/*MeshRenderer* mr = player->AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);*/

		//BoundarySphere* boundarySphere = player->AddComponent<BoundarySphere>();

		camScript->SetTarget(player);
		player->SetCamera(cameraObj);
		SetPlayer(player);


		{
			GameObject* uiCam = object::Instantiate<GameObject>(eLayerType::Camera, this);
			uiCam->SetName(L"UICamera1");
			uiCam->GetComponent<Transform>()->SetLocalPosition(Vector3::Zero);
			Camera* cameraComp = uiCam->AddComponent<Camera>();
			cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
			cameraComp->DisableLayerMasks();
			cameraComp->TurnLayerMask(eLayerType::UI, true);
		}

		{
			//GameObject* wall = object::Instantiate<GameObject>(eLayerType::Wall, this);
			//wall->SetName(L"wall");
			//Transform* wallTr = wall->GetComponent<Transform>();
			//wallTr->SetLocalPosition(Vector3(93.0f, 15.0f, 10.0f));
			//wallTr->SetLocalScale(Vector3(50.0f, 50.0f, 1.0f));
			//wallTr->SetLocalRotation(Vector3(0.0f, 90.0f, 0.0f));
			//MeshRenderer* wallRenderer = wall->AddComponent<MeshRenderer>();
			//wallRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			//wallRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);


			//Collider3D* wallCollider = wall->AddComponent<Collider3D>();
			//wallCollider->SetType(eColliderType::Box);


			//wall->AddComponent<WallScript>();

			//Collider3D* wallCollider = wall->AddComponent<Collider3D>();
			//wallCollider->SetType(eColliderType::Box);
			//wallCollider->SetSize(Vector3(1.0f, 1.0f, 1.0f));
			//wall->AddComponent<WallScript>();



			//wall = object::Instantiate<GameObject>(eLayerType::Wall, this);
			//wall->SetName(L"wall1");
			//wallTr = wall->GetComponent<Transform>();
			//wallTr->SetLocalPosition(Vector3(66.f, 15.0f, 52.0f));
			//wallTr->SetLocalScale(Vector3(50.f, 50.f, 1.f));
			//wallTr->SetLocalRotation(Vector3(0.0f, 45.f, 0.0f));
			//wallRenderer = wall->AddComponent<MeshRenderer>();
			//wallRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			//wallRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);


			//wallCollider = wall->AddComponent<Collider3D>();
			//wallCollider->SetType(eColliderType::Box, true);
			//wall->AddComponent<WallScript>();

			//wall = object::Instantiate<GameObject>(eLayerType::Wall, this);
			//wall->SetName(L"wall2");
			//wallTr = wall->GetComponent<Transform>();
			//wallTr->SetLocalPosition(Vector3(25.0f, 15.0f, 10.0f));
			//wallTr->SetLocalScale(Vector3(50.0f, 50.0f, 4.0f));
			//wallTr->SetLocalRotation(Vector3(0.0f, 90.0f, 0.0f));
			//wallRenderer = wall->AddComponent<MeshRenderer>();
			//wallRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			//wallRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);


			//wallCollider = wall->AddComponent<Collider3D>();
			//wallCollider->SetType(eColliderType::Box, true);
			//wall->AddComponent<WallScript>();
		}

		/*{
			GameObject* player = object::Instantiate<GameObject>(eLayerType::Monster);
			player->GetComponent<Transform>()->SetLocalPosition(Vector3(-25.0f, 10.0f, 0.0f));
			player->GetComponent<Transform>()->SetLocalScale(Vector3(10.0f, 10.0f, 10.0f));
			player->SetName(L"PPP");
			MeshRenderer* mr = player->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

			std::shared_ptr<Material> mat = Resources::Find<Material>(L"SpriteMaterial");
			mr->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"ShadowMapTarget"));
		}*/

		{
			GameObject* skyBox = object::Instantiate<GameObject>(eLayerType::None, this);

			skyBox->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));
			skyBox->GetComponent<Transform>()->SetLocalScale(Vector3(500.0f, 500.0f, 500.0f));
			skyBox->SetName(L"SkyBox");
			MeshRenderer* mr = skyBox->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"SkyBoxMaterial"), 0);
		}


		{
			GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground, this);
			ground->SetName(L"Ground");
			Transform* groundTr = ground->GetComponent<Transform>();
			groundTr->SetLocalPosition(Vector3(0.0f, -11.0f, 10.0f));
			groundTr->SetLocalScale(Vector3(1000.0f, 4.0f, 1000.0f));
			groundTr->SetLocalRotation(Vector3(0.0f, 0.0f, 0.0f));

			Collider3D* groundCollider = ground->AddComponent<Collider3D>();
			groundCollider->SetType(eColliderType::Box, true);
			groundCollider->SetSize(Vector3(1000.0f, 1.0f, 1000.0f));
		}


		/*{
			GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground, this);
			ground->SetName(L"Ground1");
			Transform* groundTr = ground->GetComponent<Transform>();
			groundTr->SetLocalPosition(Vector3(0.0f, -5.0f, 10.0f));
			groundTr->SetLocalScale(Vector3(20.0f, 1.0f, 30.0f));
			groundTr->SetLocalRotation(Vector3(30.0f, 0.0f, 0.0f));
			MeshRenderer* groundRenderer = ground->AddComponent<MeshRenderer>();
			groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			Collider3D* groundCollider = ground->AddComponent<Collider3D>();
			groundCollider->SetType(eColliderType::Box);
			groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
			ground->AddComponent<GroundScript>();
		}*/

		{
			//GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground, this);
			//ground->SetName(L"Ground2");
			//Transform* groundTr = ground->GetComponent<Transform>();
			//groundTr->SetLocalPosition(Vector3(100.0f, -5.0f, -15.0f));
			//groundTr->SetLocalScale(Vector3(20.0f, 1.0f, 30.0f));
			//groundTr->SetLocalRotation(Vector3(-30.0f, 0.0f, 0.0f));
			////MeshRenderer* groundRenderer = ground->AddComponent<MeshRenderer>();
			////groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			////groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			//Collider3D* groundCollider = ground->AddComponent<Collider3D>();
			//groundCollider->SetType(eColliderType::Box);
			//groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
		}

		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionalLight->SetName(L"directionalLight");

			directionalLight->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 100.0f, 0.0f));
			directionalLight->GetComponent<Transform>()->SetLocalRotation(Vector3(45.0f, 0.0f, 0.0f));

			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
		}

		{
			//GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground, this);
			//ground->SetName(L"Ground2");
			//Transform* groundTr = ground->GetComponent<Transform>();
			//groundTr->SetLocalPosition(Vector3(100.0f, -5.0f, -15.0f));
			//groundTr->SetLocalScale(Vector3(20.0f, 1.0f, 30.0f));
			//groundTr->SetLocalRotation(Vector3(-30.0f, 0.0f, 0.0f));
			////MeshRenderer* groundRenderer = ground->AddComponent<MeshRenderer>();
			////groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			////groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			//Collider3D* groundCollider = ground->AddComponent<Collider3D>();
			//groundCollider->SetType(eColliderType::Box);
			//groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
		}



		{

			//MapObjects* obj = object::Instantiate<MapObjects>(eLayerType::None, this);
			//Transform* objTransform = obj->GetComponent<Transform>();
			//objTransform->SetLocalPosition(-85.f, 35.f, 130.f);
			//objTransform->SetLocalRotation(-90.f, 0.f, 0.f);
			//2411 5640 5710 6600 6610 6620 3651 3313 
		}


		{
			//GameObject* hookTarget = object::Instantiate<GameObject>(eLayerType::Ground, this);
			//hookTarget->SetName(L"hookTarget1");
			//Transform* groundTr = hookTarget->GetComponent<Transform>();
			//groundTr->SetLocalPosition(Vector3(40.0f, 10.0f, -40.0f));
			//groundTr->SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));
			//MeshRenderer* groundRenderer = hookTarget->AddComponent<MeshRenderer>();
			//groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			//groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			//Collider3D* groundCollider = hookTarget->AddComponent<Collider3D>();
			//groundCollider->SetType(eColliderType::Box);
			//hookTarget->AddComponent<HookTargetScript>();
		}


		{
			object::Instantiate<MapCollider>(eLayerType::Ground, this);
		}
		//Resources::Load<MeshData>(L"test", L"Player/Mesh/o000100.fbx");
		//object::Instantiate<Tenzen>(eLayerType::Monster, this);
	}
	void PlayScene::CreateTestScene()
	{
		// Main Camera Game Object
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, this);
		cameraObj->SetName(L"MainCamera");
		cameraObj->GetComponent<Transform>()->SetLocalPosition(Vector3(20.0f, .0f, -80.0f));
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Perspective);
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		TestCameraScript* camScript = cameraObj->AddComponent<TestCameraScript>();
		mainCamera = cameraComp;

		//Light
		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionalLight->SetName(L"directionalLight");

			directionalLight->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 100.0f, 0.0f));
			directionalLight->GetComponent<Transform>()->SetLocalRotation(Vector3(45.0f, 0.0f, 0.0f));

			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
		}

		//디버그 플레이어
		{
			PhysXDebugObj* player = object::Instantiate<PhysXDebugObj>(eLayerType::Player, this);
			player->SetName(L"testObj");
			player->AddComponent<PhysXPlayerScript>();

			Transform* tr = player->GetComponent<Transform>();
			tr->SetLocalPosition(Vector3(-10.f, 100.f, -10.f));

			Collider3D* coll3D = player->GetComponent<Collider3D>();
			//coll3D->SetSize(Vector3(30.f, 30.f, 30.f));
			coll3D->SetMass(50000.f);
			coll3D->SetType(eColliderType::Box);
			//coll3D->EnableGravity(true);
		}

		{
			PhysXDebugObj* player = object::Instantiate<PhysXDebugObj>(eLayerType::Ground, this);
			player->SetName(L"Ground");
			Transform* tr = player->GetComponent<Transform>();
			tr->SetLocalScale(Vector3(1000.f, 30.f, 1000.f));
			tr->SetLocalPosition(Vector3(10.f, 10.f, 10.f));

			Collider3D* coll3D = player->GetComponent<Collider3D>();
			coll3D->SetType(eColliderType::Box, true);
		}

		CollisionManager::EnableCollision(eLayerType::Player, eLayerType::Ground, true);



	}
}
