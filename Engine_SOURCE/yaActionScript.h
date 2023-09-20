#pragma once
#include "yaGameObject.h"
#include "yaScript.h"

namespace ya
{
	class ActionScript : public Script
	{
	public:
		ActionScript();
		virtual ~ActionScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider) override;
		virtual void OnCollisionStay(Collider2D* collider) override;
		virtual void OnCollisionExit(Collider2D* collider) override;

	public:
		void SetSpeed(const float speed) { mSpeed = speed; }
		float GetSpeed() const { return mSpeed; }

		void SetDirection(const Vector3 dir) { mDirection = dir; }
		Vector3 GetDirection() const { return mDirection; }

		void SetJumping(bool jumping) { mbJumping = jumping; }
		bool IsJumping() { return mbJumping; }
		void SetGrounded(bool grounded) { mbGrounded = grounded; }
		bool IsGrounded() { return mbGrounded; }

		std::function<void()>& GetJumpEvent() { return mJumpEvent; }
		std::function<void()>& GetGroundEvent() { return mGroundEvent; }

	public:
		void Move(const Vector3 dir, float force = -1.0f);
		void Rotate(const Vector3 dir, float speed = -1.0f);
		void Jump(float force = -1.0f);
		void Attack();
		void Deflect();
		void Parrying();

		bool ForwardCheck(Vector3 movement);
		void CheckGround();

	protected:
		GameObject* mTarget;
		GameObject* mCheck;

		class Transform* mTransform;
		class Rigidbody* mRigidbody;
		class Collider2D* mCollider;

		float mSpeed;
		Vector3 mDirection;
		Vector3 mRotateDirection;

		Vector3 mGroundNormal;
		Vector3 mGroundCross;
		float mGroundDistance;
		float mGroundSlopeAngle;
		float mForwardSlopeAngle;

		bool mbMoving;
		bool mbRunning;
		bool mbJumping;
		bool mbGrounded;
		bool mbForwardBlocked;

		/// <summary> 점프 상태가 시작될때 발생하는 이벤트 </summary>
		std::function<void()> mJumpEvent;

		/// <summary> 점프가 끝나고 착지한 상태가 될 때 발생하는 이벤트 </summary>
		std::function<void()> mGroundEvent;

	private:
		float mJumpTimer;
		float mJumpForce;
	};
}

