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

namespace ya
{
	PlayerScript::PlayerScript()
		: Script()
		, mJumpTimer(0.0f)
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
		Transform* tr = GetOwner()->GetComponent<Transform>();

		float speed = 120.0f; // 후에 플레이어 status로 변경
		

		Rigidbody* rigidbody = GetOwner()->GetComponent<Rigidbody>();

		// camera script wasd 미사용시 키 변경
		if (Input::GetKey(eKeyCode::L))
		{
			rigidbody->AddForce(speed * tr->Right());
		}
		if (Input::GetKey(eKeyCode::J))
		{
			rigidbody->AddForce(speed * -tr->Right());
		}

		if (Input::GetKey(eKeyCode::I))
		{
			rigidbody->AddForce(speed * tr->Forward());
		}
		if (Input::GetKey(eKeyCode::K))
		{
			rigidbody->AddForce(speed * -tr->Forward());
		}

		if (Input::GetKey(eKeyCode::SPACE))
		{
			Rigidbody* rigidbody = GetOwner()->GetComponent<Rigidbody>();
			if (rigidbody == nullptr)
			{
				return;
			}

			if (rigidbody->IsGround())
			{
				rigidbody->SetGround(false);
				mJumpTimer = 0.1f;
			}
		}

		if (mJumpTimer > 0.0f)
		{
			mJumpTimer -= Time::DeltaTime();
			rigidbody->AddForce(Vector3(0.0f, 400.0f, 0.0f));
		}
		
		Vector3 rot = tr->GetRotation();
		// 임시 회전. 마우스로 방향 전환 추가시 삭제
		if (Input::GetKey(eKeyCode::O))
		{
			rot += speed * 2.0f * tr->Up() * Time::DeltaTime();
		}
		if (Input::GetKey(eKeyCode::U))
		{
			rot += speed * 2.0f * -tr->Up() * Time::DeltaTime();
		}
		tr->SetRotation(rot);
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
