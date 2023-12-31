#pragma once
#include "guiComponent.h"

namespace gui
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		ya::math::Vector3 mPosisition;
		ya::math::Vector3 mRotation;
		ya::math::Vector3 mScale;

		ya::math::Vector3 mForward;
		ya::math::Vector3 mRight;
		ya::math::Vector3 mUp;

		ya::math::Vector3 mColliderCenter;
		ya::math::Vector3 mRotationOffset;
	};

}
