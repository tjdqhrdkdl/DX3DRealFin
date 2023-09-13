#pragma once
#include "yaComponent.h"


namespace ya
{
	using namespace math;
	class BoundarySphere : public Component
	{
	public:
		BoundarySphere();
		~BoundarySphere();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetCenter(Vector3 center) { mCenter = center; }
		void SetRadius(float radius) { mRadius = radius; }

	private:
		float mRadius;
		Vector3 mCenter;
		Vector3 mPositionOffSet;
		class Transform* mTransform;
	};
}
