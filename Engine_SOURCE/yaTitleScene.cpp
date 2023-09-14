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
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera);
		cameraObj->SetName(L"MainCamera");
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(20.0f, .0f, -80.0f));
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Perspective);
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		CameraScript* camScript = cameraObj->AddComponent<CameraScript>();
		mainCamera = cameraComp;


		//Player* player = object::Instantiate<Player>(eLayerType::Player);
		//player->GetComponent<Transform>()->SetPosition(Vector3(-13.0f, 5.0f, -10.0f));
		//player->GetComponent<Transform>()->SetScale(Vector3(5.0f, 5.0f, 5.0f));
		//
		//MeshRenderer* mr = player->AddComponent<MeshRenderer>();
		//mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		//mr->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
		//
		//camScript->SetTarget(player);
		//player->SetCamera(cameraObj);
		//SetPlayer(player);
		

		//{
		//	Spearman* sperman = object::Instantiate<Spearman>(eLayerType::Monster);
		//}


		//{
		//	GameObject* uiCam = object::Instantiate<GameObject>(eLayerType::Camera);
		//	uiCam->SetName(L"UICamera");
		//	uiCam->GetComponent<Transform>()->SetPosition(Vector3::Zero);
		//	Camera* cameraComp = uiCam->AddComponent<Camera>();
		//	cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		//	cameraComp->DisableLayerMasks();
		//	cameraComp->TurnLayerMask(eLayerType::UI, true);
		//}

		//{
		//	UICanvas_InGame* ui = object::Instantiate<UICanvas_InGame>(eLayerType::UI);
		//	ui->SetName(L"UICanvasObj_InGame");
		//}

		/*{
			GameObject* wall = object::Instantiate<GameObject>(eLayerType::Wall);
			wall->SetName(L"wall");
			Transform* wallTr = wall->GetComponent<Transform>();
			wallTr->SetPosition(Vector3(25.0f, 15.0f, 10.0f));
			wallTr->SetScale(Vector3(50.0f, 50.0f, 4.0f));
			wallTr->SetRotation(Vector3(0.0f, 90.0f, 0.0f));
			MeshRenderer* wallRenderer = wall->AddComponent<MeshRenderer>();
			wallRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			wallRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			Collider2D* wallCollider = wall->AddComponent<Collider2D>();
			wallCollider->SetType(eColliderType::Box);
			wallCollider->SetSize(Vector3(1.0f, 1.0f, 1.0f));
			wall->AddComponent<WallScript>();



			wall = object::Instantiate<GameObject>(eLayerType::Wall);
			wall->SetName(L"wall1");
			wallTr = wall->GetComponent<Transform>();
			wallTr->SetPosition(Vector3(0.f, 10.0f, 30.0f));
			wallTr->SetScale(Vector3(50.f, 50.f, 4.f));
			wallTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			wallRenderer = wall->AddComponent<MeshRenderer>();
			wallRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			wallRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			wallCollider = wall->AddComponent<Collider2D>();
			wallCollider->SetType(eColliderType::Box);
			wallCollider->SetSize(Vector3(1.0f, 1.0f, 1.0f));
			wall->AddComponent<WallScript>();

			wall = object::Instantiate<GameObject>(eLayerType::Wall);
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
			GameObject* player = object::Instantiate<GameObject>(eLayerType::Monster);
			player->GetComponent<Transform>()->SetPosition(Vector3(-25.0f, 10.0f, 0.0f));
			player->GetComponent<Transform>()->SetScale(Vector3(10.0f, 10.0f, 10.0f));
			player->SetName(L"PPP");
			MeshRenderer* mr = player->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"SpriteMaterial");
			mr->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"ShadowMapTarget"));
		}*/

		{
			GameObject* player = object::Instantiate<GameObject>(eLayerType::Player);
			player->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			player->GetComponent<Transform>()->SetScale(Vector3(500.0f, 500.0f, 500.0f));
			player->SetName(L"SkyBox");
			MeshRenderer* mr = player->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"SkyBoxMaterial"), 0);
		}



		{
			GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground);
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
			GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground);
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
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
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

		CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Monster, true);

		CollisionManager::CollisionLayerCheck(eLayerType::Wall, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Logbridge, eLayerType::Player, true);


		
		mMeshData = MeshData::LoadFromFbx(L"Monster\\SwordMan\\Mesh\\c1700_SwordMan.fbx");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_400000.fbx", L"SwordMan_Boundary_Step1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_000200.fbx", L"SwordMan_Boundary_Step2");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_000401.fbx", L"SwordMan_Idle_Stand");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_000402.fbx", L"SwordMan_Boundary_Start1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_000403.fbx", L"SwordMan_Boundary_Start2");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_001040.fbx", L"SwordMan_shout");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_000411.fbx", L"SwordMan_Groggy");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405000.fbx", L"SwordMan_Walk");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405010.fbx", L"SwordMan_Running");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_000412.fbx", L"SwordMan_Groggy_Recovery");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003000.fbx", L"SwordMan_Start_Attack1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003001.fbx", L"SwordMan_Start_Attack2");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003004.fbx", L"SwordMan_Disarm_Attack1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003005.fbx", L"SwordMan_Disarm_Attack2");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003053.fbx", L"SwordMan_Disarm_Attack3");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003006.fbx", L"SwordMan_1Default_Attack1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003007.fbx", L"SwordMan_1Default_Attack2");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003010.fbx", L"SwordMan_2Default_Attack1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003011.fbx", L"SwordMan_2Default_Attack2");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003012.fbx", L"SwordMan_2Default_Attack3");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003015.fbx", L"SwordMan_3Default_Attack1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_003016.fbx", L"SwordMan_3Default_Attack2");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008600.fbx", L"SwordMan_1Default_Attack1_Failed");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008602.fbx", L"SwordMan_1Default_Attack2_Failed");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008500.fbx", L"SwordMan_2Default_Attack1_Failed");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008501.fbx", L"SwordMan_2Default_Attack2_Failed");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008602.fbx", L"SwordMan_2Default_Attack3_Failed");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008510.fbx", L"SwordMan_3Default_Attack2_Failed");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a100_003102.fbx", L"SwordMan_Defense_Reaction");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_500000.fbx", L"SwordMan_1Default_Defense1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_501040.fbx", L"SwordMan_1Default_Defense2");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505000.fbx", L"SwordMan_1Default_Defense_Forward");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505001.fbx", L"SwordMan_1Default_Defense_Back");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505002.fbx", L"SwordMan_1Default_Defense_Left");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505003.fbx", L"SwordMan_1Default_Defense_Right");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505400.fbx", L"SwordMan_1Default_Defense_Left_Turn_90");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505401.fbx", L"SwordMan_1Default_Defense_Right_Turn_90");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505402.fbx", L"SwordMan_1Default_Defense_Left_Turn_180");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_505403.fbx", L"SwordMan_1Default_Defense_Right_Turn_180");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a100_005211.fbx", L"SwordMan_Step_Back");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405400.fbx", L"SwordMan_Idle_Left_Turn_90");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405401.fbx", L"SwordMan_Idle_Right_Turn_90");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405402.fbx", L"SwordMan_Idle_Left_Turn_180");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_405403.fbx", L"SwordMan_Idle_Right_Turn_180");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_007000.fbx", L"SwordMan_Jump_1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_007010.fbx", L"SwordMan_Jump_2");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_007020.fbx", L"SwordMan_Jump_3");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_007021.fbx", L"SwordMan_Death_Hit1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_007121.fbx", L"SwordMan_Death_Hit2");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_012230.fbx", L"SwordMan_Death_Deathblow1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_007100.fbx", L"SwordMan_Levitation_Hit_Back");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_007200.fbx", L"SwordMan_Levitation_Hit_Front");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_007120.fbx", L"SwordMan_Wake_Up_Front");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_007220.fbx", L"SwordMan_Wake_Up_Back");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008010.fbx", L"SwordMan_Hit_1");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008011.fbx", L"SwordMan_Hit_2_Start");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008130.fbx", L"SwordMan_Hit_2_End");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008012.fbx", L"SwordMan_Hit_3");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008013.fbx", L"SwordMan_Hit_4");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008021.fbx", L"SwordMan_Hit_5");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008022.fbx", L"SwordMan_Hit_6");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_008023.fbx", L"SwordMan_Hit_7");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_020110.fbx", L"SwordMan_Hit_8");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_605000.fbx", L"SwordMan_Hit_Molotov_Cocktail");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a100_003000.fbx", L"SwordMan_Evasion_Left");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a100_003001.fbx", L"SwordMan_Evasion_Right");
		mMeshData->LoadAnimationFromFbx(L"Monster\\SwordMan\\Animation\\a000_400000.fbx", L"SwordMan_Boundary_Step1");
		mMeshData->AnimationSave(L"Monster\\SwordMan\\AnimationData\\SwordManAnimation_1.animationdata");


		/*{
			MapObjects* obj = object::Instantiate<MapObjects>(eLayerType::Player);
		}*/

		//Resources::Load<MeshData>(L"test", L"Player/Mesh/o000100.fbx");
		//object::Instantiate<Tenzen>(eLayerType::Monster);
		Scene::Initialize();
	}
	
	void TitleScene::Update()
	{
		//if (Input::GetKeyDown(eKeyCode::Y))
		//{
		//	//SceneManager::LoadScene(eSceneType::Play);	
		//}

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
