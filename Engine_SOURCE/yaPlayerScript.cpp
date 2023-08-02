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
#include "yaWallScript.h"

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
	}


	void PlayerScript::Render()
	{
	}

	void PlayerScript::OnCollisionEnter(Collider2D* collider)
	{
		GameObject* colObj = collider->GetOwner();
		Transform* colTransform = colObj->GetComponent<Transform>();

		GameObject* obj = GetOwner();
		Transform* objTransform = obj->GetComponent<Transform>();

		// �� �浹
		if (nullptr != colObj->GetScript<WallScript>())
		{
			Rigidbody* objRigidbody = obj->GetComponent<Rigidbody>();

			Vector3 velocity = objRigidbody->GetVelocity();
			Vector3 pos = objTransform->GetPosition();

			pos -= velocity * Time::DeltaTime();
			objTransform->SetPosition(pos);

			//colRigidbody->SetWall(true);
		}
	}

	void PlayerScript::OnCollisionStay(Collider2D* collider)
	{
		GameObject* colObj = collider->GetOwner();
		Transform* colTransform = colObj->GetComponent<Transform>();

		GameObject* obj = GetOwner();
		Transform* objTransform = obj->GetComponent<Transform>();

		// �� �浹
		if (nullptr != colObj->GetScript<WallScript>())
		{
			Rigidbody* objRigidbody = obj->GetComponent<Rigidbody>();

			Vector3 wallNormal = colTransform->Right();

			Vector3 objVelocity = objRigidbody->GetVelocity();
			Vector3 objPos = objTransform->GetPosition();

			Vector3 projvec = wallNormal * objVelocity;
			projvec *= wallNormal;

			objVelocity -= projvec;

			objPos -= objVelocity * Time::DeltaTime();
			objTransform->SetPosition(objPos);
		}
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
