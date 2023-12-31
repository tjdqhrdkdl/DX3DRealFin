#include "yaPaintShader.h"


namespace ya::graphics
{
	PaintShader::PaintShader()
		: ComputeShader()
		, mTarget(nullptr)
	{
	}

	PaintShader::~PaintShader()
	{
	}

	void PaintShader::Binds()
	{
		mTarget->BindUnorderedAccessView(0);

		mGroupX = (UINT)mTarget->GetWidth() / mThreadGroupCountX + 1u;
		mGroupY = (UINT)mTarget->GetHeight() / mThreadGroupCountY + 1u;
		mGroupZ = 1;
	}

	void PaintShader::Clear()
	{
		mTarget->ClearUnorderedAccessView(0);
	}

}
