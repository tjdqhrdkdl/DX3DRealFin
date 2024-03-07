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
#include "yaSwordMan.h"
#include "yaTenzen.h"
#include "yaAshinaSoldier.h"
#include "yaAshinaSpearMan.h"

#include "MapObjects.h"
#include "yaBoundarySphere.h"
#include "yaMapCollider.h"

#include "yaNavMeshTool.h"
#include "yaAudioListener.h"

#include "yaParticleSystem.h"
#include "yaAudioClip.h"
#include "yaPlayerActionScript.h"

namespace ya
{
	PlayScene::PlayScene()
		: Scene(eSceneType::Play)
	{
	}
	PlayScene::~PlayScene()
	{
		NavMeshTool::DestroyInst();

	}
	void PlayScene::Initialize()
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
		player->GetComponent<Transform>()->SetPosition(Vector3(190.0f, -28.0f, 86.0f));
		player->GetComponent<Transform>()->SetRotation(Vector3(0.0f, -150.0f, 0.0f));
		player->GetComponent<Transform>()->SetScale(Vector3(1.0f, 1.0f, 1.0f));

		AudioListener* audioComp = player->AddComponent<AudioListener>();

		/*MeshRenderer* mr = player->AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);*/

		BoundarySphere* boundarySphere = player->AddComponent<BoundarySphere>();

		camScript->SetTarget(player);
		player->SetCamera(cameraObj);
		SetPlayer(player);
		

		{
			GameObject* uiCam = object::Instantiate<GameObject>(eLayerType::Camera, this);
			uiCam->SetName(L"UICamera1");
			uiCam->GetComponent<Transform>()->SetPosition(Vector3::Zero);
			Camera* UICameraComp = uiCam->AddComponent<Camera>();
			UICameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
			UICameraComp->DisableLayerMasks();
			UICameraComp->TurnLayerMask(eLayerType::UI, true);
			UICamera = UICameraComp;
			
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
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionalLight->SetName(L"directionalLight");

			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 100.0f, 0.0f));
			directionalLight->GetComponent<Transform>()->SetRotation(Vector3(45.0f, 0.0f, 0.0f));

			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
		}


		{
			GameObject* ground = object::Instantiate<GameObject>(eLayerType::Ground, this);
			ground->SetName(L"Ground2");
			Transform* groundTr = ground->GetComponent<Transform>();
			groundTr->SetPosition(Vector3(100.0f, -50.0f, -15.0f));
			groundTr->SetScale(Vector3(2000.0f, 1.0f, 3000.0f));
			groundTr->SetRotation(Vector3(0, 0.0f, 0.0f));
			//MeshRenderer* groundRenderer = ground->AddComponent<MeshRenderer>();
			//groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			//groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			Collider2D* groundCollider = ground->AddComponent<Collider2D>();
			groundCollider->SetType(eColliderType::Box);
			groundCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
		}

		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Monster, true);
		CollisionManager::CollisionLayerCheck(eLayerType::PlayerProjectile, eLayerType::Monster, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::MonsterProjectile, true);
		CollisionManager::CollisionLayerCheck(eLayerType::PlayerProjectile, eLayerType::MonsterProjectile, true);

		//CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Ground, eLayerType::Monster, true);

		//CollisionManager::CollisionLayerCheck(eLayerType::Wall, eLayerType::Player, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Wall, eLayerType::WallCheckCollision, true);
		//CollisionManager::CollisionLayerCheck(eLayerType::Logbridge, eLayerType::Player, true);

		{

			//MapObjects* obj = object::Instantiate<MapObjects>(eLayerType::None, this);
			//Transform* objTransform = obj->GetComponent<Transform>();
			//objTransform->SetPosition(-85.f, 35.f, 130.f);
			//objTransform->SetRotation(-90.f, 0.f, 0.f);
			//2411 5640 5710 6600 6610 6620 3651 3313 
		}


		{
			GameObject* hookTarget = object::Instantiate<GameObject>(eLayerType::Ground, this);
			hookTarget->SetName(L"hookTarget1");
			Transform* groundTr = hookTarget->GetComponent<Transform>();
			groundTr->SetPosition(Vector3(40.0f, 10.0f, -40.0f));
			groundTr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
			MeshRenderer* groundRenderer = hookTarget->AddComponent<MeshRenderer>();
			groundRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			groundRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);
			Collider2D* groundCollider = hookTarget->AddComponent<Collider2D>();
			groundCollider->SetType(eColliderType::Box);
			hookTarget->AddComponent<HookTargetScript>();
		}


		{
			object::Instantiate<MapCollider>(eLayerType::Ground, this);
		}

		{
			MapObjects* obj = object::Instantiate<MapObjects>(eLayerType::None, this);
			Transform* objTransform = obj->GetComponent<Transform>();
			objTransform->SetPosition(-85.f, 35.f, 130.f);
			objTransform->SetRotation(-90.f, 0.f, 0.f);
		}
		{
			GameObject* trObj = object::Instantiate<GameObject>(eLayerType::None, this);
			trObj->SetName(L".");
			Transform* navTr = trObj->GetComponent<Transform>();
			navTr->SetPosition(-85.f, 35.f, 130.f);
			navTr->SetRotation(-90.f, 0.f, 0.f);

			NavMeshTool* m = NavMeshTool::GetInst();
			m->SetMapMeshTr(navTr);
			m->Init(this);
		}


		//Resources::Load<MeshData>(L"test", L"Player/Mesh/o000100.fbx");


		// 1
		mMonsters.push_back(object::Instantiate<AshinaSoldier>(eLayerType::Monster, this, Vector3(180.0f, -33.0f, 70.0f)));		// 싸움
		mMonsters.push_back(object::Instantiate<AshinaSoldier>(eLayerType::Monster, this, Vector3(169.0f, -31.0f, 65.0f), Vector3(0.0f, 180.0f, 0.0f)));	// 암살
		
		// 2
		mMonsters.push_back(object::Instantiate<AshinaSoldier>(eLayerType::Monster, this, Vector3(128.0f, -21.0f, 33.0f), Vector3(0.0f, 30.0f, 0.0f)));
		mMonsters.push_back(object::Instantiate<AshinaSpearMan>(eLayerType::Monster, this, Vector3(109.0f, -21.0f, 9.0f), Vector3(0.0f, 180.0f, 0.0f)));	// 암살

		// 3
		mMonsters.push_back(object::Instantiate<AshinaSoldier>(eLayerType::Monster, this, Vector3(86.0f, -15.0f, 12.0f)));
		mMonsters.push_back(object::Instantiate<AshinaSoldier>(eLayerType::Monster, this, Vector3(64.0f, -9.0f, -9.0f)));
		mMonsters.push_back(object::Instantiate<SwordMan>(eLayerType::Monster, this, Vector3(78.0f, -15.0f, -4.0f)));
		//mMonsters.push_back(object::Instantiate<SwordMan>(eLayerType::Monster, this, Vector3(74.0f, -9.0f, 9.0f)));
		//mMonsters.push_back(object::Instantiate<AshinaSpearMan>(eLayerType::Monster, this, Vector3(45.0f, -6.0f, -2.0f)));

		// 4
		//mMonsters.push_back(object::Instantiate<AshinaSoldier>(eLayerType::Monster, this, Vector3(-8.0f, -2.0f, -7.0f)));
		mMonsters.push_back(object::Instantiate<AshinaSpearMan>(eLayerType::Monster, this, Vector3(18.f, -4.0f, -11.0f)));
		mMonsters.push_back(object::Instantiate<AshinaSoldier>(eLayerType::Monster, this, Vector3(6.0f, 8.0f, -2.0f)));
		mMonsters.push_back(object::Instantiate<Tenzen>(eLayerType::Monster, this, Vector3(1.0f, -4.0f, -15.0f)));



		Scene::Initialize();
	}
	
	void PlayScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Loading);
		}

		if (Input::GetKeyDown(eKeyCode::M))
		{
			Reset();
		}

		if (Input::GetKeyDown(eKeyCode::N_1))
		{
			GetPlayer()->GetComponent<Transform>()->SetPosition(GetPlayer()->GetCamera()->GetComponent<Transform>()->GetPosition());
		}


		bool bgmVolUp = false;
		for (UINT i = 0; i < mMonsters.size(); i++)
		{
			if (mMonsters[i]->IsRecognize() && ! (mMonsters[i]->IsMonsterState(MonsterBase::MonsterState_Dead)))
			{
				bgmVolUp = true;
				Vector3 monPos1 = mMonsters[i]->GetComponent<Transform>()->GetPosition();

				for (UINT k = 0; k < mMonsters.size(); k++)
				{
					if (i == k)
						continue;
					if (!(mMonsters[k]->IsRecognize()))
					{
						Vector3 monPos2 = mMonsters[k]->GetComponent<Transform>()->GetPosition();
						if (Vector3::Distance(monPos1, monPos2) < 7)
						{
							mMonsters[k]->SetPlayerLastPosition(GetPlayer()->GetComponent<Transform>()->GetPosition());
							mMonsters[k]->SetAlertnessCount(100);
							mMonsters[k]->AddMonsterState(MonsterBase::eMonsterState::MonsterState_Alert);
						}
					}
				}
			}
		}
		
		if (bgmVolUp)
			GetPlayer()->GetScript<PlayerActionScript>()->SetBGMVolume(10);
		else
			GetPlayer()->GetScript<PlayerActionScript>()->SetBGMVolume(5);

		

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
		Resources::Find<AudioClip>(L"bgm-usual")->Play();

	}
	void PlayScene::OnExit()
	{

	}

	void PlayScene::Reset()
	{
		GetPlayer()->Reset();

		for (MonsterBase* monster : mMonsters)
		{
			monster->Reset();
		}
	}
}