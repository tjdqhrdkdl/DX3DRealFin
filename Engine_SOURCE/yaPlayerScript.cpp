#include "yaPlayerScript.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaAnimator.h"

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
		//Vector3 rot = tr->GetRotation();
		//rot.z += 10.0f * Time::DeltaTime();
		//tr->SetRotation(rot);

		//if (Input::GetKeyState(eKeyCode::R) == eKeyState::PRESSED)
		//{
		//	Vector3 rot = tr->GetRotation();
		//	rot.z += 10.0f * Time::DeltaTime();
		//	tr->SetRotation(rot);
		//}


		if (Input::GetKey(eKeyCode::RIGHT))
		{
			Vector3 pos = tr->GetPosition();
			pos.x += 60.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}
		if (Input::GetKey(eKeyCode::LEFT))
		{
			Vector3 pos = tr->GetPosition();
			pos.x -= 60.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}

		if (Input::GetKey(eKeyCode::DOWN))
		{
			Vector3 pos = tr->GetPosition();
			pos.z -= 60.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}
		if (Input::GetKey(eKeyCode::UP))
		{
			Vector3 pos = tr->GetPosition();
			pos.z += 60.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}

		if (Input::GetKey(eKeyCode::SPACE))
		{
			Vector3 pos = tr->GetPosition();
			pos.y += 60.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}

		if (Input::GetKey(eKeyCode::M))
		{
			Vector3 pos = tr->GetPosition();
			pos.y -= 60.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}

		/*Transform* tr = GetOwner()->GetComponent<Transform>();

		Vector3 pos = tr->GetPosition();

		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos.x += 3.0f * Time::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos.x -= 3.0f * Time::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos.y += 3.0f * Time::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos.y -= 3.0f * Time::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
		{
			pos.z += 3.0f * Time::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
		{
			pos.z -= 3.0f * Time::DeltaTime();
		}

		tr->SetPosition(pos);*/
		Animator* animator = GetOwner()->GetComponent<Animator>();
		//if (Input::GetKey(eKeyCode::N_1))
		//{
		//	animator->Play(L"MoveDown");
		//}


		// 마우스의 이동 거리(픽셀) 측정 - 반영 - 마우스 위치 고정 
		
		

			//디버깅시에 문제생기는 부분 막음.
			if (Time::DeltaTime() < 0.1f)
			{
				//두번 계산해줄 것이다.
				//카메라를 원점(플레이어) 기준으로 먼저 위치를 이동시키고
				//카메라 오브젝트의 회전을 바꿔준다.



				//위치 이동 다음 카메라를 회전 시켜서 항상 타겟을 바라보게끔.
				//목표 : forward가 newForward가 되는 것.
				//두 벡터의 외적을 통해서 수직이 되는 벡터를 구한다.
				//두 벡터의 내적을 통해서 두 벡터 사이의 코사인 세타를 구한다.
				//수직이 되는 벡터를 축으로, 세타만큼 회전시킨다.

				Vector3 forward = Vector3::Forward;
				Vector3 pos = tr->GetPosition();
				Vector3 targetPos = mTarget->GetComponent<Transform>()->GetPosition();

				Vector3 newForward = targetPos - pos;


				newForward.Normalize();
				Vector3 axis = forward.Cross(newForward);
				axis.Normalize();

				float c = forward.Dot(newForward);
				float vecTheta = acosf(c);
				float s = sinf(vecTheta);

				//축 벡터에 기반한 rotation matrix.
				Matrix rotationFromAxis = {
					c + (1 - c) * axis.x * axis.x			, (1 - c) * axis.x * axis.y + s * axis.z , (1 - c) * axis.x * axis.z - s * axis.y	, 0,
					(1 - c) * axis.x * axis.y - s * axis.z  , c + (1 - c) * axis.y * axis.y			 , (1 - c) * axis.y * axis.z + s * axis.x	, 0,
					(1 - c) * axis.x * axis.z + s * axis.y  , (1 - c) * axis.y * axis.z - s * axis.x ,  c + (1 - c) * axis.z * axis.z			, 0,
					0										, 0									     , 0										, 1
				};

				tr->RotateFromAxis(rotationFromAxis);


				



			}

		
	}


	void PlayerScript::Render()
	{
	}

	void PlayerScript::OnCollisionEnter(Collider2D* collider)
	{
		int a = 0;
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
