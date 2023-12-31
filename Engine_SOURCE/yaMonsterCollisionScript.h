#pragma once
#include "yaScript.h"

namespace ya
{
	class MonsterCollisionScript : public Script
	{

	public:
		MonsterCollisionScript();
		~MonsterCollisionScript();

		virtual void Initialize() override;
		//virtual void Update() override;
		//virtual void FixedUpdate() override;
		//virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider);
		virtual void OnCollisionStay(Collider2D* collider);
		virtual void OnCollisionExit(Collider2D* collider);

	private:
		class MonsterBase* mMonster;
	};

}
