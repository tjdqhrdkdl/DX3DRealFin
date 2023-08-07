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
		
		void TrackTarget();

		void MouseMove();
		
		void ObstacleDetection();

		void SetTarget(GameObject* target) { mPlayerTarget = target; }

		void LockOn();

		void SetLockOnTarget(GameObject* target) { mLockOnTarget = target; }
		  


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


		//개발 도중에 사용하는 임시 변수
		GameObject* mLockOnTarget;
		GameObject* mTestTarget;



	};
}
