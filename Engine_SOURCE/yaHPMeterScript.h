#pragma once
#include "yaScript.h"
#include "yaRenderer.h"
namespace ya
{
	class HPMeterScript:public Script
	{
	public:
		HPMeterScript();
		~HPMeterScript();
		void Render() override;

		void SetCB(renderer::MeterCB cb) { mCB.HpMeter = cb.HpMeter; mCB.PostureMeter = cb.PostureMeter; }
	private:
		renderer::MeterCB mCB;
	};
}


