#pragma once
#include "yaActionScript.h"
#include "yaMonsterBase.h"

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

			AttackMove,
			HitMove,
			DeathBlow,
			
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

		virtual void OnCollisionEnter(Collider2D* collider) override;
		virtual void OnCollisionStay(Collider2D* collider) override;
		virtual void OnCollisionExit(Collider2D* collider) override;

	public:
		eAttackState GetAttackState() { return mAttackState; }
		float GetBlockTime() { return mTimer[(UINT)eAttackState::Block]; }

		void SetDeathBlowTarget(MonsterBase* monster, float distance);
		void EraseDeathBlowTarget(MonsterBase* monster);
		void ClearDeathBlowTarget() { mDeathBlowTarget = nullptr; mDeathBlowTargets.clear(); }

	private:
		void DeathBlow(MonsterBase* monster);

	private:
		Player* mPlayer;
		PlayerMeshScript* mPlayerAnim;

		// Attack
		eAttackState mAttackState;
		float mTimer[(UINT)eAttackState::End];
		float mTimerMax[(UINT)eAttackState::End];
		bool mbKeyInput;

		// Hit
		Vector3 mHitDirection;

		// DeathBlow
		MonsterBase* mDeathBlowTarget;
		std::map<MonsterBase*, float> mDeathBlowTargets;

	};
}
