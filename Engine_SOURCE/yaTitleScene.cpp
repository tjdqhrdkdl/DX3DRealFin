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

#include "yaSpearman.h"
#include "yaMusketeerman.h"
#include "yaSwordsman.h"

#include "yaCrouchObjectScript.h"

#include "yaMonsterScript.h"

#include "yaFbxLoader.h"
#include "yaMeshData.h"
#include "yaTenzen.h"
#include "yaRedOgre.h"

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
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		CameraScript* camScript = cameraObj->AddComponent<CameraScript>();
		mainCamera = cameraComp;


		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->GetComponent<Transform>()->SetPosition(Vector3(-13.0f, 0.0f, -10.0f));
		player->GetComponent<Transform>()->SetScale(Vector3(5.0f, 5.0f, 5.0f));
		//player->GetComponent<Transform>()->SetRotation(Vector3(15.0f, 45.0f, 0.0f));

		/*MeshRenderer* mr = player->AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);*/

		camScript->SetTarget(player);
		player->SetCamera(cameraObj);
		SetPlayer(player);

		{

			Spearman* mSpearman = object::Instantiate<Spearman>(eLayerType::Monster);
			mSpearman->GetComponent<Transform>()->SetPosition(Vector3(5.0f, 0.0f, 15.0f));
			mSpearman->GetComponent<Transform>()->SetScale(Vector3(5.0f, 5.0f, 5.0f));
			mSpearman->SetName(L"Spearman");
			mSpearman->SetPlayerObject(player);
			//MeshRenderer* spearmanmr = mSpearman->AddComponent<MeshRenderer>();
			//spearmanmr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			//spearmanmr->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			Collider2D* spearmancol = mSpearman->AddComponent <Collider2D>();
			spearmancol->SetType(eColliderType::Box);
			spearmancol->SetSize(Vector3(1.0, 2.0f, 1.0f));
			Rigidbody* spearmanRigidbody = mSpearman->AddComponent<Rigidbody>();
			spearmanRigidbody->SetGround(false);
			mSpearman->AddComponent<MonsterScript>();

			camScript->SetLockOnTarget(mSpearman);

		}


		//{
		//	Musketeerman* musketeerman = object::Instantiate<Musketeerman>(eLayerType::Monster);
		//	musketeerman->GetComponent<Transform>()->SetPosition(Vector3(-5.0f, 0.0f, 15.0f));
		//	musketeerman->GetComponent<Transform>()->SetScale(Vector3(5.0f, 5.0f, 5.0f));
		//	musketeerman->SetName(L"Musketeerman");
		//	musketeerman->SetPlayerObject(player);
		//	MeshRenderer* musketeermanmr = musketeerman->AddComponent<MeshRenderer>();
		//	musketeermanmr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		//	musketeermanmr->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
		//	Collider2D* musketeermancol = musketeerman->AddComponent <Collider2D>();
		//	musketeermancol->SetType(eColliderType::Box);
		//	musketeermancol->SetSize(Vector3(1.0, 2.0f, 2.0f));
		//	Rigidbody* spearmanRigidbody = musketeerman->AddComponent<Rigidbody>();
		//	spearmanRigidbody->SetGround(false);

		//	camScript->SetLockOnTarget(musketeerman);
		//}

		//{
		//	Swordsman* swordsman = object::Instantiate<Swordsman>(eLayerType::Monster);
		//	swordsman->GetComponent<Transform>()->SetPosition(Vector3(5.0f, 0.0f, 15.0f));
		//	swordsman->GetComponent<Transform>()->SetScale(Vector3(5.0f, 5.0f, 5.0f));
		//	swordsman->SetName(L"Swordsman");
		//	swordsman->SetPlayerObject(player);
		//	MeshRenderer* swordsmanmr = swordsman->AddComponent<MeshRenderer>();
		//	swordsmanmr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		//	swordsmanmr->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
		//	Collider2D* swordsmancol = swordsman->AddComponent <Collider2D>();
		//	swordsmancol->SetType(eColliderType::Box);
		//	swordsmancol->SetSize(Vector3(1.0, 2.0f, 2.0f));
		//	Rigidbody* spearmanRigidbody = swordsman->AddComponent<Rigidbody>();
		//	spearmanRigidbody->SetGround(false);
		//	swordsman->AddComponent<MonsterScript>();

		//	camScript->SetLockOnTarget(swordsman);
		//}


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


		//{
		//	GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground);
		//	ground->SetName(L"Ground1");
		//	Transform* groundTr = ground->GetComponent<Transform>();
		//	groundTr->SetPosition(Vector3(0.0f, -5.0f, 10.0f));
		//	groundTr->SetScale(Vector3(50.0f, 10.0f, 10.0f));
		//	groundTr->SetRotation(Vector3(45.0f, 0.0f, 0.0f));
		//	MeshRenderer* groundRenderer = ground->AddComponent<MeshRenderer>();
		//	groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		//	groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
		//	Collider2D* groundCollider = ground->AddComponent<Collider2D>();
		//	groundCollider->SetType(eColliderType::Box);
		//	groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
		//	ground->AddComponent<GroundScript>();
		//}


		/*{
			GameObject* grappleGround = object::Instantiate<GameObject>(eLayerType::Ground);
			grappleGround->SetName(L"grapple target");
			Transform* groundTr = grappleGround->GetComponent<Transform>();
			groundTr->SetPosition(Vector3(-20.0f, 7.0f, -20.0f));
			groundTr->SetScale(Vector3(10.0f, 2.0f, 10.0f));
			groundTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			MeshRenderer* groundRenderer = grappleGround->AddComponent<MeshRenderer>();
			groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			Collider2D* groundCollider = grappleGround->AddComponent<Collider2D>();
			groundCollider->SetType(eColliderType::Box);
			groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
			grappleGround->AddComponent<GroundScript>();


			{
				GameObject* hookTarget = object::Instantiate<GameObject>(eLayerType::Hook);
				hookTarget->SetName(L"hook target");
				Transform* hookTargetTr = hookTarget->GetComponent<Transform>();
				hookTargetTr->SetPosition(Vector3(-20.0f, 5.0f, -20.0f));
				hookTargetTr->SetScale(Vector3(4.0f, 4.0f, 4.0f));
				Collider2D* hookTargetCollider = hookTarget->AddComponent<Collider2D>();
				hookTargetCollider->SetType(eColliderType::Box);
				hookTargetCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
				hookTarget->AddComponent<HookTargetScript>();
			}

			{
				//GameObject* grappleGround1 = object::Instantiate<GameObject>(eLayerType::Ground);
				//grappleGround1->SetName(L"grapple target2");
				//Transform* ground1Tr = grappleGround1->GetComponent<Transform>();
				//ground1Tr->SetPosition(Vector3(-40.0f, 7.0f, -20.0f));
				//ground1Tr->SetScale(Vector3(10.0f, 2.0f, 10.0f));
				//ground1Tr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
				//MeshRenderer* groundRenderer = grappleGround1->AddComponent<MeshRenderer>();
				//groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
				//groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
				//Collider2D* groundCollider = grappleGround1->AddComponent<Collider2D>();
				//groundCollider->SetType(eColliderType::Box);
				//groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));


				{
					GameObject* hookTarget1 = object::Instantiate<GameObject>(eLayerType::Hook);
					hookTarget1->SetName(L"Hook target2");
					Transform* hookTargetTr = hookTarget1->GetComponent<Transform>();
					hookTargetTr->SetPosition(Vector3(-40.0f, 5.0f, -20.0f));
					hookTargetTr->SetScale(Vector3(4.0f, 4.0f, 4.0f));
					Collider2D* hookTargetCollider = hookTarget1->AddComponent<Collider2D>();
					hookTargetCollider->SetType(eColliderType::Box);
					hookTargetCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
					hookTarget1->AddComponent<HookTargetScript>();
				}
			}

			{
				//GameObject* grappleGround1 = object::Instantiate<GameObject>(eLayerType::Ground);
				//grappleGround1->SetName(L"crouch object");
				//Transform* ground1Tr = grappleGround1->GetComponent<Transform>();
				//ground1Tr->SetPosition(Vector3(60.0f, 0.0f, 60.0f));
				//ground1Tr->SetScale(Vector3(40.0f, 20.0f, 40.0f));
				//Collider2D* groundCollider = grappleGround1->AddComponent<Collider2D>();
				//groundCollider->SetType(eColliderType::Box);
				//groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
				//grappleGround1->AddComponent<CrouchObjectScript>();
			}

		}

		{
			GameObject* grappleGround1 = object::Instantiate<GameObject>(eLayerType::Ground);
			grappleGround1->SetName(L"crouch object");
			Transform* ground1Tr = grappleGround1->GetComponent<Transform>();
			ground1Tr->SetPosition(Vector3(60.0f, 0.0f, 60.0f));
			ground1Tr->SetScale(Vector3(40.0f, 20.0f, 40.0f));
			Collider2D* groundCollider = grappleGround1->AddComponent<Collider2D>();
			groundCollider->SetType(eColliderType::Box);
			groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
			grappleGround1->AddComponent<CrouchObjectScript>();
		}*/

		//{
		//		GameObject* logbridge = object::Instantiate<GameObject>(eLayerType::Logbridge);
		//		logbridge->SetName(L"LogBridge");
		//		Transform* logbridgeTr = logbridge->GetComponent<Transform>();
		//		logbridgeTr->SetPosition(Vector3(0.0f, -6.0f, 10.0f));
		//		logbridgeTr->SetScale(Vector3(10.0f, 2.0f, 50.0f));
		//		logbridgeTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		//		MeshRenderer* logbridgeRenderer = logbridge->AddComponent<MeshRenderer>();
		//		logbridgeRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		//		logbridgeRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"),0);
		//		Collider2D* logbridgeCollider = logbridge->AddComponent<Collider2D>();
		//		logbridgeCollider->SetType(eColliderType::Box);
		//		logbridgeCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
		//		logbridge->AddComponent<LogBridgeScript>();

		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Monster, true);

		CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Monster, true);

		CollisionManager::CollisionLayerCheck(eLayerType::Logbridge, eLayerType::Player, true);



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

		//{
		//	GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
		//	directionalLight->SetName(L"PointLight");


			//{
			//		GameObject* logbridge = object::Instantiate<GameObject>(eLayerType::Logbridge);
			//		logbridge->SetName(L"LogBridge");
			//		Transform* logbridgeTr = logbridge->GetComponent<Transform>();
			//		logbridgeTr->SetPosition(Vector3(0.0f, -6.0f, 10.0f));
			//		logbridgeTr->SetScale(Vector3(10.0f, 2.0f, 50.0f));
			//		logbridgeTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			//		MeshRenderer* logbridgeRenderer = logbridge->AddComponent<MeshRenderer>();
			//		logbridgeRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			//		logbridgeRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"),0);
			//		Collider2D* logbridgeCollider = logbridge->AddComponent<Collider2D>();
			//		logbridgeCollider->SetType(eColliderType::Box);
			//		logbridgeCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
			//		logbridge->AddComponent<LogBridgeScript>();

			CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Player, true);
			CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Monster, true);

			CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Player, true);
			CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Monster, true);

			CollisionManager::CollisionLayerCheck(eLayerType::Logbridge, eLayerType::Player, true);

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

			//{
			//	GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			//	directionalLight->SetName(L"PointLight");

			//	directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

			//	Light* lightComp = directionalLight->AddComponent<Light>();
			//	lightComp->SetType(eLightType::Point);
			//	lightComp->SetRadius(20.0f);
			//	lightComp->SetDiffuse(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
			//	lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			//	lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
			//}

			//{
			//	GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			//	directionalLight->SetName(L"PointLight");

			//	directionalLight->GetComponent<Transform>()->SetPosition(Vector3(-15.0f, 0.0f, 0.0f));

			//	Light* lightComp = directionalLight->AddComponent<Light>();
			//	lightComp->SetType(eLightType::Point);
			//	lightComp->SetRadius(30.0f);
			//	lightComp->SetDiffuse(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			//	lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			//	lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
			//}
			{
				//mMeshData = MeshData::LoadFromFbx(L"Player\\Mesh\\AM_M_9000.fbx");
				//mMeshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000100.fbx", L"attack1");
				////mMeshData->LoadAnimationFromFbx(L"Player\\IInteriorMinistry_Samurai\\Animation\\a000_003007.fbx", L"attack2");			
				////mMeshData->LoadAnimationFromFbx(L"Player\\IInteriorMinistry_Samurai\\Animation\\a000_003055.fbx", L"attack3");
				////mMeshData->LoadAnimationFromFbx(L"Player\\IInteriorMinistry_Samurai\\Animation\\a000_007310.fbx", L"attack4");
				////mMeshData->LoadAnimationFromFbx(L"Player\\IInteriorMinistry_Samurai\\Animation\\a000_008051.fbx", L"attack5");
				//mMeshData->AnimationSave(L"Player\\AnimationData\\Player.animationdata");
				//mMeshData->Instantiate(eLayerType::Monster);

				//mMeshData = std::make_shared<MeshData>();
				//mMeshData->Load(L"Monster\\IInteriorMinistry_Samurai\\MeshData\\1700.meshdata");
				//mMeshData->AnimationLoad(L"Monster\\IInteriorMinistry_Samurai\\AnimationData\\Samurai.animationdata");
				//
				//mMeshData->Instantiate(eLayerType::Monster);


				//mMeshData->GetAnimationCompleteEvent(L"attack1") = std::bind(&TitleScene::Test, this);
				//mMeshData->GetAnimationStartEvent(L"attack2") = std::bind(&TitleScene::Test, this);
				//mMeshData->GetAnimationEndEvent(L"attack2") = std::bind(&TitleScene::Test, this);


				//mMeshData->GetAnimationFrameEvent(L"attack2", 10) = std::bind(&TitleScene::Test, this);


				//MeshData* newMeshData = new MeshData();
				//newMeshData->Load(L"Monster\\IInteriorMinistry_Samurai\\MeshData\\1700.meshdata");
				//GameObject* player = newMeshData->Instantiate(eLayerType::Monster);

				//mMeshData = new MeshData();


				//mMeshData = MeshData::LoadFromFbx(L"Map\\Mesh\\FinalMesh_Join.fbx");
				//mMeshData->Load(L"Map\\MeshData\\FinalMesh_Join.meshdata");
				//
				//GameObject* player = mMeshData->Instantiate(eLayerType::Monster);

				//mMeshData = MeshData::LoadFromFbx(L"Monster\\RedOgre\\Mesh\\c5020.fbx");
				//
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a000_000000.fbx", L"a000_000000");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a000_000401.fbx", L"a000_000401");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a000_000402.fbx", L"a000_000402");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a000_010000.fbx", L"a000_010000");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003000.fbx", L"a100_003000");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003001.fbx", L"a100_003001");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003002.fbx", L"a100_003002");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003003.fbx", L"a100_003003");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003005.fbx", L"a100_003005");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003006.fbx", L"a100_003006");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003007.fbx", L"a100_003007");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003008.fbx", L"a100_003008");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003011.fbx", L"a100_003011");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_005401.fbx", L"a100_005401");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_005402.fbx", L"a100_005402");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_005403.fbx", L"a100_005403");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_005400.fbx", L"a100_005400");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_005000.fbx", L"a100_005000");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_405010.fbx", L"a100_405010");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_405001.fbx", L"a100_405001");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_405002.fbx", L"a100_405002");
				//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_405003.fbx", L"a100_405003");
				////mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\.fbx", L"");


				//mMeshData->AnimationSave(L"Monster\\RedOgre\\AnimationData\\RedOgre.animationdata");

				//mMeshData = std::make_shared<MeshData>();
				//mMeshData->Load(L"Monster\\RedOgre\\MeshData\\c5020.meshdata");
				//mMeshData->AnimationLoad(L"Monster\\RedOgre\\AnimationData\\RedOgre.animationdata");
				//
				//mMeshData->Instantiate(eLayerType::Monster);
        
        
        
        //mMeshData = std::make_shared<MeshData>();
			//mMeshData->Load(L"Player\\MeshData\\AM_M_9000.meshdata");
			//mMeshData = MeshData::LoadFromFbx(L"Player\\Mesh\\AM_M_9000.fbx");

			//mMeshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000100.fbx", L"attack1");
			////mMeshData->LoadAnimationFromFbx(L"Player\\IInteriorMinistry_Samurai\\Animation\\a000_003007.fbx", L"attack2");			
			////mMeshData->LoadAnimationFromFbx(L"Player\\IInteriorMinistry_Samurai\\Animation\\a000_003055.fbx", L"attack3");
			////mMeshData->LoadAnimationFromFbx(L"Player\\IInteriorMinistry_Samurai\\Animation\\a000_007310.fbx", L"attack4");
			////mMeshData->LoadAnimationFromFbx(L"Player\\IInteriorMinistry_Samurai\\Animation\\a000_008051.fbx", L"attack5");
			//mMeshData->AnimationSave(L"Player\\AnimationData\\Player.animationdata");
			//mMeshData->Instantiate(eLayerType::Monster);

			//mMeshData = MeshData::LoadFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Mesh\\c1700_SpearMan.fbx");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_400000.fbx", L"a000_400000");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_000200.fbx", L"a000_000200");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_000401.fbx", L"a000_000401");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_000402.fbx", L"a000_000402");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_001040.fbx", L"a000_001040");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_000411.fbx", L"a000_000411");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_405000.fbx", L"a000_405000");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_405010.fbx", L"a000_405010");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_000412.fbx", L"a000_000412");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003000.fbx", L"a000_003000");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003001.fbx", L"a000_003001");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003004.fbx", L"a000_003004");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003005.fbx", L"a000_003005");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003053.fbx", L"a000_003053");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003006.fbx", L"a000_003006");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003007.fbx", L"a000_003007");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003010.fbx", L"a000_003010");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003011.fbx", L"a000_003011");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003012.fbx", L"a000_003012");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003015.fbx", L"a000_003015");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_003016.fbx", L"a000_003016");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008600.fbx", L"a000_008600");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008602.fbx", L"a000_008602");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008500.fbx", L"a000_008500");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008501.fbx", L"a000_008501");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008602.fbx", L"a000_008602");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008510.fbx", L"a000_008510");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a100_003102.fbx", L"a100_003102");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_500000.fbx", L"a000_500000");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_501040.fbx", L"a000_501040");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_505000.fbx", L"a000_505000");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_505001.fbx", L"a000_505001");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_505002.fbx", L"a000_505002");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_505003.fbx", L"a000_505003");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_505400.fbx", L"a000_505400");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_505401.fbx", L"a000_505401");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_505402.fbx", L"a000_505402");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_505403.fbx", L"a000_505403");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a100_005211.fbx", L"a100_005211");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_405400.fbx", L"a000_405400");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_405401.fbx", L"a000_405401");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_405402.fbx", L"a000_405402");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_405403.fbx", L"a000_405403");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_007000.fbx", L"a000_007000");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_007010.fbx", L"a000_007010");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_007020.fbx", L"a000_007020");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_007021.fbx", L"a000_007021");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_007121.fbx", L"a000_007121");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_012230.fbx", L"a000_012230");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_007100.fbx", L"a000_007100");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_007200.fbx", L"a000_007200");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_007120.fbx", L"a000_007120");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_007220.fbx", L"a000_007220");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008010.fbx", L"a000_008010");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008011.fbx", L"a000_008011");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008130.fbx", L"a000_008130");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008012.fbx", L"a000_008012");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008013.fbx", L"a000_008013");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008021.fbx", L"a000_008021");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008022.fbx", L"a000_008022");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_008023.fbx", L"a000_008023");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_020110.fbx", L"a000_020110");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a100_003000.fbx", L"a100_003000");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a100_003001.fbx", L"a100_003001");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_605000.fbx", L"a000_605000");
			//

			//mMeshData->AnimationSave(L"Monster\\\IInteriorMinistry_Samurai\\AnimationData\\SpearManAnimation_1.animationdata");
			//mMeshData->Instantiate(eLayerType::Monster);

			//mMeshData->LoadAnimationFromFbx(L"Monster\\\MusketeerMan\\Animation\\a300_400000.fbx", L"TEST");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\MusketeerMan\\Animation\\a300_003000.fbx", L"TEST1");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\MusketeerMan\\Animation\\a300_000700.fbx", L"TEST2");			
			//mMeshData->AnimationSave(L"Monster\\\MusketeerMan\\Animation\\MusketeerManAnimation_1.animationdata");



			//mMeshData = std::make_shared<MeshData>();
			//mMeshData = MeshData::LoadFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Mesh\\c1700_SpearMan.fbx");

			//mMeshData->Load(L"Monster\\\IInteriorMinistry_Samurai\\MeshData\\c1700_SpearMan.meshdata");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\\IInteriorMinistry_Samurai\\Animation\\a000_400000.fbx", L"SpearMan_Boundary_Step1");

			//mMeshData->AnimationLoad(L"Monster\\IInteriorMinistry_Samurai\\AnimationData\\SpearManAnimation_1.animationdata");
			//mMeshData->Instantiate(eLayerType::Monster);

			//mMeshData = MeshData::LoadFromFbx(L"House\\Mesh\\House.fbx");
			//mMeshData->Instantiate(eLayerType::Monster);


			//mMeshData->GetAnimationCompleteEvent(L"attack1") = std::bind(&TitleScene::Test, this);
			//mMeshData->GetAnimationStartEvent(L"attack2") = std::bind(&TitleScene::Test, this);
			//mMeshData->GetAnimationEndEvent(L"attack2") = std::bind(&TitleScene::Test, this);


			//mMeshData->GetAnimationFrameEvent(L"attack2", 10) = std::bind(&TitleScene::Test, this);


			//MeshData* newMeshData = new MeshData();
			//newMeshData->Load(L"Monster\\IInteriorMinistry_Samurai\\MeshData\\1700.meshdata");
			//GameObject* player = newMeshData->Instantiate(eLayerType::Monster);

			//mMeshData = new MeshData();

			//mMeshData = MeshData::LoadFromFbx(L"Map\\Mesh\\FinMap.fbx");
			
			//mMeshData = std::make_shared<MeshData>();
			//mMeshData->Load(L"Map\\MeshData\\FinMap.meshdata");
			//
			//mMeshData->Instantiate(eLayerType::Ground);

			//mMeshData = MeshData::LoadFromFbx(L"Monster\\RedOgre\\Mesh\\c5020.fbx");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a000_000402.fbx", L"a000_000402");
			//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a000_003001.fbx", L"a000_003001");
			//mMeshData->AnimationSave(L"Monster\\\RedOgre\\AnimationData\\RedOrge_Animation.animationdata");
			//
			//
			//mMeshData = std::make_shared<MeshData>();
			//mMeshData->Load(L"Monster\\RedOgre\\MeshData\\c5020.meshdata");
			//mMeshData->AnimationLoad(L"Monster\\\RedOgre\\AnimationData\\RedOrge_Animation.animationdata");
			//
			//
			//mMeshData->Instantiate(eLayerType::Monster);

			//mMeshData = MeshData::LoadFromFbx(L"Map\\Mesh\\FinMap.fbx");

			//mMeshData = std::make_shared<MeshData>();
			//mMeshData->Load(L"Map\\MeshData\\FinMap.meshdata");
			//mMeshData->Instantiate(eLayerType::Ground);
		}
		//object::Instantiate<Tenzen>(eLayerType::Monster);
		Scene::Initialize();
	}

	void TitleScene::Update()
	{

		if (Input::GetKeyDown(eKeyCode::U))
		{
			mMeshData->Play(L"a000_000402");
		}
		if (Input::GetKeyDown(eKeyCode::I))
		{
			mMeshData->Play(L"a000_003001");
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
