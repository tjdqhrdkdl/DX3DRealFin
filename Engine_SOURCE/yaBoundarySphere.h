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

		Vector3 GetCenter() { return mCenter; }
		float GetRadius() { return mRadius; }

	private:
		float mRadius;
		Vector3 mCenter;
		class Transform* mTransform;
	};
}
