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

		virtual void Initalize() override;
		virtual void Update() override;
		virtual void Render() override;	
		
		void TrackTarget();

		void MouseMove();
		

		void SetTarget(GameObject* target) { mTarget = target; }



	private:
		GameObject* mTarget;
		std::queue<Vector3> mQueDelayedTargetPos;
		Vector3 mDelayedTargetPos;

		Vector3 mChildPos;

		float mDelayTime;
		float mDelayTimeChecker;
		float mThetaAxisY;
		float mThetaAxisX;
		float mDistFromTarget;

		bool mbFirstInit;
	};
}
