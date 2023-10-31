#include "yaRigidbody.h"
#include "yaTime.h"
#include "yaGameObject.h"
#include "yaCollider2D.h"
#include "yaActionScript.h"

#include "yaCollisionManager.h"

#include "yaInput.h"

namespace ya
{
	Rigidbody::Rigidbody()
		:Component(eComponentType::Rigidbody)
		, mTransform(nullptr)
		, mActionScript(nullptr)
		, mMass(0.6f)
		, mForce(Vector3::Zero)
		, mVelocity(Vector3::Zero)
		, mAccelation(Vector3::Zero)
		, mFriction(180.0f)

		, mRightWallDir(Vector3::Zero)
		, mGroundDistance(0.f)
		, mGroundNormal(Vector3::Zero)
		, mGroundSlopeAngle(0.f)
		, mForwardSlopeAngle(0.f)
		, mGroundCross(0.f)

		//, mbMoving(false)
		//, mbRunning(false)
		, mbJumping(false)
		, mbGrounded(false)
		, mbForwardBlocked(false)

		, mJumpEvent(nullptr)
		, mGroundEvent(nullptr)
		, mJumpCount(0)
	{
		mGravity = Vector3(0.0f, -140.0f, 0.0f);
		mLimitVelocity = Vector3(10.0f, 30.0f, 10.0f);
	}

	Rigidbody::~Rigidbody()
	{

	}

	void Rigidbody::Initialize()
	{
		GameObject* obj = GetOwner();
		assert(obj != nullptr);

		mTransform = obj->GetComponent<Transform>();
		mCollider = obj->GetComponent<Collider2D>();
		mActionScript = obj->GetScript<ActionScript>();
	}

	void Rigidbody::FixedUpdate()
	{
		
	}

	void Rigidbody::Update()
	{
		CheckGround();

		// F = M X A
		// A = F / M

		if (mMass == 0.0f)
			return;

		Transform* tr = mTransform;

		mAccelation = mForce / mMass;

		// 속도에 가속도를 더해준다.
		mVelocity += (mAccelation * Time::DeltaTime());

		Vector3 gravity = mGravity;
		gravity.Normalize();
		float dot = gravity.Dot(mVelocity);
		Vector3 sideVelocity;
		if (mbGrounded && !mbJumping)
		{ // 땅

			mVelocity -= gravity * dot;
			sideVelocity = mVelocity;
		}
		else
		{ // 공중
			mVelocity += mGravity * Time::DeltaTime();
			gravity = gravity * dot;
			sideVelocity = mVelocity - gravity;
		}

		// 최대 속도 제한
		if (mLimitVelocity.y < gravity.Length())
		{
			gravity.Normalize();
			gravity *= mLimitVelocity.y;
		}

		if (mLimitVelocity.x < sideVelocity.Length())
		{
			sideVelocity.Normalize();
			sideVelocity *= mLimitVelocity.x;
		}

		mVelocity = gravity + sideVelocity;

		// 마찰력 조건 : 적용된 힘이 없고, 속도가 0이 아닐때
		if (!(mVelocity == Vector3::Zero))
		{
			//속도에 반대 방향
			Vector3 friction = -mVelocity;
			friction.Normalize();
			friction = friction * mFriction * mMass * Time::DeltaTime();

			//마찰력으로 인한 속도 감소량이 현재 속도보다 더 큰 경우
			if (mVelocity.Length() < friction.Length())
				//속도를 0 로 만든다.
				mVelocity = Vector3::Zero;
			else
				//속도에서 마찰력으로 인한 반대방향으로 속도를 차감한다.
				mVelocity += friction;
		}

		// 속도에 맞춰 물체를 이동시킨다.
		Vector3 velo;

		if (mbGrounded)
		{
			Vector3 dir = mVelocity;
			float length = dir.Length();

			dir.Normalize();
			dir = Vector3::Transform(dir, mRotateDirection);

			// 회전된 속도는 땅에서만 사용하므로 멤버 변수를 건드리지 않는다.
			velo = dir * length;
			mRotateDirection = {};
		}
		else
			velo = mVelocity;

		if (mbForwardBlocked && !mbJumping && mbGrounded)
		{
			float projLength = velo.Dot(mRightWallDir);

			velo = mRightWallDir * projLength;

			Vector3 objPos = mTransform->GetPosition();
			objPos -= mVelocity * Time::DeltaTime();

			mTransform->SetPosition(objPos);
		}

		if (mbForwardBlocked && mbJumping)
		{
 			velo.x = 0.f;
			velo.z = 0.f;
		}

		if (ForwardCheck(velo))
		{
			velo = Vector3::Zero;

			Vector3 objPos = mTransform->GetPosition();
			objPos -= mVelocity * Time::DeltaTime();

			mTransform->SetPosition(objPos);
		}

		Vector3 pos = tr->GetPosition();
		pos += velo * Time::DeltaTime();
		tr->SetPosition(pos);

		ClearForce();
	}

	void Rigidbody::Render()
	{
	}

	void Rigidbody::AddForce(Vector3 force)
	{
		mForce += force;
	}

	void Rigidbody::ClearForce()
	{
		mForce = Vector3::Zero;
	}

	void Rigidbody::CheckGround()
	{
		Vector3 position = mTransform->GetPosition();
		Vector3 objPos = mCollider->GetCenter();
		Vector3 Scale = mTransform->GetScale();
		Vector3 objScale = mCollider->GetSize();

		position += objPos;
		objScale *= Scale;

		// 지형체크용 레이의 시작점은 포지션의 맨아래에서 시작
		Vector3 rayPosition = position;
		rayPosition.y -= objScale.y / 2.f;

		// z의 크기 절반 만큼의 크기를 가진 forward 벡터와 위에서 설정한 위치 벡터를 더하여
		// forward 방향을 가진 z크기의 절반 값을 가진 벡터를 구한다
		Vector3 forward = mTransform->Forward();
		forward = forward * objScale.z / 2.f;
		Vector3 forwardPos = rayPosition + forward;

		// 순서대로 북서, 북동, 남서, 남동
		//Vector3 nw = forwardPos;
		//nw.x -= objScale.x / 2.f;

		//Vector3 ne = forwardPos;
		//ne.x += objScale.x / 2.f;

		//Vector3 sw = forwardPos;
		//sw.x -= objScale.x / 2.f;
		//sw.z -= objScale.z;

		//Vector3 se = forwardPos;
		//se.x += objScale.x / 2.f;
		//se.z -= objScale.z;

		std::vector<eLayerType> layers = {};
		layers.push_back(eLayerType::Ground);

		Vector3 direction = -(mTransform->Up());

		// 지형 체크용
		//RayHit CheckHit[4] = {};
		//CheckHit[0] = CollisionManager::RayCast(GetOwner(), nw, direction, layers);
		//CheckHit[1] = CollisionManager::RayCast(GetOwner(), ne, direction, layers);
		//CheckHit[2] = CollisionManager::RayCast(GetOwner(), sw, direction, layers);
		//CheckHit[3] = CollisionManager::RayCast(GetOwner(), se, direction, layers);

		// 지형 보정용
		RayHit CorrectionHit = CollisionManager::RayCast(GetOwner(), position, direction, layers);

		bool bGroundFlag = false;

		if (CorrectionHit.isHit)
		{
			// 땅에 붙었는지 체크하기 위한 플래그
			Transform* hitTransform = CorrectionHit.hitObj->GetComponent<Transform>();

			mGroundNormal = hitTransform->Up();

			Vector3 direction = mVelocity;
			direction.Normalize();

			float groundRadian = mGroundNormal.Dot(Vector3::Up);
			groundRadian = acos(groundRadian);

			float forwardRadian = mGroundNormal.Dot(direction);
			forwardRadian = acos(forwardRadian);

			mGroundSlopeAngle = groundRadian;
			mForwardSlopeAngle = forwardRadian - 1.5708f;

			// 밑바닥을 기준으로 구한 거리
			float CorrectionLength = CorrectionHit.length - objScale.y / 2.f;

			if (CorrectionLength < 0.001f)
			{ // 바닥으로 부터 거리가 0.001f 미만일때
				if (!mbGrounded)
				{	// 착지하는지 체크
					mbJumping = false;

					if (mGroundEvent != nullptr)
					{
						mGroundEvent();
						mJumpCount = 0;
					}
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
					mTransform->SetPosition(correctionPos);
				}
			}
		}

		if (!bGroundFlag)
		{
			if (mbGrounded)
				if (mJumpEvent != nullptr)
					mJumpEvent();

			mbGrounded = false;
		}

		mGroundCross = mGroundNormal.Cross(Vector3::Up);

		if (Vector3::Zero != mGroundCross)
		{
			Vector3 axis = mGroundCross;

			mRotateDirection = Matrix::CreateFromAxisAngle(axis, -mGroundSlopeAngle);
		}
	}
	bool Rigidbody::ForwardCheck(Vector3 movement)
	{
		Vector3 position = mTransform->GetPosition();
		Vector3 scale = mTransform->GetScale();
		Vector3 colScale = mCollider->GetSize();
		Vector3 velocity = movement * Time::DeltaTime();
		Vector3 dir = movement;
		dir.Normalize();

		float rayLength = scale.z / 2.f;

		float velocityLength = velocity.Length();
		float positionLength = position.Length();

		scale *= colScale;

		Vector3 top = position;
		top.y += scale.y / 2.f;

		Vector3 middle = position;

		Vector3 bottom = position;
		bottom.y -= scale.y / 2.f;

		Vector3 rayDirection = dir;

		std::vector<eLayerType> layers;
		layers.push_back(eLayerType::Player);
		layers.push_back(eLayerType::Monster);

		RayHit ForwardHit[3];
		ForwardHit[0] = CollisionManager::RayCast(GetOwner(), top, rayDirection, layers);
		ForwardHit[1] = CollisionManager::RayCast(GetOwner(), middle, rayDirection, layers);
		ForwardHit[2] = CollisionManager::RayCast(GetOwner(), bottom, rayDirection, layers);

		for (int i = 0; i < 3; ++i)
		{
			//if (velocity.Length() <= ForwardHit[i].length && ForwardHit[i].isHit)
			if(rayLength >= ForwardHit[i].length && ForwardHit[i].isHit)
				return true;
		}

		return false;
	}
}