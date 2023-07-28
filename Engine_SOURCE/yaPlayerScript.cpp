#include "yaPlayerScript.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaAnimator.h"
#include "yaMonster.h"

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
		//Animator* animator = GetOwner()->GetComponent<Animator>();
		//animator->GetStartEvent(L"MoveDown") = std::bind(&PlayerScript::Start, this);
		//animator->GetCompleteEvent(L"Idle") = std::bind(&PlayerScript::Action, this);
		//animator->GetEndEvent(L"Idle") = std::bind(&PlayerScript::End, this);
		//animator->GetEvent(L"Idle", 1) = std::bind(&PlayerScript::End, this);
	}

	void PlayerScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		Vector3 rihght = tr->Right();
		Vector3 up = tr->Up();
		Vector3 foward = tr->Foward();


		//if (Input::GetKey(eKeyCode::RIGHT))
		//{
		//	Vector3 pos = tr->Right();
		//	pos.x += 30.0f * Time::DeltaTime();
		//	tr->SetRight(pos);
		//}
		//if (Input::GetKey(eKeyCode::LEFT))
		//{
		//	Vector3 pos = tr->Right();
		//	pos.x -= 30.0f * Time::DeltaTime();
		//	tr->SetRight(pos);
		//}

		//if (Input::GetKey(eKeyCode::DOWN))
		//{
		//	Vector3 pos = tr->Foward();
		//	pos.x -= 30.0f * Time::DeltaTime();
		//	tr->SetFoward(pos);
		//}
		//if (Input::GetKey(eKeyCode::UP))
		//{
		//	Vector3 pos = tr->Foward();
		//	pos.x += 30.0f * Time::DeltaTime();
		//	tr->SetFoward(pos);
		//}





		if (Input::GetKey(eKeyCode::RIGHT))
		{
			Vector3 pos = tr->GetRotation();
			pos.y += 20.0f * Time::DeltaTime();
			tr->SetRotation(pos);			
		}
		if (Input::GetKey(eKeyCode::LEFT))
		{
			Vector3 pos = tr->GetRotation();
			pos.y -= 20.0f * Time::DeltaTime();
			tr->SetRotation(pos);
		}
		if (Input::GetKey(eKeyCode::UP))
		{
			Vector3 pos = tr->GetPosition();
			Vector3 test = tr->Foward();
			pos.x += test.x;
			pos.y += test.y;
			pos.z += 10.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}

		if (Input::GetKey(eKeyCode::DOWN))
		{
			Vector3 pos = tr->GetPosition();
			Vector3 test = tr->Foward();
			pos.x -= test.x;
			pos.y -= test.y;
			pos.z -= 10.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}



		Animator* animator = GetOwner()->GetComponent<Animator>();


	}

	void PlayerScript::Render()
	{
	}

	void PlayerScript::OnCollisionEnter(Collider2D* collider)
	{
		if (dynamic_cast<GameObject*>(collider->GetOwner()))
		{
			int a = 0;
		}



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
