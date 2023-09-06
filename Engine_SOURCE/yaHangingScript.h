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
			void DoHanging(Collider2D* collider);

			virtual void Initialize() override;
			virtual void Update() override;
			virtual void Render() override;

			virtual void OnCollisionEnter(Collider2D* collider) override;
			virtual void OnCollisionStay(Collider2D* collider) override;
			virtual void OnCollisionExit(Collider2D* collider) override;

		private:
			bool mbHanging;

	};
}
