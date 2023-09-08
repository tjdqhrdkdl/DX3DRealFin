#include "yaActionScript.h"

#include "yaTime.h"
#include "yaInput.h"
#include "yaCollisionManager.h"

#include "yaRigidbody.h"
#include "yaTransform.h"
#include "yaCollider2D.h"

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
		, mCollider(nullptr)
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
		, mJumping(false)
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
		mCollider = obj->GetComponent<Collider2D>();
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

		if (mGrounded)
		{
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
	
	// 땅, 경사로 체크. 일단 가운데 레이만 사용함..
	void ActionScript::CheckGround()
	{
		Vector3 position = mTransform->GetPosition();
		Vector3 Scale = mTransform->GetScale();
		Vector3 objScale = mCollider->GetSize();
		objScale *= Scale;

		// 지형체크용 레이의 시작점은 포지션의 맨아래에서 시작
		Vector3 rayPosition = mTransform->GetPosition();
		rayPosition.y -= objScale.y / 2.f;

		// z의 크기 절반 만큼의 크기를 가진 forward 벡터와 위에서 설정한 위치 벡터를 더하여
		// forward 방향을 가진 z크기의 절반 값을 가진 벡터를 구한다
		Vector3 forward = mTransform->Forward();
		forward = forward * objScale.z / 2.f;
		Vector3 forwardPos = rayPosition + forward;

		// 순서대로 북서, 북동, 남서, 남동
		Vector3 nw = forwardPos;
		nw.x -= objScale.x / 2.f;

		Vector3 ne = forwardPos;
		ne.x += objScale.x / 2.f;

		Vector3 sw = forwardPos;
		sw.x -= objScale.x / 2.f;
		sw.z -= objScale.z;

		Vector3 se = forwardPos;
		se.x += objScale.x / 2.f;
		se.z -= objScale.z;

		std::vector<eLayerType> layers = {};
		layers.push_back(eLayerType::Ground);

		Vector3 direction = -(mTransform->Up());

		// 지형 체크용
		RayHit CheckHit[4] = {};
		CheckHit[0] = CollisionManager::RayCast(GetOwner(), nw, direction, layers);
		CheckHit[1] = CollisionManager::RayCast(GetOwner(), ne, direction, layers);
		CheckHit[2] = CollisionManager::RayCast(GetOwner(), sw, direction, layers);
		CheckHit[3] = CollisionManager::RayCast(GetOwner(), se, direction, layers);

		// 지형 보정용
		RayHit CorrectionHit = CollisionManager::RayCast(GetOwner(), position, direction, layers);

		mGrounded = false;

		for (int i = 0; i < 4; ++i)
		{
			if (CheckHit[i].isHit)
			{
				Transform* hitTransform = CheckHit[i].hitObj->GetComponent<Transform>();

				mGroundNormal = hitTransform->Up();

				float groundRadian = mGroundNormal.Dot(Vector3::Up);
				groundRadian = acos(groundRadian);

				float forwardRadian = mGroundNormal.Dot(mDirection);
				forwardRadian = acos(forwardRadian);

				mGroundSlopeAngle = groundRadian;
				mForwardSlopeAngle = forwardRadian - 1.5708f;

				// 밑바닥을 기준으로 구한 거리
				float CorrectionLength = CorrectionHit.length - objScale.y / 2.f;

				if (CorrectionLength < 0.001f)
				{
					mGrounded = true;

					// 물체의 y 크기의 절반과 가운데에서 쏜 레이의 길이를 뺐을때
					// 수치가 0보다 크면 물체는 땅을 뚫었다고 판단.
					float overPos = objScale.y / 2.f - CorrectionHit.length;
					Vector3 correctionPos = position;

					// 땅을 뚫을때
					if (0.f < overPos)
					{
						correctionPos.y += overPos;
						mTransform->SetPosition(correctionPos);
					}
				}
				break;
			}
		}

		mGroundCross = mGroundNormal.Cross(Vector3::Up);

		if (Vector3::Zero != mGroundCross)
		{
			Vector3 axis = mGroundCross;

			Matrix mat = Matrix::CreateFromAxisAngle(axis, -mGroundSlopeAngle);
			mRigidbody->SetRotateDirection(mat);
		}
	}
}