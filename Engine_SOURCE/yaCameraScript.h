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

		void SetDestinationDir(Vector3 destDir) { mbDestination = true, mDestination = destDir; }
		void SetDestinationFree() { mbDestination = false; }

		//기본 줌은 현재 7
		void SetCameraZoomDistance(float destDist) { mbCameraDistChanging = true, mDestDistFromTarget = destDist; }
		void SetCameraZoomSpeed(float speed) { mZoomSpeed = speed; }

		bool IsLockOn() { return mbLockOn; }


	private:

		void TrackTarget();
		void MouseMove();
		void LockOn();
		void SetLockOnTarget();
		void MoveToDestinationDir();
		void ZoomCamera();
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
		float mDestDistFromTarget;
		float mZoomSpeed;
		bool mbCameraDistChanging;

		bool mbFirstInit;
		bool mbMouseMove;
		bool mbLockOn;


		//개발중 변수
		bool mbDestination;
		Vector3 mDestination;
		bool mbSelfCameraMoveMode;

	};
}
