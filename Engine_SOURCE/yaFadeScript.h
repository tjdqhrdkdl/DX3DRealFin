#pragma once
#include "yaScript.h"
#include "yaRenderer.h"

namespace ya
{
	class FadeScript : public Script
	{
	public:
		enum eState
		{
			None,
			FadeIn,
			FadeOut,
			Full,
		};

	public:
		FadeScript();
		virtual ~FadeScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetState(eState state) { mState = state; }

	private:
		eState mState;
		float mAlpha;
		renderer::ImageCB mImageCB;
	};
}