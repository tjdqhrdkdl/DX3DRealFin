#pragma once
#include "yaActionScript.h"

namespace ya
{
	class Player;
	class PlayerMeshScript;
	class PlayerAttackScript : public Script
	{
	public:
		enum class eAttackState
		{
			None,

			Attack1,			// a050_300100 // 7	
			Attack2,			// a050_305101 // 2	
			Attack3,			// a050_300020 // 3
			Attack4,			// a050_300030 // 9
			Attack5,			// a050_300040 // 5 

			JumpAttack1,		// a050_308010  a050_308060
			JumpAttack2,		// a050_308000  a050_308050
			JumpAttack3,		// a050_308010	a050_308060

			CrouchAttack1,		// a050_301050

			HangAttack1,		// a050_314000

			Block,				// a050_002000

			Move,

			End,
		};

//		- 가드


	public:
		PlayerAttackScript();
		virtual ~PlayerAttackScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	public:


	private:
		Player* mPlayer;
		PlayerMeshScript* mPlayerAnim;

		eAttackState mAttackState;
		float mTimer[(UINT)eAttackState::End];
		float mTimerMax[(UINT)eAttackState::End];
		bool mbKeyInput;

		float mblockTime;


	};
}
