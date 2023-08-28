#pragma once
#include "yaScript.h"

namespace ya
{
	class CrouchObjectScript : public Script
	{
	public:
		CrouchObjectScript();
		~CrouchObjectScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider) override;
		virtual void OnCollisionStay(Collider2D* collider) override;
		virtual void OnCollisionExit(Collider2D* collider) override;

		void Start();
		void Action();
		void End();

	private:

	};
}
