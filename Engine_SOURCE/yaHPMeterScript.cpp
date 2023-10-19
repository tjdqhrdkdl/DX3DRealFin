#include "yaHPMeterScript.h"


namespace ya
{
	HPMeterScript::HPMeterScript()
	{
	}
	HPMeterScript::~HPMeterScript()
	{
	}
	void HPMeterScript::Render()
	{
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::HpMeter];

		cb->SetData(&mCB);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}
}