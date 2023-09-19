#pragma once
#include "yaActionScript.h"

namespace ya
{
	class Player;
	class PlayerMeshScript;
	class PlayerProjectileScript : public Script
	{
	public:
		PlayerProjectileScript();
		virtual ~PlayerProjectileScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider);
		virtual void OnCollisionStay(Collider2D* collider);
		virtual void OnCollisionExit(Collider2D* collider);

		virtual void OnTriggerEnter(Collider2D* collider);
		virtual void OnTriggerStay(Collider2D* collider);
		virtual void OnTriggerExit(Collider2D* collider);

	private:
		Player* mPlayer;
		PlayerMeshScript* mPlayerAnim;

		bool mbBlock;
	};
}
