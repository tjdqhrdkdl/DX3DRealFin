#pragma once
#include "yaComputeShader.h"
#include "yaRenderer.h"
#include "yaStructedBuffer.h"



namespace ya::graphics
{
	using namespace renderer;
	class BoneShader : public ComputeShader
	{
	public:
		BoneShader();
		virtual ~BoneShader();

		void SetBoneCount(int boneCount) { mBoneAnimationData.boneCount = boneCount; }
		void SetFrameIndex(int frameIdx) { mBoneAnimationData.frameIdx = frameIdx; }
		void SetNextFrameIdx(int frameIdx) { mBoneAnimationData.nextFrameIdx = frameIdx; }
		void SetFrameRatio(float frameRatio) { mBoneAnimationData.frameRatio = frameRatio; }
		void SetAnimChange(bool animChange) { mBoneAnimationData.animChange = animChange; }

		void SetFrameDataBuffer(StructedBuffer* buffer) { mFrameDataBuffer = buffer; }
		void SetNextFrameDataBuffer(StructedBuffer* buffer) { mNextFrameDataBuffer = buffer; }
		void SetOffsetMatBuffer(StructedBuffer* buffer) { mOffsetMatBuffer = buffer; }
		void SetOutputBuffer(StructedBuffer* buffer) { mOutputBuffer = buffer; }

		virtual void Binds() override;
		virtual void Clear() override;

	private:
		StructedBuffer* mFrameDataBuffer;			// t17
		StructedBuffer* mNextFrameDataBuffer;		// t18
		StructedBuffer* mOffsetMatBuffer;			// t19 
		StructedBuffer* mOutputBuffer;				// u0

		BoneAnimationCB mBoneAnimationData;
	};
}
