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

		constexpr float speed = 100.f;

		if (Input::GetKey(eKeyCode::W))
		{
			pos += tr->Forward() * speed;
		}
		else if (Input::GetKey(eKeyCode::A))
		{
			pos -= tr->Right() * speed;
		}
		else if (Input::GetKey(eKeyCode::S))
		{
			pos -= tr->Forward() * speed;
		}
		else if (Input::GetKey(eKeyCode::D))
		{
			pos += tr->Right() * speed;
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