#pragma once
#include "yaScript.h"
#include <queue>

namespace ya
{
	class CameraScript : public Script
	{
	public:
		CameraScript();
		~CameraScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

		void SetTarget(GameObject* target) { mPlayerTarget = target; }

		GameObject* GetLockOnTarget() { return mLockOnTarget; }
		void SetLockOnTarget(GameObject* target) { mLockOnTarget = target; mbLockOn = true; }

		void SetDestination(Vector3 dest) { mbDestination = true, mDestination = dest; }
		void SetDestinationFree() { mbDestination = false; }

		bool IsLockOn() { return mbLockOn; }


	private:

		void TrackTarget();
		void MouseMove();
		void LockOn();
		void SetLockOnTarget();
		void MoveToDestination();
		void ObstacleDetection();


		  


	private:
		GameObject* mPlayerTarget;

		GameObject* mLockOnTarget;
		
		std::queue<Vector3> mQueDelayedTargetPos;
		Vector3 mDelayedTargetPos;

		Vector3 mChildPos;

		float mDelayTime;
		float mDelayTimeChecker;
		float mThetaAxisY;
		float mThetaAxisX;
		float mDistFromTarget;

		bool mbFirstInit;
		bool mbMouseMove;
		bool mbLockOn;


		//개발중 변수
		bool mbDestination;
		Vector3 mDestination;
		bool mbSelfCameraMoveMode;
	};
}
