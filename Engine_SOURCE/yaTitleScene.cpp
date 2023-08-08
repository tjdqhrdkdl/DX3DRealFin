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
//#include "yaFBXLoader.h"
#include "yaRigidbody.h"
#include "yaGroundScript.h"
#include "yaLogBridgeScript.h"
#include "yaSpearman.h"
#include "yaActionScript.h"
#include "yaMoveScript.h"
#include "yaGrappleHookScript.h"
#include "yaHookTargetScript.h"

#include "yaSpearman.h"
#include "yaMonsterScript.h"

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
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -20.0f));
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Perspective);
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		CameraScript* camScript = cameraObj->AddComponent<CameraScript>();
		mainCamera = cameraComp;


		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->GetComponent<Transform>()->SetPosition(Vector3(-13.0f, 0.0f, -10.0f));
		player->GetComponent<Transform>()->SetScale(Vector3(5.0f, 5.0f, 5.0f));
		//player->GetComponent<Transform>()->SetRotation(Vector3(15.0f, 45.0f, 0.0f));
		
		MeshRenderer* mr = player->AddComponent<MeshRenderer>();
		mr->SetMaterial(Resources::Find<Material>(L"DeferredMaterial"));
		mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));

		camScript->SetTarget(player);
		player->SetCamera(cameraObj);


		Spearman* spearman = object::Instantiate<Spearman>(eLayerType::Monster);
		spearman->GetComponent<Transform>()->SetPosition(Vector3(5.0f, 0.0f, 15.0f));
		spearman->GetComponent<Transform>()->SetScale(Vector3(5.0f, 5.0f, 5.0f));
		spearman->SetName(L"Spearman");
		spearman->SetPlayerObject(player);
		MeshRenderer* spearmanmr = spearman->AddComponent<MeshRenderer>();
		spearmanmr->SetMaterial(Resources::Find<Material>(L"DeferredMaterial"));
		spearmanmr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		Collider2D* spearmancol = spearman->AddComponent <Collider2D>();
		spearmancol->SetType(eColliderType::Box);
		spearmancol->SetSize(Vector3(1.0, 2.0f, 2.0f));
		Rigidbody* spearmanRigidbody = spearman->AddComponent<Rigidbody>();
		spearmanRigidbody->SetGround(false);
		spearman->AddComponent<MonsterScript>();


		

		{
			GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground);
			ground->SetName(L"Ground");
			Transform* groundTr = ground->GetComponent<Transform>();
			groundTr->SetPosition(Vector3(0.0f, -10.0f, 10.0f));
			groundTr->SetScale(Vector3(100.0f, 4.0f, 100.0f));
			groundTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			MeshRenderer* groundRenderer = ground->AddComponent<MeshRenderer>();
			groundRenderer->SetMaterial(Resources::Find<Material>(L"DeferredMaterial"));
			groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			Collider2D* groundCollider = ground->AddComponent<Collider2D>();
			groundCollider->SetType(eColliderType::Box);
			groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
			ground->AddComponent<GroundScript>();
		}

		//{
		//		GameObject* logbridge = object::Instantiate<GameObject>(eLayerType::LogBridge);
		//		logbridge->SetName(L"LogBridge");
		//		Transform* logbridgeTr = logbridge->GetComponent<Transform>();
		//		logbridgeTr->SetPosition(Vector3(0.0f, -6.0f, 10.0f));
		//		logbridgeTr->SetScale(Vector3(10.0f, 2.0f, 50.0f));
		//		logbridgeTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		//		MeshRenderer* logbridgeRenderer = logbridge->AddComponent<MeshRenderer>();
		//		logbridgeRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"));
		//		logbridgeRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		//		Collider2D* logbridgeCollider = logbridge->AddComponent<Collider2D>();
		//		logbridgeCollider->SetType(eColliderType::Box);
		//		logbridgeCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
		//		logbridge->AddComponent<LogBridgeScript>();
		//}

		{
			GameObject* grappleGround = object::Instantiate<GameObject>(eLayerType::Ground);
			grappleGround->SetName(L"grapple target");
			Transform* groundTr = grappleGround->GetComponent<Transform>();
			groundTr->SetPosition(Vector3(0.0f, 7.0f, 0.0f));
			groundTr->SetScale(Vector3(10.0f, 2.0f, 10.0f));
			groundTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			MeshRenderer* groundRenderer = grappleGround->AddComponent<MeshRenderer>();
			groundRenderer->SetMaterial(Resources::Find<Material>(L"DeferredMaterial"));
			groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			Collider2D* groundCollider = grappleGround->AddComponent<Collider2D>();
			groundCollider->SetType(eColliderType::Box);
			groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
			grappleGround->AddComponent<GroundScript>();

			GrappleHookScript* action = player->GetScript<GrappleHookScript>();
			action->SetGrappleHookTarget(grappleGround);

			{
				GameObject* hookTarget = object::Instantiate<GameObject>(eLayerType::Ground);
				hookTarget->SetName(L"grapple target");
				Transform* hookTargetTr = hookTarget->GetComponent<Transform>();
				hookTargetTr->SetPosition(Vector3(0.0f, 12.0f, 0.0f));
				hookTargetTr->SetScale(Vector3(4.0f, 20.0f, 4.0f));
				Collider2D* hookTargetCollider = hookTarget->AddComponent<Collider2D>();
				hookTargetCollider->SetType(eColliderType::Box);
				hookTargetCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
				hookTarget->AddComponent<HookTargetScript>();
			}
		}

		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Monster, true);

		CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Monster, true);

		CollisionManager::CollisionLayerCheck(eLayerType::LogBridge, eLayerType::Player, true);



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

		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			directionalLight->SetName(L"PointLight");

			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Point);
			lightComp->SetRadius(20.0f);
			lightComp->SetDiffuse(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
		}

		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			directionalLight->SetName(L"PointLight");

			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(-15.0f, 0.0f, 0.0f));

			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Point);
			lightComp->SetRadius(30.0f);
			lightComp->SetDiffuse(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
		}

	
		Scene::Initialize();
	}
	void TitleScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Play);
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
