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
			//mRigidbody->SetGround(false);
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
		Vector3 position = mTransform->GetPosition();
		Vector3 objScale = mTransform->GetScale();

		// 지형체크용 레이캐스트의 시작점은 포지션의 맨아래에서 시작
		Vector3 rayPosition = mTransform->GetPosition();//rayDirection * position.Length();
		Matrix rotation = mTransform->GetRotationMatrix();
		rayPosition.y -= objScale.y / 2.f;

		// 순서대로 북서, 북동, 남서, 남동
		Vector3 nw = rayPosition;
		nw.x -= objScale.x / 2.f;
		nw.z += objScale.z / 2.f;
		//nw = Vector3::Transform(nw, rotation);

		Vector3 ne = rayPosition;
		ne.x += objScale.x / 2.f;
		ne.z += objScale.z / 2.f;
		//ne = Vector3::Transform(ne, rotation);

		Vector3 sw = rayPosition;
		sw.x -= objScale.x / 2.f;
		sw.z -= objScale.z / 2.f;
		//sw = Vector3::Transform(sw, rotation);

		Vector3 se = rayPosition;
		se.x += objScale.x / 2.f;
		se.z -= objScale.z / 2.f;
		//se = Vector3::Transform(se, rotation);

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

		mRigidbody->SetGround(false);

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

				if (CheckHit[i].length < 0.1f)
					mRigidbody->SetGround(true);

				if (mRigidbody->IsGround())
				{
					float overPos = objScale.y / 2.f - CorrectionHit.length;
					Vector3 correctionPos = position;

					// 땅을 뚫을때
					if (0.f < overPos)
						correctionPos.y += overPos;

					mTransform->SetPosition(correctionPos);
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