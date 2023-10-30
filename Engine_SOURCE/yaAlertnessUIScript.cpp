#include "yaAlertnessUIScript.h"


namespace ya
{
	AlertnessUIScript::AlertnessUIScript()
	{
	}
	AlertnessUIScript::~AlertnessUIScript()
	{
	}
	void AlertnessUIScript::Render()
	{
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::UniformData];

		cb->SetData(&mCB);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}
}