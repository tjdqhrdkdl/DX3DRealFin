#include "yaActionScript.h"

#include "yaTime.h"
#include "yaInput.h"
#include "yaCollisionManager.h"
#include "PhysXWrapper.h"

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
		mCollider = obj->GetComponent<Collider3D>();

		WallCheckObject* checkObj = object::Instantiate<WallCheckObject>(eLayerType::WallCheckCollision, obj->GetScene());
		assert(checkObj != nullptr);
		mCheck = checkObj;
		checkObj->SetName(L"WallCheck");
		checkObj->SetParentObj(obj);

		Transform* checkTransform = mCheck->GetComponent<Transform>();
		checkTransform->SetLocalScale(mTransform->GetLocalScale());

		Collider3D* checkCol = mCheck->AddComponent<Collider3D>();

		Collider3D* ownerCol = obj->GetComponent<Collider3D>();
		if (ownerCol != nullptr)
		{
			checkCol->SetType(ownerCol->GetCollider3DType());
			checkCol->setOffsetPosition(ownerCol->getOffsetPosition());
			checkCol->setOffsetScale(ownerCol->getOffsetScale());
		}
		else
		{
			checkCol->SetType(eColliderType::Box);
			checkCol->setOffsetPosition(Vector3(0.f, 1.2f, 0.f));
			checkCol->setOffsetPosition(Vector3(1.0, 3.0f, 1.0f));
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

		Vector3 rot = mTransform->GetLocalRotation();
		mRotateDirection = dir;
		
		if (speed > 0.0f)
		{
			rot += speed * 4.0f * mRotateDirection * Time::DeltaTime();
		}
		else
		{
			rot += mSpeed * 4.0f * mRotateDirection * Time::DeltaTime();
		}

		mTransform->SetLocalRotation(rot);
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
		Vector3 position = mTransform->GetLocalPosition();
		Vector3 scale = mTransform->GetLocalScale();
		Vector3 colScale = mCollider->getOffsetScale();
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

		//std::vector<eLayerType> layers;
		//layers.push_back(eLayerType::Monster);

		//GameObject* owner, Vector3 direction, std::vector<eLayerType> layers

		RaycastHit ForwardHit[3]{};
		CollisionManager::EnableRaycast((UINT32)GetOwner()->GetLayerType(), (UINT32)eLayerType::Monster, true);
		CollisionManager::Raycast((UINT32)eLayerType::Monster, top, rayDirection, 10000.f, &ForwardHit[0]);
		CollisionManager::Raycast((UINT32)eLayerType::Monster, middle, rayDirection, 10000.f, &ForwardHit[1]);
		CollisionManager::Raycast((UINT32)eLayerType::Monster, bottom, rayDirection, 10000.f, &ForwardHit[2]);
		//ForwardHit[0] = CollisionManager::Raycast(GetOwner(), top, rayDirection, layers);
		//ForwardHit[1] = CollisionManager::RayCast(GetOwner(), middle, rayDirection, layers);
		//ForwardHit[2] = CollisionManager::RayCast(GetOwner(), bottom, rayDirection, layers);

		for (int i = 0; i < 3; ++i)
		{
			if (ForwardHit[i].gameObject && velocity.Length() <= ForwardHit[i].hitDistance)
				return true;
		}

		return false;
	}
}