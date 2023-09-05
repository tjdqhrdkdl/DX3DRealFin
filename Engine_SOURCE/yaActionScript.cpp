#include "yaActionScript.h"

#include "yaTime.h"
#include "yaInput.h"
#include "yaCollisionManager.h"

#include "yaRigidbody.h"
#include "yaTransform.h"

#include "yaPlayer.h"

#include <assert.h>

namespace ya
{
	const float defaultJumpForce = 400.0f;

	ActionScript::ActionScript()
		: Script()
		, mTarget(nullptr)
		, mRigidbody(nullptr)
		, mTransform(nullptr)
		, mSpeed(100.0f)
		, mDirection(Vector3::Zero)
		, mRotateDirection(Vector3::Zero)
		, mGroundDistance(0.f)
		, mGroundNormal(Vector3::Zero)
		, mGroundSlopeAngle(0.f)
		, mForwardSlopeAngle(0.f)
		, mGroundCross(0.f)
		, mMoving(false)
		, mRunning(false)
		, mGrounded(false)
		, mJumpTimer(0.0f)
		, mJumpForce(0.0f)
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
	}

	void ActionScript::Update()
	{
		assert(GetOwner() != nullptr);

		CheckGround();

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
	/// owner rigidbody를 통한 이동
	/// </summary>
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

		if (mRigidbody->IsGround())
		{
			mRigidbody->SetGround(false);
			mJumpTimer = 0.1f;
		}
	}

	/// <summary>
	/// 공격
	/// </summary>
	void ActionScript::Attack()
	{
	}

	/// <summary>
	/// 막기
	/// </summary>
	void ActionScript::Deflect()
	{
	}

	/// <summary>
	/// 패링
	/// </summary>
	void ActionScript::Parrying()
	{
		// 체간 증가

		// 패링 이펙트 발생
	}
	
	// 땅, 경사로 체크
	void ActionScript::CheckGround()
	{
		Vector3 rayPosition = mTransform->GetPosition();
		Vector3 objScale = mTransform->GetScale();
		rayPosition.y -= objScale.y / 2.f;
		//rayPosition.y += 0.1f;

		std::vector<eLayerType> layers = {};
		layers.push_back(eLayerType::Ground);

		Vector3 direction = -(mTransform->Up());

		RayHit Hit = CollisionManager::RayCast(GetOwner(), rayPosition, direction, layers);

		mRigidbody->SetGround(false);

		if (Hit.isHit)
		{
            Transform* hitTransform = Hit.hitObj->GetComponent<Transform>();

			mGroundNormal = hitTransform->Up();

			float groundRadian = mGroundNormal.Dot(Vector3::Up);
			groundRadian = acos(groundRadian);

			float forwardRadian = mGroundNormal.Dot(mDirection);
			forwardRadian = acos(forwardRadian);

			mGroundSlopeAngle = groundRadian;
			mForwardSlopeAngle = forwardRadian - 1.5708f;

			if (Hit.length < 0.5f)
				mRigidbody->SetGround(true);
		}

		mGroundCross = mGroundNormal.Cross(Vector3::Up);

		if (Vector3::Zero != mGroundCross)
		{
			Vector3 velocity = mRigidbody->GetVelocity();
			Vector3 position = mTransform->GetPosition();

			Vector3 axis = mGroundCross;

			Matrix mat = Matrix::CreateFromAxisAngle(axis, -mGroundSlopeAngle);
			mRigidbody->SetRotateDirection(mat);
			//forward = Vector3::Transform(forward, mat);

			//mDirection = forward;
		}
	}

	void ActionScript::UpdatePhysics()
	{
		if (mGrounded)
		{
			mRigidbody->SetGround(true);
		}
		else
		{
			mRigidbody->SetGround(false);
		}
	}
}