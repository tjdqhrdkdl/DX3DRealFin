#include "yaActionScript.h"

#include "yaTime.h"
#include "yaInput.h"
#include "yaCollisionManager.h"

#include "yaObject.h"
#include "yaRigidbody.h"
#include "yaTransform.h"
#include "yaCollider2D.h"

#include "yaPlayer.h"
#include "yaWallScript.h"
#include "yaWallCheckObject.h"
#include "yaWallCheckScript.h"

#include <assert.h>

#include "yaApplication.h"

extern ya::Application application;

namespace ya
{
	const float defaultJumpForce = 200.0f;

	ActionScript::ActionScript()
		: Script()
		, mTarget(nullptr)
		, mCheck(nullptr)
		, mRigidbody(nullptr)
		, mTransform(nullptr)
		, mCollider(nullptr)
		, mSpeed(200.0f)

		, mDirection(Vector3::Zero)
		, mRotateDirection(Vector3::Zero)
		, mJumpTimer(0.0f)
		, mJumpForce(0.0f)
		, mbForwardBlocked(false)
	{
	}

	ActionScript::~ActionScript()
	{
	}

	void ActionScript::Initialize()
	{
		GameObject* obj = GetOwner();
		assert(obj != nullptr);

		mTransform = obj->GetComponent<Transform>();
		mRigidbody = obj->GetComponent<Rigidbody>();
		mCollider = obj->GetComponent<Collider2D>();

		WallCheckObject* checkObj = object::Instantiate<WallCheckObject>(eLayerType::WallCheckCollision, obj->GetScene());
		assert(checkObj != nullptr);
		mCheck = checkObj;
		checkObj->SetName(L"WallCheck");
		checkObj->SetParentObj(obj);

		Transform* checkTransform = mCheck->GetComponent<Transform>();
		checkTransform->SetScale(mTransform->GetScale());

		Collider2D* checkCol = mCheck->AddComponent<Collider2D>();

		Collider2D* ownerCol = obj->GetComponent<Collider2D>();
		if (ownerCol != nullptr)
		{
			checkCol->SetType(ownerCol->GetColliderType());
			checkCol->SetCenter(ownerCol->GetCenter());
			checkCol->SetSize(ownerCol->GetSize());
		}
		else
		{
			checkCol->SetType(eColliderType::Box);
			checkCol->SetCenter(Vector3(0.f, 1.2f, 0.f));
			checkCol->SetSize(Vector3(1.0, 3.0f, 1.0f));
		}
	}

	void ActionScript::Update()
	{
		assert(GetOwner() != nullptr);

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
		/*if (mRigidbody->IsGround())
		{
			wchar_t szFloat[50] = {};
			swprintf_s(szFloat, 50, L"ground");
			TextOut(application.GetHdc(), 800, 150, szFloat, wcslen(szFloat));
		}*/
	}

	void ActionScript::OnCollisionEnter(Collider2D* collider)
	{
	}

	void ActionScript::OnCollisionStay(Collider2D* collider)
	{
	}

	void ActionScript::OnCollisionExit(Collider2D* collider)
	{
	}

	/// <summary> limit velocity를 늘려서 최대 속도를 변경한다. 인자없을시 default값(40.0f)으로 설정됨. </summary>
	/// <param name="velocity">최대속도</param>
	void ActionScript::Velocity(const float velocity)
	{
		Vector3 limitVelocity = mRigidbody->GetLimitVelocity();
		mRigidbody->SetLimitVelocity(Vector3(velocity, limitVelocity.y, velocity));
	}

	/// <summary> owner rigidbody를 통한 이동 </summary>
	/// <param name="dir">방향</param>
	/// <param name="speed">속도</param>
	void ActionScript::Move(const Vector3 dir, float speed)
	{
		if (mRigidbody == nullptr)
		{
			assert(mRigidbody != nullptr);
			return;
		}

		mDirection = dir;

		if (speed > 0.0f)
			mRigidbody->AddForce(speed * mDirection);
		else
			mRigidbody->AddForce(mSpeed * mDirection);
		
	}

	/// <summary>
	/// owner rigidbody를 통한 회전
	/// </summary>
	/// <param name="dir">방향</param>
	/// <param name="speed">속도</param>
	void ActionScript::Rotate(const Vector3 dir, float speed)
	{
		if(mTransform == nullptr)
		{
			assert(mTransform != nullptr);
			return;
		}

		Vector3 rot = mTransform->GetRotation();
		mRotateDirection = dir;
		
		if (speed > 0.0f)
		{
			rot += speed * 4.0f * mRotateDirection * Time::DeltaTime();
		}
		else
		{
			rot += mSpeed * 4.0f * mRotateDirection * Time::DeltaTime();
		}

		mTransform->SetRotation(rot);
	}

	/// <summary>
	/// rigidbody를 통한 점프
	/// </summary>
	/// <param name="force">점프 force</param>
	void ActionScript::Jump(float force)
	{
		if (mRigidbody == nullptr)
		{
			assert(mRigidbody != nullptr);
			return;
		}

		if (!mRigidbody->IsGrounded())
			return;

		// force가 지정되었을 때(음수가 아닌경우) force로 사용
		if (force > 0.0f)
		{
			mJumpForce = force;
		}
		// force가 지정되지 않았을 때 기존의 force 사용
		else
		{
  			mJumpForce = defaultJumpForce;
		}

		mJumpTimer = 0.2f;
		mRigidbody->AddJumpCount();
	}

	void ActionScript::JumpDouble(float force)
	{
		if (mRigidbody == nullptr)
		{
			assert(mRigidbody != nullptr);
			return;
		}

		mJumpTimer = 0.25f;
		mRigidbody->AddJumpCount();
	}

	bool ActionScript::ForwardCheck(Vector3 movement)
	{
		Vector3 position = mTransform->GetPosition();
		Vector3 scale = mTransform->GetScale();
		Vector3 colScale = mCollider->GetSize();
		Vector3 velocity = movement * Time::DeltaTime();
		Vector3 dir = movement;
		dir.Normalize();

		float velocityLength = velocity.Length();
		float positionLength = position.Length();

		colScale *= scale;

		Vector3 top = position;
		top.y += colScale.y * 0.5f;

		Vector3 middle = position;

		Vector3 bottom = position;
		bottom.y -= colScale.y * 0.5f;

		Vector3 rayDirection = dir;

		std::vector<eLayerType> layers;
		layers.push_back(eLayerType::Monster);

		RayHit ForwardHit[3];
		ForwardHit[0] = CollisionManager::RayCast(GetOwner(), top, rayDirection, layers);
		ForwardHit[1] = CollisionManager::RayCast(GetOwner(), middle, rayDirection, layers);
		ForwardHit[2] = CollisionManager::RayCast(GetOwner(), bottom, rayDirection, layers);

		for (int i = 0; i < 3; ++i)
		{
			if (velocity.Length() <= ForwardHit[i].length && ForwardHit[i].isHit)
				return true;
		}

		return false;
	}
}