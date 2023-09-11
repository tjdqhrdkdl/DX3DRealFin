#include "yaActionScript.h"

#include "yaTime.h"
#include "yaInput.h"
#include "yaCollisionManager.h"

#include "yaRigidbody.h"
#include "yaTransform.h"
#include "yaCollider2D.h"

#include "yaPlayer.h"
#include "yaWallScript.h"

#include <assert.h>

#include "yaApplication.h"

extern ya::Application application;

namespace ya
{
	const float defaultJumpForce = 1000.0f;

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
		, mbMoving(false)
		, mbRunning(false)
		, mbJumping(false)
		, mbGrounded(false)
		, mbForwardBlocked(false)
		, mJumpTimer(0.0f)
		, mJumpForce(0.0f)
		, mJumpEvent(nullptr)
		, mGroundEvent(nullptr)
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

		CheckGround();
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
		GameObject* colObj = collider->GetOwner();
		Transform* colTransform = colObj->GetComponent<Transform>();

		GameObject* obj = GetOwner();
		Transform* objTransform = obj->GetComponent<Transform>();

		// 벽 충돌
		if (nullptr != colObj->GetScript<WallScript>())
		{
			Rigidbody* objRigidbody = obj->GetComponent<Rigidbody>();

			Vector3 velocity = objRigidbody->GetVelocity();
			Vector3 pos = objTransform->GetPosition();

			pos -= velocity * Time::DeltaTime();
			objTransform->SetPosition(pos);
		}
	}

	void ActionScript::OnCollisionStay(Collider2D* collider)
	{
		GameObject* colObj = collider->GetOwner();
		Transform* colTransform = colObj->GetComponent<Transform>();

		GameObject* obj = GetOwner();
		Transform* objTransform = obj->GetComponent<Transform>();

		// 벽 충돌
		if (nullptr != colObj->GetScript<WallScript>())
		{
			Rigidbody* objRigidbody = obj->GetComponent<Rigidbody>();

			Vector3 wallNormal = colTransform->Right();

			Vector3 objVelocity = objRigidbody->GetVelocity();
			Vector3 objPos = objTransform->GetPosition();

			Vector3 projvec = wallNormal * objVelocity;
			projvec *= wallNormal;

			objVelocity -= projvec;

			objPos -= objVelocity * Time::DeltaTime();
			objTransform->SetPosition(objPos);
		}
	}

	void ActionScript::OnCollisionExit(Collider2D* collider)
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

		if (mbGrounded)
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
	
	void ActionScript::ForwardCheck()
	{
		Vector3 position = mTransform->GetPosition();
		Vector3 scale = mTransform->GetScale();
		Vector3 colScale = mCollider->GetSize();
		Vector3 velocity = mRigidbody->GetVelocity();

		colScale *= scale;

		Vector3 top = position;
		top.y += colScale.y * 0.5f;

		Vector3 middle = position;

		Vector3 bottom = position;
		bottom.y -= colScale.y * 0.5f;

		Vector3 rayDirection = mTransform->Forward();

		std::vector<eLayerType> layers;
		layers.push_back(eLayerType::Wall);

		RayHit ForwardHit[3];
		ForwardHit[0] = CollisionManager::RayCast(GetOwner(), top, rayDirection, layers);
		ForwardHit[1] = CollisionManager::RayCast(GetOwner(), middle, rayDirection, layers);
		ForwardHit[2] = CollisionManager::RayCast(GetOwner(), bottom, rayDirection, layers);

		for (int i = 0; i < 3; ++i)
		{
			if (velocity.Length() <= ForwardHit[i].length && ForwardHit[i].isHit)
			{
				//mRigidbody->SetVelocity(Vector3::Zero);
				mbForwardBlocked = true;
			}
			else
				mbForwardBlocked = false;
		}

	}

	// 땅, 경사로 체크. 일단 가운데 레이만 사용함..
	void ActionScript::CheckGround()
	{
		Vector3 position = mTransform->GetPosition();
		Vector3 objPos = mCollider->GetCenter();
		Vector3 Scale = mTransform->GetScale();
		Vector3 objScale = mCollider->GetSize();

		position += objPos;
		objScale *= Scale;

		// 지형체크용 레이의 시작점은 포지션의 맨아래에서 시작
		Vector3 rayPosition = position;//mTransform->GetPosition();
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
	
		// 땅에 붙었는지 체크하기 위한 플래그
		bool bGroundFlag = false;

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
				{ // 바닥으로 부터 거리가 0.001f 미만일때
					if (!mbGrounded && mJumpTimer < 0.0f)
					{	// 착지하는지 체크
						// ground가 아닌 상태(공중일 때)에서 jumpTimer가 종료 됐을 때(jump가 끝났을 때) jump->ground로 상태가 변화한 것으로 판단한다.
						// jumpTimer가 0.0f 보다 크면 jump를 시작한 것이므로 jump 상태를 false로 바꾸지 않는다.
						SetJumping(false);

						if (mGroundEvent != nullptr)
							mGroundEvent();
					}

					mbGrounded = true;
					bGroundFlag |= true;

					// 물체의 y 크기의 절반과 가운데에서 쏜 레이의 길이를 뺐을때
					// 수치가 0보다 크면 물체는 땅을 뚫었다고 판단.
					float overPos = objScale.y / 2.f - CorrectionHit.length;
					Vector3 correctionPos = mTransform->GetPosition();

					// 땅을 뚫을때
					if (0.f < overPos)
					{
						correctionPos.y += overPos;
						//correctionPos -= objPos;
						mTransform->SetPosition(correctionPos);
					}
				}
			}
		}

		if (!bGroundFlag)
		{
			if (mbGrounded)
				if(mJumpEvent != nullptr)
					mJumpEvent();

			mbGrounded = false;
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