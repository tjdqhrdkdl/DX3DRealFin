#include "yaBoneShader.h"
#include "yaConstantBuffer.h"
#include "yaRenderer.h"


namespace ya::graphics
{
	BoneShader::BoneShader()
		: mFrameDataBuffer(nullptr)
		, mOffsetMatBuffer(nullptr)
		, mOutputBuffer(nullptr)
		, mBoneAnimationData{}
	{

	}

	BoneShader::~BoneShader()
	{
	}

	void BoneShader::Binds()
	{
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Bone];
		cb->SetData(&mBoneAnimationData);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
		//mBoneAnimationData

		mFrameDataBuffer->BindSRV(eShaderStage::CS, 17);
		mOffsetMatBuffer->BindSRV(eShaderStage::CS, 18);
		mOutputBuffer->BindUAV(eShaderStage::CS, 2);

		mGroupX = (mBoneAnimationData.boneCount / mThreadGroupCountX + 1);
		mGroupY = 1;
		mGroupZ = 1;
	}

	void BoneShader::Clear()
	{
		if (mFrameDataBuffer)
		{
			mFrameDataBuffer->Clear();
			mFrameDataBuffer = nullptr;
		}
		if (mOffsetMatBuffer)
		{
			mOffsetMatBuffer->Clear();
			mOffsetMatBuffer = nullptr;
		}
		if (mOutputBuffer)
		{
			mOutputBuffer->Clear();
			mOutputBuffer = nullptr;
		}
	}

}
