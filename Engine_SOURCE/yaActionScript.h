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
		void Velocity(const float velocity = 12.0f);

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
		class Collider2D* mCollider;

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

