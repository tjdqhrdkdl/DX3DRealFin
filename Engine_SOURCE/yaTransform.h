#pragma once
#include "yaComponent.h"

using namespace ya::math;
namespace ya
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetConstantBuffer();
		
		void SetParent(Transform* parent) { mParent = parent; }
		Transform* GetParent() { return mParent; }

		Vector3 GetPosition() { return mPosition; };
		Vector3 GetRotation() { return mRotation; };
		Vector3 GetScale() { return mScale; };

		Vector3 GetSubPosition() { return mSubPosition; };
		Vector3 GetSubRotation() { return mSubRotation; };
		Vector3 GetSubScale()	 { return mSubScale; };

		void SetPosition(Vector3 position) { mPosition = position; };
		void SetRotation(Vector3 degree) { mRotation = degree; };
		void SetScale(Vector3 scale) { mScale = scale; };

		void SetSubPosition(Vector3 position) { mSubPosition = position; };
		void SetSubRotation(Vector3 degree) { mSubRotation = degree; };
		void SetSubScale(Vector3 scale) { mSubScale = scale; };

		Vector3 Forward() { return mForward; }
		Vector3 Right() { return mRight; }
		Vector3 Up() { return mUp; }
		void SetForward(Vector3 forward)	{ mForward = forward;	}
		void SetRight(Vector3 right)		{ mRight = right;}
		void SetUp(Vector3 up)		{ mUp = up;}
		Matrix& GetWorldMatrix() { return mWorld; }

		void IsCamera(bool cam) { mbCamera = cam; }

	private:
		Transform* mParent;
		Vector3 mForward;
		Vector3 mRight;
		Vector3 mUp;
		Vector3 mPosition;
		Vector3 mRotation;
		Vector3 mScale;

		bool mbCamera;

		Matrix mMatScale;
		Matrix mMatTranslation;
		Matrix mMatRotation;

		Matrix mWorld;

		Vector3 mSubPosition;
		Vector3 mSubRotation;
		Vector3 mSubScale;
	};
}
