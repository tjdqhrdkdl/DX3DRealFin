#pragma once
#include "yaScript.h"

namespace ya
{
	class Player;
	class PlayerMeshScript;
	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		~PlayerScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider) override;
		virtual void OnCollisionStay(Collider2D* collider) override;
		virtual void OnCollisionExit(Collider2D* collider) override;

	private:
		Player* mPlayer;
		PlayerMeshScript* mPlayerAnim;

		float mMoveTimer;
		float mMoveTimerMax;
		Vector3 mHitDirection;

		float TESTTime;

	};
}
