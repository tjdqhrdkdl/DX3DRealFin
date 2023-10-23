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

		virtual void OnCollisionEnter(GameObject* _otherObj , const Vector3& _hitPoint) override;
		virtual void OnCollisionStay( GameObject* _otherObj , const Vector3& _hitPoint) override;
		virtual void OnCollisionExit( GameObject* _otherObj , const Vector3& _hitPoint) override;

	private:

		Player* mPlayer;
		PlayerMeshScript* mPlayerAnim;

		float mMoveTimer;
		float mMoveTimerMax;
		Vector3 mHitDirection;

		float TESTTime;

	};
}
