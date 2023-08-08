#pragma once
#include "yaActionScript.h"

namespace ya
{
	class PlayerActionScript : public ActionScript
	{
		enum class eDirection
		{
			Forward,
			Back,
			Right,
			Left,
		};

	public:
		PlayerActionScript();
		virtual ~PlayerActionScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
		
	private:
		void Walk();
		void Run();

	private:
		bool mbRotate;			// 진행하려는 방향과 플레이어의 forward를 맞춰주기 위해 회전 중인지 여부
		eDirection mLastDir;	// 마지막에 입력받은 방향

		float mFrontTheta;		// 정면이라고 인식되는 최소 각도 차이

		bool mbDash;
		float mDashSpeed;
		float mDashTimer;
		float mDashTimerMax;
	};
}