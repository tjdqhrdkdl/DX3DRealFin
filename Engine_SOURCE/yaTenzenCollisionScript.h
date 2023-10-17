#pragma once
#include "yaScript.h"

namespace ya
{
	class TenzenCollisionScript : public Script
	{

	public:
		TenzenCollisionScript();
		~TenzenCollisionScript();

		virtual void Initialize() override;
		//virtual void Update() override;
		//virtual void FixedUpdate() override;
		//virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider);
		virtual void OnCollisionStay(Collider2D* collider);
		virtual void OnCollisionExit(Collider2D* collider);

	private:
		class Tenzen* mTenzen;
	};

}
