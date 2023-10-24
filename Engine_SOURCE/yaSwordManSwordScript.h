#pragma once
#include "yaScript.h"

namespace ya
{
	class SwordManSwordScript : public Script
	{

	public:
		SwordManSwordScript();
		~SwordManSwordScript();

		virtual void Initialize() override;
		//virtual void Update() override;
		//virtual void FixedUpdate() override;
		//virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider);
		virtual void OnCollisionStay(Collider2D* collider);
		virtual void OnCollisionExit(Collider2D* collider);

		void SetBlock(bool b) { mbBlock = b; }
		void SetAttackLeft(bool b) { mbAttackLeft = b; }
	private:
		bool mbBlock;
		bool mbAttackLeft;
	};

}
