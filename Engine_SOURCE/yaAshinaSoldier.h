#pragma once
#include "yaMonsterBase.h"

namespace ya
{
	class AshinaSoldier:
		public MonsterBase
	{
	public:
		AshinaSoldier();
		~AshinaSoldier();
	public:

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void DeathBlow() override;

		void OnCollisionEnter(Collider2D* collider) override;
		void OnCollisionStay(Collider2D* collider)  override;
		void OnCollisionExit(Collider2D* collider)  override;
	};
}
