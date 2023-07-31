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

		virtual void Initalize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetConstantBuffer();
		
		void SetParent(Transform* parent) { mParent = parent; }
		Transform* GetParent() { return mParent; }

		Vector3 GetPosition() { return mPosition; };
		Vector3 GetRotation() { return mRotation; };
		Vector3 GetScale() { return mScale; };

		void SetPosition(Vector3 position) { mPosition = position; };
		void SetRotation(Vector3 degree) { mRotation = degree; };
		void SetScale(Vector3 scale) { mScale = scale; };

		Vector3 Forward() { return mForward; }
		Vector3 Right() { return mRight; }
		Vector3 Up() { return mUp; }
		void SetForward(Vector3 forward)	{ mForward = forward;	}
		void SetRight(Vector3 right)		{ mRight = right;}
		void SetUp(Vector3 up)		{ mUp = up;}
		Matrix& GetWorldMatrix() { return mWorld; }

		void RotateFromAxis(Matrix matrix) { mbRotateFromAxis = true; mRotationFromAxis = matrix; }

		void SetQuaternion(Quaternion quat) { mbUseQuaternion = true; mRotationQuaternion = quat; }

		void IsCamera(bool cam) { mbCamera = cam; }
		Quaternion GetQuaternion() { return mRotationQuaternion; }

	private:
		Transform* mParent;
		Vector3 mForward;
		Vector3 mRight;
		Vector3 mUp;
		Vector3 mPosition;
		Vector3 mRotation;
		Vector3 mScale;
		
		bool mbRotateFromAxis;

		bool mbUseQuaternion;

		bool mbCamera;

		Quaternion mRotationQuaternion;

		Matrix mRotationFromAxis;

		Matrix mMatScale;
		Matrix mMatTranslation;
		Matrix mMatRotation;

		Matrix mWorld;
	};
}
