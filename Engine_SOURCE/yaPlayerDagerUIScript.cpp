#include "yaPlayerDagerUIScript.h"

namespace ya
{
	PlayerDangerUIScript::PlayerDangerUIScript()
	{
	}

	PlayerDangerUIScript::~PlayerDangerUIScript()
	{
	}

	void PlayerDangerUIScript::Render()
	{
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Time];


		cb->SetData(&mCB);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}
}