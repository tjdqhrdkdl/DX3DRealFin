#include "yaActionScript.h"

#include "yaTime.h"

#include "yaRigidbody.h"
#include "yaTransform.h"

#include <assert.h>

namespace ya
{
	const float defaultJumpForce = 400.0f;

	ActionScript::ActionScript()
		: Script()
		, mRigidbody(nullptr)
		, mTransform(nullptr)
		, mSpeed(120.0f)
		, mDirection(Vector3::Zero)
		, mJumpTimer(0.0f)
		, mJumpForce(0.0f)
		, mTarget(nullptr)
	{
	}
	ActionScript::~ActionScript()
	{
	}
	void ActionScript::Initalize()
	{
		GameObject* obj = GetOwner();
		assert(obj != nullptr);

		mTransform = obj->GetComponent<Transform>();
		mRigidbody = obj->AddComponent<Rigidbody>();
	}

	void ActionScript::Update()
	{
		assert(GetOwner() != nullptr);

		if (mTransform == nullptr || mRigidbody == nullptr)
		{
			return;
		}

		if (mJumpTimer > 0.0f)
		{
			mJumpTimer -= Time::DeltaTime();
			mRigidbody->AddForce(Vector3(0.0f, mJumpForce, 0.0f));
		}
	}

	void ActionScript::FixedUpdate()
	{
	}

	void ActionScript::Render()
	{
	}

	/// <summary>
	/// owner rigidbody�� ���� �̵�
	/// </summary>
	/// <param name="dir">����</param>
	/// <param name="speed">�ӵ�</param>
	void ActionScript::Move(const Vector3 dir, float speed)
	{
		if (mRigidbody == nullptr)
		{
			assert(mRigidbody != nullptr);
			return;
		}

		if (speed > 0.0f)
		{
			mJumpForce = speed;
		}
		else
		{
			mRigidbody->AddForce(mSpeed * dir);
		}
		
	}

	/// <summary>
	/// owner rigidbody�� ���� ȸ��
	/// </summary>
	/// <param name="dir">����</param>
	/// <param name="speed">�ӵ�</param>
	void ActionScript::Rotate(const Vector3 dir, float speed)
	{
		if(mTransform == nullptr)
		{
			assert(mTransform != nullptr);
			return;
		}

		Vector3 rot = mTransform->GetRotation();
		
		if (speed > 0.0f)
		{
			rot += speed * 4.0f * dir * Time::DeltaTime();
		}
		else
		{
			rot += mSpeed * 4.0f * dir * Time::DeltaTime();
		}

		mTransform->SetRotation(rot);
	}

	/// <summary>
	/// rigidbody�� ���� ����
	/// </summary>
	/// <param name="force">���� force</param>
	void ActionScript::Jump(float force)
	{
		if (mRigidbody == nullptr)
		{
			assert(mRigidbody != nullptr);
			return;
		}

		// force�� �����Ǿ��� ��(������ �ƴѰ��) force�� ���
		if (force > 0.0f)
		{
			mJumpForce = force;
		}
		// force�� �������� �ʾ��� �� ������ force ���
		else
		{
			mJumpForce = defaultJumpForce;
		}

		if (mRigidbody->IsGround())
		{
			mRigidbody->SetGround(false);
			mJumpTimer = 0.1f;
		}
	}

	/// <summary>
	/// ����
	/// </summary>
	void ActionScript::Attack()
	{
	}

	/// <summary>
	/// ����
	/// </summary>
	void ActionScript::Deflect()
	{
	}

	/// <summary>
	/// �и�
	/// </summary>
	void ActionScript::Parrying()
	{
	}
}