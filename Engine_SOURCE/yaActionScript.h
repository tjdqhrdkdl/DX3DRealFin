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

		virtual void Initalize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	public:
		void SetSpeed(const float speed) { mSpeed = speed; }
		float GetSpeed() const { return mSpeed; }

		void SetDirection(const Vector3 dir) { mDirection = dir; }
		Vector3 GetDirection() const { return mDirection; }

	public:
		void Move(const Vector3 dir, float speed = -1.0f);
		void Rotate(const Vector3 dir, float speed = -1.0f);
		void Jump(float force = -1.0f);
		void Attack();
		void Deflect();
		void Parrying();

	private:
		GameObject* mTarget;

		class Transform* mTransform;
		class Rigidbody* mRigidbody;

		float mSpeed;
		Vector3 mDirection;

		float mJumpTimer;
		float mJumpForce;
	};
}
