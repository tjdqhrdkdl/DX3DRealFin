#pragma once
#include "yaComponent.h"
#include "yaCollider3D.h"

using namespace ya::math;
namespace ya
{	/// 물리 

	//class WallCheckObject;

	class Rigidbody : public Component
	{
	public:
		Rigidbody();
		virtual ~Rigidbody();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	public:
		void AddForce(const Vector3& force) { mCollider->AddForce(force); }
		void ClearForce() { mCollider->ClearForce(); }

		void SetMass(float mass) { mCollider->SetMass(mass); }

		void SetVelocity(const Vector3& velocity) { mCollider->SetVelocity(velocity); }
		inline Vector3 GetVelocity();

		void SetDynamicFriction(float friction) { mCollider->SetDynamicFriction(friction); };
		
		float GetLimitVelocity() { return mCollider->GetLimitVelocity(); }
		void SetLimitVelocity(float limit) { mCollider->SetLimitVelocity(limit); }

		//void SetRightWallDir(Vector3 dir) { mRightWallDir = dir; }

		void SetJumping(bool jumping) { mbJumping = jumping; }
		bool IsJumping() { return mbJumping; }
		//void SetGrounded(bool grounded) { mbGrounded = grounded; }
		bool IsGrounded() const;
		void SetForwardBlocked(bool forward) { mbForwardBlocked = forward; }
		bool IsForwardBlocked() { return mbForwardBlocked; }

		std::function<void()>& GetJumpEvent() { return mJumpEvent; }
		std::function<void()>& GetGroundEvent() { return mGroundEvent; }

		void AddJumpCount() { mJumpCount++; }
		UINT GetJumpCount() { return mJumpCount; }

	private:
		//Vector3 GetForce() { return mForce; }
		//void SetForce(Vector3 force) { mForce = force; }
		//void CheckGround();
		bool ForwardCheck(Vector3 movement);

	private:
		class Transform* mTransform;
		class Collider3D* mCollider;
		class ActionScript* mActionScript;
		class GroundCheckScript* mGroundCheckScript;
		//friend class WallCheckScript;

		// 힘과 마찰력을 이용한 이동
		//float mMass;		/// 무게
		//Vector3 mForce;		/// 물체는 미는 힘
		//Vector3 mVelocity;	/// 방향을 갖는 속도
		//Vector3 mAccelation;/// 가속도
		//float mFriction;	/// 마찰력

		//// 중력을 이용한 점프
		//Vector3 mGravity;	/// 중력 크기
		//Vector3 mLimitVelocity;	/// 떨어지는 속도 한계값
		//Matrix mRotateDirection; /// 경사 회전 행렬

		//Vector3 mRightWallDir;

		//Vector3 mGroundNormal;
		//Vector3 mGroundCross;
		//float mGroundDistance;
		//float mGroundSlopeAngle;
		//float mForwardSlopeAngle;

		//bool mbMoving;
		//bool mbRunning;
		bool mbJumping;
		bool mbGrounded;
		bool mbForwardBlocked;
		UINT mJumpCount;			// 이단 점프 체크

		/// <summary> 점프 상태가 시작될때 발생하는 이벤트 </summary>
		std::function<void()> mJumpEvent;

		/// <summary> 점프가 끝나고 착지한 상태가 될 때 발생하는 이벤트 </summary>
		std::function<void()> mGroundEvent;
	};


	inline Vector3 Rigidbody::GetVelocity()
	{
		return mCollider->GetVelocity();
	}


}