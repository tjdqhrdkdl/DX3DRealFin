#pragma once
#include "yaScript.h"

namespace ya
{
	class ImageScript : public Script
	{
	public:
		enum eFadeState
		{
			None,
			Fade_In,
			Fade_Out,
			Full,
		};

		enum eAlphaType
		{
			AlphaNone,
			Opacity,
			Transparent,
		};

	public:
		ImageScript();
		virtual ~ImageScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	public:
		void SetColorSpeed(float speed) { mColorSpeed = speed; }
		void SetAlphaSpeed(float speed) { mAlphaSpeed = speed; }
		
		void SetRange(Vector4 range) { mbUseRange = true; mImageRange = range; }
		void SetRate(Vector2 rate) { mbUseRate = true; mImageRate = rate; }

		void SetColor(Vector3 color, float speed = 0.0f);
		void SetAlpha(float alpha);
		void SetAlphaType(eAlphaType type) { mAlphaType = type; }

		void FadeIn(float speed = 1.0f);
		void FadeOut(float speed = 1.0f);

	protected:
		float	mColorSpeed;
		float	mAlphaSpeed;
		
		UINT	mColorType;
		bool	mbUseRate;
		bool	mbUseRange;
		Vector4 mImageColor;
		Vector4 mImageRange;
		Vector2 mImageRate;
		float	mAlpha;
		eAlphaType	mAlphaType;

		eFadeState	mColorState;
		eFadeState	mAlphaState;
	};
}
