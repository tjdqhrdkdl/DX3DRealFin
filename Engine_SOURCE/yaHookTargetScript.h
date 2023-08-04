#pragma once
#include "yaScript.h"

namespace ya
{
	class HookTargetScript : public Script
	{
	public:
		HookTargetScript();
		~HookTargetScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider) override;
		virtual void OnCollisionStay(Collider2D* collider) override;
		virtual void OnCollisionExit(Collider2D* collider) override;


	private:

	};
}
