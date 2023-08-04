#include "yaMoveScript.h"

#include "yaInput.h"

#include "yaPlayer.h"
#include "yaTransform.h"
#include "yaActionScript.h"

namespace ya
{
	const float frontTheta = 2.0f; // �����̶�� �νĵǴ� �ּ� ���� ����

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

		// player�� �ٶ� ����� camera ������ ������ ���Ѵ�.
		Quaternion quater = Quaternion::FromToRotation(dir, Vector3(cameraPos.x - pos.x, pos.y, cameraPos.z - pos.z));
		Vector3 quaterToEuler = quater.ToEuler();
		Vector3 theta = quaterToEuler * 180.0f / XM_PI;

		if (mbRotate)
		{
			if (abs(theta.y) < frontTheta)	
			{	// �����Ϸ��� ����� player�� forward�� ��������� ȸ���� ������.
				mbRotate = false;
				tr->SetRotation(Vector3(0.0f, rot.y + theta.y, 0.0f));
			}
			else
			{	// �����Ϸ��� ����� player�� forward�� ������� �� ���� ȸ���Ѵ�. theta ���� ���� ȸ�� ������ �����Ѵ�.
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
			{	// �����Ϸ��� ����� ���� ���̰� ���� ȸ����Ų��.
				mbRotate = true;
			}
			else							
			{	// �����Ϸ��� ����� ���� ���̰� ������ player�� forward �������� �̵���Ų��.
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
