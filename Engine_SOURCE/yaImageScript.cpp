#include "yaImageScript.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaConstantBuffer.h"
#include "yaRenderer.h"
#include "yaMeshRenderer.h"
#include "yaMaterial.h"
#include "yaTime.h"
#include "yaInput.h"
#include "yaResources.h"

namespace ya
{
	ImageScript::ImageScript()
		: Script()
		, mColorSpeed(1.0f)
		, mAlphaSpeed(1.0f)
		, mColorType(2)
		, mbUseRate(false)
		, mbUseRange(false)
		, mImageColor(Vector4::Zero)
		, mImageRange(Vector4::Zero)
		, mImageRate(Vector2::Zero)
		, mAlphaType(eAlphaType::AlphaNone)
		, mAlpha(1.0f)
		, mColorState(eFadeState::None)
		, mAlphaState(eFadeState::None)
	{
	}

	ImageScript::~ImageScript()
	{
	}

	void ImageScript::Initialize()
	{
	}

	void ImageScript::Update()
	{
		switch (mColorState)
		{
		case ya::ImageScript::eFadeState::None:
			break;
		case ya::ImageScript::eFadeState::Fade_In:
		{
			if (mColorSpeed > 0.0f)
				mImageColor.w += mColorSpeed * Time::DeltaTime();
			else
				mImageColor.w = 1.0f;

			if (mImageColor.w >= 1.0f)
			{
				mImageColor.w = 1.0f;
				mColorState = eFadeState::Full;
			}
		}
		break;
		case ya::ImageScript::eFadeState::Fade_Out:
		{
			if (mColorSpeed > 0.0f)
				mImageColor.w -= mColorSpeed * Time::DeltaTime();
			else
				mImageColor.w = 0.0f;

			if (mImageColor.w <= 0.0f)
			{
				mImageColor.w = 0.0f;
				mColorState = eFadeState::None;
			}
		}
		break;
		case ya::ImageScript::eFadeState::Full:
			break;
		default:
			break;
		}

		switch (mAlphaState)
		{
		case ya::ImageScript::eFadeState::None:
			break;
		case ya::ImageScript::eFadeState::Fade_In:
		{
			mAlpha += mAlphaSpeed * Time::DeltaTime();
			if (mAlpha > 1.0f)
			{
				mAlpha = 1.0f;
				mAlphaState = eFadeState::Full;
			}
		}
		break;
		case ya::ImageScript::eFadeState::Fade_Out:
		{
			mAlpha -= mAlphaSpeed * Time::DeltaTime();
			if (mAlpha <= 0.0f)
			{
				mAlpha = 0.0f;
				mAlphaState = eFadeState::None;
			}
		}
		break;
		case ya::ImageScript::eFadeState::Full:
			break;
		default:
			break;
		}
	}

	void ImageScript::FixedUpdate()
	{
	}

	void ImageScript::Render()
	{
		renderer::ImageCB imageCb = {};
		imageCb.colorType = mColorType;
		//imageCb.useRange = mbUseRange;
		imageCb.imageColor = mImageColor;
		//imageCb.imageRange = mImageRange;
		imageCb.imageAlphaType = (UINT)mAlphaType;
		imageCb.imageAlpha = mAlpha;

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Image];
		cb->SetData(&imageCb);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	void ImageScript::SetColor(Vector3 color, float speed)
	{
		mColorSpeed = speed;
		mImageColor = Vector4(color.x, color.y, color.z, 0.0f);
		mColorState = eFadeState::Fade_In;
	}

	void ImageScript::SetAlpha(float alpha)
	{
		mAlpha = alpha;

		if (alpha == 1.0f)
			mAlphaState = eFadeState::Full;
		else if(alpha == 0.0f)
			mAlphaState = eFadeState::None;
	}

	void ImageScript::FadeIn(float speed)
	{
		mAlphaSpeed = speed;
		mAlphaState = eFadeState::Fade_In;
	}

	void ImageScript::FadeOut(float speed)
	{
		mAlphaSpeed = speed;
		mAlphaState = eFadeState::Fade_Out;
	}
}