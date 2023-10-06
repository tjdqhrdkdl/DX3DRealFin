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

		virtual void OnCollisionEnter(Collider2D* collider);
		virtual void OnCollisionStay(Collider2D* collider);
		virtual void OnCollisionExit(Collider2D* collider);

		void SetBlock(bool b) { mbBlock = b; }
	private:
		bool mbBlock;
	};

}
