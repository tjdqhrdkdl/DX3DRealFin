#pragma once
#include "yaGameObject.h"
#include "yaScript.h"

namespace ya
{
	class Player;
	class PlayerMeshScript;
	class GrappleHookScript : public Script
	{

	public:
		GrappleHookScript();
		virtual ~GrappleHookScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	public:
		void SetHookTarget(GameObject* target, float distance);
		GameObject* GetHookTarget() const { return mHookTarget; }

		void SetHookTargetDistance(float distance) { mHookTargetDistance = distance; }
		float GetHookTargetDistance() { return mHookTargetDistance; }

		void SetSpeed(const float speed) { mSpeed = speed; }
		float GetSpeed() const { return mSpeed; }

		void SetDirection(const Vector3 dir) { mDirection = dir; }
		Vector3 GetDirection() const { return mDirection; }

		void SetGrappleHook(bool hook) { mbGrappleHook = hook; }
		bool IsGrappleHook() const { return mbGrappleHook; }

		void AddHookTargetCount() { mHookTargetCount++; }
		void SubHookTargetCount();

	public:
		void GrappleHook();

	private:
		Player* mPlayer;
		PlayerMeshScript* mPlayerAnim;

		UINT mHookTargetCount;
		
		GameObject* mHookTarget;
		Vector3 mHookTargetPosition;
		float mHookTargetDistance;

		float mSpeed;
		Vector3 mDirection;

		float mDistance;
		float mCurrentDistance;

		bool mbGrappleHook;

	};
}

