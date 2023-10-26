#pragma once

#include "yaScript.h"

namespace ya
{
	class GameObject;
	class ActionScript : public Script
	{
	public:
		ActionScript();
		virtual ~ActionScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(GameObject* _otherObj, const Vector3& _hitPoint) override {}
		virtual void OnCollisionStay(GameObject* _otherObj, const Vector3& _hitPoint) override {}
		virtual void OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint) override {}

	public:
		void Velocity(const float velocity = 18.0f);

		void SetSpeed(const float speed = 200.0f) { mSpeed = speed; }
		float GetSpeed() const { return mSpeed; }

		void SetDirection(const Vector3 dir) { mDirection = dir; }
		Vector3 GetDirection() const { return mDirection; }

		void SetJumpTime(float time) { mJumpTime = time; }
	public:
		void Move(const Vector3 dir, float force = -1.0f);
		void Rotate(const Vector3 dir, float speed = -1.0f);
		void Jump(float force = -1.0f);
		void JumpDouble(float force = -1.0f);

		bool ForwardCheck(Vector3 movement);

	protected:
		GameObject* mTarget;
		GameObject* mCheck;

		class Transform* mTransform;
		class Rigidbody* mRigidbody;
		class Collider3D* mCollider;

		float mSpeed;
		Vector3 mDirection;
		Vector3 mRotateDirection;

		bool mbForwardBlocked;

	private:
		float mJumpTimer;
		float mJumpTime;
		float mJumpForce;
	};
}

