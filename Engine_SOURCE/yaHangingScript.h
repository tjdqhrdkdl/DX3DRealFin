#pragma once

#include "yaScript.h"

namespace ya
{
	class HangingScript : public Script
	{
		public:
			HangingScript();
			~HangingScript();

			void InitHanging();
			void DoHanging(GameObject* _otherObj, const Vector3& _hitPoint);

			virtual void Initialize() override;
			virtual void Update() override;
			virtual void Render() override;

			virtual void OnCollisionEnter(GameObject* _otherObj , const Vector3& _hitPoint) override;
			virtual void OnCollisionStay( GameObject* _otherObj , const Vector3& _hitPoint) override;
			virtual void OnCollisionExit( GameObject* _otherObj , const Vector3& _hitPoint) override;

		private:
			bool mbHanging;

	};
}
