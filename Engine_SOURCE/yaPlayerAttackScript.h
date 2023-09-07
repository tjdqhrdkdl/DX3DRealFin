#pragma once
#include "yaActionScript.h"

namespace ya
{
	class PlayerAttackScript : public ActionScript
	{
		enum class eAttackState
		{
			None,
			Attack1,	// 7
			Attack2,	// 2
			Attack3,	// 3
			Attack4,	// 9
			Attack5,	// 5 

			JumpAttack1,
			JumpAttack2,

			CrouchAttack1,

			HangAttack1,

			Block,

			End,
		};

		/*
		- 스탠딩 공격
		- 이동하면서 공격
		- 점프하면서 공격
		- 매달린채로 공격
		

		- 가드

		*/

	public:
		PlayerAttackScript();
		virtual ~PlayerAttackScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	public:


	private:
		eAttackState mAttackState;
		float mTimer[(UINT)eAttackState::End];
		float mTimerMax[(UINT)eAttackState::End];

		class GameObject* mAttackProjectile;

	};
}
