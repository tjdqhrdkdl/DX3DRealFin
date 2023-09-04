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

	public:
		void SetSpeed(const float speed) { mSpeed = speed; }
		float GetSpeed() const { return mSpeed; }

		void SetDirection(const Vector3 dir) { mDirection = dir; }
		Vector3 GetDirection() const { return mDirection; }

	public:
		void Move(const Vector3 dir, float force = -1.0f);
		void Rotate(const Vector3 dir, float speed = -1.0f);
		void Jump(float force = -1.0f);
		void Attack();
		void Deflect();
		void Parrying();

		void CheckGround();
		void UpdatePhysics();

	protected:
		GameObject* mTarget;

		class Transform* mTransform;
		class Rigidbody* mRigidbody;

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
		bool mGrounded;

	private:
		float mJumpTimer;
		float mJumpForce;
	};
}

