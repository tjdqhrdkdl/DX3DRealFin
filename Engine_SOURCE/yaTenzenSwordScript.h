#pragma once
#include "yaScript.h"

namespace ya
{
	class TenzenSwordScript : public Script
	{

	public:
		TenzenSwordScript();
		~TenzenSwordScript();

		virtual void Initialize() override;
		//virtual void Update() override;
		//virtual void FixedUpdate() override;
		//virtual void Render() override;

		virtual void OnCollisionEnter(Collider3D* collider);
		virtual void OnCollisionStay(Collider3D* collider);
		virtual void OnCollisionExit(Collider3D* collider);

		void SetBlock(bool b) { mbBlock = b; }
		void SetAttackLeft(bool b) { mbAttackLeft = b; }
	private:
		bool mbBlock;
		bool mbAttackLeft;
	};

}
