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

		bool IsLockOn() { return mbLockOn; }

	private:

		void TrackTarget();
		void MouseMove();
		void LockOn();
		void SetLockOnTarget();
		void ObstacleDetection();


		  


	private:
		GameObject* mPlayerTarget;
		
		
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

		//개발 도중에 사용하는 임시 변수
		GameObject* mLockOnTarget;
		GameObject* mTestTarget;



	};
}
