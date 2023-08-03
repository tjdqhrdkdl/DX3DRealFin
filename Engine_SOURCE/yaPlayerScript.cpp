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
			GameObject* face = object::Instantiate<GameObject>(eLayerType::Player, tr);
			face->SetName(L"face");
			Transform* faceTr = face->GetComponent<Transform>();
			faceTr->SetPosition(Vector3(0.0f, 0.5f, 0.5f));
			faceTr->SetScale(Vector3(0.4f, 0.4f, 0.4f));
			MeshRenderer* faceRenderer = face->AddComponent<MeshRenderer>();
			faceRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"));
			faceRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		}
	}

	void PlayerScript::Update()
	{
		ActionScript* action = GetOwner()->GetScript<ActionScript>();
		action->SetSpeed(120.0f); // �Ŀ� �÷��̾� status�� ����

		Transform* tr = GetOwner()->GetComponent<Transform>();
		float speed = 120.0f; // 후에 플레이어 status로 변경

		if (Input::GetKey(eKeyCode::L))
		{
			action->Move(tr->Right());
		}
		if (Input::GetKey(eKeyCode::J))
		{
			action->Move(-tr->Right());
		}
		if (Input::GetKey(eKeyCode::I))
		{
			action->Move(tr->Forward());
		}
		if (Input::GetKey(eKeyCode::K))
		{
			action->Move(-tr->Forward());
		}

		if (Input::GetKey(eKeyCode::SPACE))
		{
			action->Jump();
		}

		if (Input::GetKey(eKeyCode::O))
		{
			action->Rotate(tr->Up());
		}
		if (Input::GetKey(eKeyCode::U))
		{
			action->Rotate(-tr->Up());
		}

		if (Input::GetKey(eKeyCode::F))
		{
			GrappleHookScript* grap = GetOwner()->GetScript<GrappleHookScript>();
			grap->GrappleHook();
		}

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
	}

	void PlayerScript::OnCollisionStay(Collider2D* collider)
	{
	}

	void PlayerScript::OnCollisionExit(Collider2D* collider)
	{
	}

	void PlayerScript::Start()
	{
	}

	void PlayerScript::Action()
	{
	}

	void PlayerScript::End()
	{
	}

}