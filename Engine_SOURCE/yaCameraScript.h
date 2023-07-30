#pragma once
#include "yaScript.h"


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

		
		Vector3 mChildPos;
	
		float mThetaAxisY;
		float mThetaAxisX;
		float mDistFromTarget;

		bool mbFirstInit;
	};
}
