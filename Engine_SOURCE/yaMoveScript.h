#pragma once
#include "yaScript.h"

namespace ya
{
	class MoveScript : public Script
	{
		enum class eDirection
		{
			Forward,
			Back,
			Right,
			Left,
		};

	public:
		MoveScript();
		~MoveScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider) override;
		virtual void OnCollisionStay(Collider2D* collider) override;
		virtual void OnCollisionExit(Collider2D* collider) override;

	private:
		bool mbRotate;			// 진행하려는 방향과 플레이어의 forward를 맞춰주기 위해 회전 중인지 여부
		eDirection mLastDir;	// 마지막에 입력받은 방향
	};
}
