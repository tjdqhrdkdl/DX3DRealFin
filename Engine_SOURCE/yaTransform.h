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
		Vector3 GetRotationOffset() { return mRotationOffset; };
		Matrix& GetRotationMatrix() { return mMatRotation; };


		Vector3 GetFinalScale() { return mFinalScale; }


		void SetPosition(Vector3 position) { mPosition = position; };
		void SetPosition(float _x, float _y, float _z) { mPosition.x = _x; mPosition.y = _y; mPosition.z = _z; }
		void SetRotation(Vector3 degree) { mRotation = degree; };
		void SetRotation(float _x, float _y, float _z) { mRotation.x = _x; mRotation.y = _y; mRotation.z = _z; };
		void SetScale(Vector3 scale) { mScale = scale; };
		void SetScale(float _x, float _y, float _z) { mScale.x = _x; mScale.y = _y; mScale.z = _z; };
		void SetRotationOffset(Vector3 offset) { mRotationOffset = offset; };

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
		Vector3 mRotationOffset;

		Vector3 mFinalScale;

		bool mbCamera;

		Matrix mMatScale;
		Matrix mMatTranslation;
		Matrix mMatRotation;
		Matrix mMatRotationOffset;

		Matrix mWorld;
	};
}
