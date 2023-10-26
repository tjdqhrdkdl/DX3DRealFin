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

		virtual void OnCollisionEnter(Collider3D* collider);
		virtual void OnCollisionStay(Collider3D* collider);
		virtual void OnCollisionExit(Collider3D* collider);

		virtual void OnTriggerEnter(Collider3D* collider);
		virtual void OnTriggerStay(Collider3D* collider);
		virtual void OnTriggerExit(Collider3D* collider);

	public:
		void SetPlayer(Player* player);

	private:
		Player* mPlayer;
		PlayerMeshScript* mPlayerAnim;
	};
}
