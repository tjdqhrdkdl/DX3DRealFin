#include "yaMoveScript.h"

#include "yaInput.h"

#include "yaPlayer.h"
#include "yaTransform.h"
#include "yaActionScript.h"

namespace ya
{
	const float frontTheta = 2.0f; // 정면이라고 인식되는 최소 각도 차이

	MoveScript::MoveScript()
		: Script()
		, mbRotate(false)
		, mLastDir(eDirection::Forward)
	{
	}

	MoveScript::~MoveScript()
	{
	}

	void MoveScript::Initalize()
	{
	}

	void MoveScript::Update()
	{
		Player* player = dynamic_cast<Player*>(GetOwner());

		Transform* tr = player->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		Vector3 rot = tr->GetRotation();

		GameObject* camera = player->GetCamera();
		Transform* cameraTr = camera->GetComponent<Transform>();
		Vector3 cameraPos = cameraTr->GetPosition();

		ActionScript* action = GetOwner()->GetScript<ActionScript>();
		
		if (action == nullptr)
			return;

		Vector3 dir = -tr->Forward();
		if (mLastDir == eDirection::Forward)
			dir = -tr->Forward();
		if (mLastDir == eDirection::Back)
			dir = tr->Forward();
		if (mLastDir == eDirection::Right)
			dir = tr->Right();
		if (mLastDir == eDirection::Left)
			dir = -tr->Right();

		// player가 바라볼 방향과 camera 사이의 각도를 구한다.
		Quaternion quater = Quaternion::FromToRotation(dir, Vector3(cameraPos.x - pos.x, pos.y, cameraPos.z - pos.z));
		Vector3 quaterToEuler = quater.ToEuler();
		Vector3 theta = quaterToEuler * 180.0f / XM_PI;

		if (mbRotate)
		{
			if (abs(theta.y) < frontTheta)	
			{	// 진행하려는 방향과 player의 forward가 비슷해지면 회전이 끝난다.
				mbRotate = false;
				tr->SetRotation(Vector3(0.0f, rot.y + theta.y, 0.0f));
			}
			else
			{	// 진행하려는 방향과 player의 forward가 비슷해질 때 까지 회전한다. theta 각에 따라 회전 방향을 결정한다.
				if (theta.y > 0.0f)
					action->Rotate(Vector3(0.0f, 1.0f, 0.0f));
				else
					action->Rotate(Vector3(0.0f, -1.0f, 0.0f));
			}
		}

		if (Input::GetKey(eKeyCode::W))
		{
			mLastDir = eDirection::Forward;

			if (abs(theta.y) > frontTheta)	
			{	// 진행하려는 방향과 각도 차이가 날때 회전시킨다.
				mbRotate = true;
			}
			else							
			{	// 진행하려는 방향과 각도 차이가 없으면 player의 forward 방향으로 이동시킨다.
				action->Move(tr->Forward());
			}
		}
		if (Input::GetKey(eKeyCode::S))
		{
			mLastDir = eDirection::Back;

			if (abs(theta.y) > frontTheta)
			{
				mbRotate = true;
			}
			else
			{
				action->Move(tr->Forward());
			}
		}

		if (Input::GetKey(eKeyCode::D))
		{
			mLastDir = eDirection::Right;

			if (abs(theta.y) > frontTheta)
			{
				mbRotate = true;
			}
			else
			{
				action->Move(tr->Forward());
			}
		}
		if (Input::GetKey(eKeyCode::A))
		{
			mLastDir = eDirection::Left;

			if (abs(theta.y) > frontTheta)
			{
				mbRotate = true;
			}
			else
			{
				action->Move(tr->Forward());
			}
		}

		if (Input::GetKey(eKeyCode::SPACE))
		{
			action->Jump();
		}
	}


	void MoveScript::Render()
	{
	}

	void MoveScript::OnCollisionEnter(Collider2D* collider)
	{
	}

	void MoveScript::OnCollisionStay(Collider2D* collider)
	{
	}

	void MoveScript::OnCollisionExit(Collider2D* collider)
	{
	}
}
