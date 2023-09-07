#include "yaPlayerScript.h"
#include "yaPlayer.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaAnimator.h"
#include "yaObject.h";
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaRigidbody.h"
#include "yaActionScript.h"
#include "yaGrappleHookScript.h"
#include "yaMusketeerman.h"
#include "yaSpearman.h"
#include "yaSwordsman.h"


#include "yaSceneManager.h"
#include "yaScene.h"


namespace ya
{
	PlayerScript::PlayerScript()
		: Script()
	{
	}

	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initialize()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		{
			// 플레이어의 forward를 구분하기위한 object
			// 후에 mesh 씌우면 없앨 예정
			/*GameObject* face = object::Instantiate<GameObject>(eLayerType::Player, tr);
			face->SetName(L"face");
			Transform* faceTr = face->GetComponent<Transform>();
			faceTr->SetPosition(Vector3(0.0f, 0.5f, 0.5f));
			faceTr->SetScale(Vector3(0.4f, 0.4f, 0.4f));
			MeshRenderer* faceRenderer = face->AddComponent<MeshRenderer>();
			faceRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
			faceRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);*/

		}
	}

	void PlayerScript::Update()
	{
		if (Input::GetKey(eKeyCode::F))
		{
			GrappleHookScript* grap = GetOwner()->GetScript<GrappleHookScript>();
			if(grap != nullptr)
			{
				grap->GrappleHook();
			}
		}
		ActionScript* action = GetOwner()->GetScript<ActionScript>();
		Transform* tr = GetOwner()->GetComponent < Transform>();

		// 테스트로 만든 공격 입니다 나중에 지울것
		if (Input::GetKeyDown(eKeyCode::Q))
		{
			action->Move(tr->Forward() * 500.f);
			Player* player = (Player*)GetOwner();
			player->SetAttack(true);
		}
		Player* player = (Player*)GetOwner();
		if (player->IsAttack())
		{
			TESTTime += Time::DeltaTime();
			if (TESTTime >= 1.5f)
			{
				player->SetAttack(false);
				TESTTime = 0.f;
			}
		}
		if (player->IsWalk())
		{
			action->Move(-(tr->Forward() * 250.f));
			TESTTime += Time::DeltaTime();
			if (TESTTime >= 0.5f)
			{
				player->SetWalk(false);
				TESTTime = 0.f;
			}
		}



	}


	void PlayerScript::Render()
	{
	}

	void PlayerScript::OnCollisionEnter(Collider2D* collider)
	{
		//Scene* test = SceneManager::GetActiveScene();
		//test->GetGameObjects(enums::eLayerType::Monster);

		if (dynamic_cast<GameObject*>(collider->GetOwner()))
		{
			GameObject* ground = (GameObject*)collider->GetOwner();
			{
				if (L"Ground1" == ground->GetName())
				{
					
					Transform* playertr = GetOwner()->GetComponent<Transform>();
					Transform* groundtr = ground->GetComponent<Transform>();

					Vector3 goundUp = groundtr->Up();
					Vector3 playerUp = playertr->Up();


		
					int a = 0;


				}


			}
		}

		
		if (dynamic_cast<MonsterBase*>(collider->GetOwner()))
		{

		}
		if (dynamic_cast<Spearman*>(collider->GetOwner()))
		{
		
		}
		if (dynamic_cast<Swordsman*>(collider->GetOwner()))
		{
		
		}

	}

	void PlayerScript::OnCollisionStay(Collider2D* collider)
	{
	}

	void PlayerScript::OnCollisionExit(Collider2D* collider)
	{
	}

}
