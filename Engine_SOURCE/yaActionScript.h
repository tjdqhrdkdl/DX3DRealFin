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

		//virtual void OnCollisionEnter(Collider2D* collider) override;
		//virtual void OnCollisionStay(Collider2D* collider) override;
		virtual void OnCollisionExit(Collider2D* collider) override;

	public:
		void SetSpeed(const float speed) { mSpeed = speed; }
		float GetSpeed() const { return mSpeed; }

		void SetDirection(const Vector3 dir) { mDirection = dir; }
		Vector3 GetDirection() const { return mDirection; }

		void SetJumping(bool jumping) { mJumping = jumping; }
		bool IsJumping() { return mJumping; }
		void SetGrounded(bool grounded) { mGrounded = grounded; }
		bool IsGrounded() { return mGrounded; }

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

		bool mMoving;
		bool mRunning;
		bool mJumping;
		bool mGrounded;
		bool mForwardBlocked;

	private:
		float mJumpTimer;
		float mJumpForce;
	};
}

