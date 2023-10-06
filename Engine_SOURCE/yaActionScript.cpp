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
	const float defaultJumpForce = 400.0f;

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
		, mJumpEvent(nullptr)
		, mGroundEvent(nullptr)
		, mbJumpDouble(false)
		, mJumpTime(0.2f)
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

		WallCheckObject* checkObj = object::Instantiate<WallCheckObject>(eLayerType::WallCheckCollision);
		assert(checkObj != nullptr);
		mCheck = checkObj;
		checkObj->SetName(L"WallCheck");
		checkObj->SetParentObj(obj);

		Collider2D* checkCol = mCheck->AddComponent<Collider2D>();
		Transform* checkTransform = mCheck->GetComponent<Transform>();
		
		checkTransform->SetScale(Vector3(1.f, 1.f ,1.f));

		checkCol->SetType(eColliderType::Box);
		checkCol->SetCenter(Vector3(0.f, 1.2f, 0.f));
		checkCol->SetSize(Vector3(1.0, 3.0f, 1.0f));
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

		if (mRigidbody->IsGrounded())
		{
			mJumpTimer = mJumpTime;
		}
	}

	void ActionScript::JumpDouble(float force)
	{
		if (mRigidbody == nullptr)
		{
			assert(mRigidbody != nullptr);
			return;
		}

		mJumpTimer = 0.1f;
		Jump(force);

		mbJumpDouble = false;

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

		position += dir * 5.f;

		colScale *= scale;

		Vector3 top = position;
		top.y += colScale.y * 0.5f;

		Vector3 middle = position;

		Vector3 bottom = position;
		bottom.y -= colScale.y * 0.5f;

		Vector3 rayDirection = dir;

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
				if (!mbForwardBlocked)
				{
					mbJumpDouble = true;
				}

				mbForwardBlocked = true;
			}
			else
				if (mbForwardBlocked)
				{
					mbJumpDouble = false;
				}

				mbForwardBlocked = false;
		}

		return false;
	}


	//// 땅, 경사로 체크. 일단 가운데 레이만 사용함..
	//void ActionScript::CheckGround()
	//{
	//	Vector3 position = mTransform->GetPosition();
	//	Vector3 objPos = mCollider->GetCenter();
	//	Vector3 Scale = mTransform->GetScale();
	//	Vector3 objScale = mCollider->GetSize();

	//	position += objPos;
	//	objScale *= Scale;

	//	// 지형체크용 레이의 시작점은 포지션의 맨아래에서 시작
	//	Vector3 rayPosition = position;
	//	rayPosition.y -= objScale.y / 2.f;

	//	// z의 크기 절반 만큼의 크기를 가진 forward 벡터와 위에서 설정한 위치 벡터를 더하여
	//	// forward 방향을 가진 z크기의 절반 값을 가진 벡터를 구한다
	//	Vector3 forward = mTransform->Forward();
	//	forward = forward * objScale.z / 2.f;
	//	Vector3 forwardPos = rayPosition + forward;

	//	// 순서대로 북서, 북동, 남서, 남동
	//	//Vector3 nw = forwardPos;
	//	//nw.x -= objScale.x / 2.f;

	//	//Vector3 ne = forwardPos;
	//	//ne.x += objScale.x / 2.f;

	//	//Vector3 sw = forwardPos;
	//	//sw.x -= objScale.x / 2.f;
	//	//sw.z -= objScale.z;

	//	//Vector3 se = forwardPos;
	//	//se.x += objScale.x / 2.f;
	//	//se.z -= objScale.z;

	//	std::vector<eLayerType> layers = {};
	//	layers.push_back(eLayerType::Ground);

	//	Vector3 direction = -(mTransform->Up());

	//	// 지형 체크용
	//	//RayHit CheckHit[4] = {};
	//	//CheckHit[0] = CollisionManager::RayCast(GetOwner(), nw, direction, layers);
	//	//CheckHit[1] = CollisionManager::RayCast(GetOwner(), ne, direction, layers);
	//	//CheckHit[2] = CollisionManager::RayCast(GetOwner(), sw, direction, layers);
	//	//CheckHit[3] = CollisionManager::RayCast(GetOwner(), se, direction, layers);

	//	// 지형 보정용
	//	RayHit CorrectionHit = CollisionManager::RayCast(GetOwner(), position, direction, layers);

	//	bool bGroundFlag = false;
	//
	//	if (CorrectionHit.isHit)
	//	{
	//		// 땅에 붙었는지 체크하기 위한 플래그
	//		Transform* hitTransform = CorrectionHit.hitObj->GetComponent<Transform>();

	//		mGroundNormal = hitTransform->Up();

	//		float groundRadian = mGroundNormal.Dot(Vector3::Up);
	//		groundRadian = acos(groundRadian);

	//		float forwardRadian = mGroundNormal.Dot(mDirection);
	//		forwardRadian = acos(forwardRadian);

	//		mGroundSlopeAngle = groundRadian;
	//		mForwardSlopeAngle = forwardRadian - 1.5708f;

	//		// 밑바닥을 기준으로 구한 거리
	//		float CorrectionLength = CorrectionHit.length - objScale.y / 2.f;

	//		if (CorrectionLength < 0.001f)
	//		{ // 바닥으로 부터 거리가 0.001f 미만일때
	//			if (!mbGrounded && mJumpTimer < 0.0f)
	//			{	// 착지하는지 체크
	//				// ground가 아닌 상태(공중일 때)에서 jumpTimer가 종료 됐을 때(jump가 끝났을 때) jump->ground로 상태가 변화한 것으로 판단한다.
	//				// jumpTimer가 0.0f 보다 크면 jump를 시작한 것이므로 jump 상태를 false로 바꾸지 않는다.
	//				SetJumping(false);

	//				if (mGroundEvent != nullptr)
	//					mGroundEvent();
	//			}

	//			mbGrounded = true;
	//			bGroundFlag |= true;

	//			// 물체의 y 크기의 절반과 가운데에서 쏜 레이의 길이를 뺐을때
	//			// 수치가 0보다 크면 물체는 땅을 뚫었다고 판단.
	//			float overPos = objScale.y / 2.f - CorrectionHit.length;
	//			Vector3 correctionPos = mTransform->GetPosition();

	//			// 땅을 뚫을때
	//			if (0.f < overPos)
	//			{
	//				correctionPos.y += overPos;
	//				mTransform->SetPosition(correctionPos);
	//			}
	//		}
	//	}

	//	if (!bGroundFlag)
	//	{
	//		if (mbGrounded)
	//			if(mJumpEvent != nullptr)
	//				mJumpEvent();

	//		mbGrounded = false;
	//	}

	//	mGroundCross = mGroundNormal.Cross(Vector3::Up);

	//	if (Vector3::Zero != mGroundCross)
	//	{
	//		Vector3 axis = mGroundCross;

	//		Matrix mat = Matrix::CreateFromAxisAngle(axis, -mGroundSlopeAngle);
	//		mRigidbody->SetRotateDirection(mat);
	//	}
	//}
}