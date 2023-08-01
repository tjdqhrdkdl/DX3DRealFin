#pragma once
#include "yaGameObject.h"
#include "yaScript.h"

namespace ya
{
	class GrappleHookScript : public Script
	{
	public:
		GrappleHookScript();
		virtual ~GrappleHookScript();

		virtual void Initalize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	public:
		void SetGrappleHookTarget(GameObject* target) { mGrappleHookTarget = target; }
		GameObject* GetGrappleHookTarget() const { return mGrappleHookTarget; }

		void SetSpeed(const float speed) { mSpeed = speed; }
		float GetSpeed() const { return mSpeed; }

		void SetDirection(const Vector3 dir) { mDirection = dir; }
		Vector3 GetDirection() const { return mDirection; }

		void SetGrappleHook(bool hook) { mbGrappleHook = hook; }
		bool IsGrappleHook() const { return mbGrappleHook; }

	public:
		void GrappleHook();

	private:
		GameObject* mGrappleHookTarget;
		Vector3 mHookTargetPosition;

		float mSpeed;
		Vector3 mDirection;

		float mDistance;
		float mCurrentDistance;

		bool mbGrappleHook;
	};
}

