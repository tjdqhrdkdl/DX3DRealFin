#include "PhysXPlayerScript.h"
#include "yaInput.h"
#include "yaGameObject.h"

namespace ya
{
	PhysXPlayerScript::PhysXPlayerScript()
	{
	}
	PhysXPlayerScript::~PhysXPlayerScript()
	{
	}
	void PhysXPlayerScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		Vector3 pos = tr->GetLocalPosition();

		constexpr float speed = 1.f;

		bool changed = false;
		if (Input::GetKey(eKeyCode::UP))
		{
			pos += tr->Forward() * speed;
			changed = true;
		}
		else if (Input::GetKey(eKeyCode::LEFT))
		{
			pos -= tr->Right() * speed;
			changed = true;
		}
		else if (Input::GetKey(eKeyCode::DOWN))
		{
			pos -= tr->Forward() * speed;
			changed = true;
		}
		else if (Input::GetKey(eKeyCode::RIGHT))
		{
			pos += tr->Right() * speed;
			changed = true;
		}

		if (changed)
		{
			tr->SetLocalPosition(pos);
		}

	}
	void PhysXPlayerScript::OnCollisionEnter(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		int a = 3;
	}
	void PhysXPlayerScript::OnCollisionStay(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		int a = 3;
	}
	void PhysXPlayerScript::OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		int a = 3;
	}
}