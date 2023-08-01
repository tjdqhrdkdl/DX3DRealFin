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

namespace ya
{
	PlayerScript::PlayerScript()
		: Script()
	{
	}

	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initalize()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		{
			// �÷��̾��� forward�� �����ϱ����� object
			// �Ŀ� mesh ����� ���� ����
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
		// camera script wasd �̻��� Ű ����
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
			action->Move(tr->Foward());
		}
		if (Input::GetKey(eKeyCode::K))
		{
			action->Move(-tr->Foward());
		}

		if (Input::GetKey(eKeyCode::SPACE))
		{
			action->Jump();
		}
		
		// �ӽ� ȸ��. ���콺�� ���� ��ȯ �߰��� ����
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
