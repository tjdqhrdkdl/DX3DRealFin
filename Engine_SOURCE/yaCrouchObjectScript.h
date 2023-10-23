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

		virtual void OnCollisionEnter(GameObject* _otherObj , const Vector3& _hitPoint) override;
		virtual void OnCollisionStay( GameObject* _otherObj , const Vector3& _hitPoint) override;
		virtual void OnCollisionExit( GameObject* _otherObj , const Vector3& _hitPoint) override;

		void Start();
		void Action();
		void End();

	private:

	};
}
