#pragma once
#include "yaScript.h"
#include "yaRenderer.h"

namespace ya
{
	class AlertnessUIScript :public Script
	{
	public:
		AlertnessUIScript();
		~AlertnessUIScript();
		void Render() override;

		void SetCB(renderer::UniformDataCB cb) { mCB.float_0 = cb.float_0; mCB.int_0 = cb.int_0; mCB.int_1 = cb.int_1;}
	private:
		renderer::UniformDataCB mCB;
	};
}


