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

		virtual void Initialize() override {};
		virtual void Update() override {};
		virtual void FixedUpdate() override;
		virtual void PrevRender() override;
		virtual void Render() override;
		virtual void FrameEnd() override {};

		void SetConstantBuffer();

		void SetParent(Transform* parent) { mParent = parent; }
		Transform* GetParent() { return mParent; }

		//================================== Local ====================================
		Vector3 GetLocalPosition() { return mLocalPosition; }
		Vector3 GetLocalRotation() { return mLocalRotation; }
		Quaternion GetLocalRotationQuaternion() { return mLocalRotationQuaternion; }
		Vector3 GetLocalScale() { return mLocalScale; };

		
		void SetLocalPosition(const Vector3& position) { mLocalPosition = position; };
		void SetLocalPosition(float _x, float _y, float _z) { mLocalPosition.x = _x; mLocalPosition.y = _y; mLocalPosition.z = _z; }
		void SetLocalRotation(const Vector3& degree) { mLocalRotation = degree; };
		void SetLocalRotation(float _x, float _y, float _z) { mLocalRotation.x = _x; mLocalRotation.y = _y; mLocalRotation.z = _z; };
		inline void SetLocalRotationQuaternion(const Quaternion& _rot);


		void SetLocalScale(const Vector3& scale) { mLocalScale = scale; };
		void SetLocalScale(float _x, float _y, float _z) { mLocalScale.x = _x; mLocalScale.y = _y; mLocalScale.z = _z; };
		//===============================================================================


		//================================== World ======================================
		Vector3 GetWorldRotation() const { return mWorldRotation; }
		Quaternion GetWorldRotationQuaternion() const { return mWorldRotationQuaternion; }


		void SetWorldPosition(const Vector3& _pos);
		void SetWorldRotation(const Quaternion& _rot);
		//===============================================================================


		Vector3 Forward() { return mWorldForward; }
		Vector3 Right() { return mWorldRight; }
		Vector3 Up() { return mWorldUp; }


		Vector3 GetRotationOffset() { return mLocalRotationOffset; };


		Vector3 GetWorldScale() { return mWorldScale; }



		void SetForward(const Vector3& _forward) { mWorldForward = _forward; }
		void SetRight(const Vector3& _right) { mWorldRight = _right; }
		void SetUp(const Vector3& _up) { mWorldUp = _up; }

		void SetCameraMode(bool _isCamera) { mbCameraMode = _isCamera; }

		void SetRotationOffset(const Vector3& offset) { mLocalRotationOffset = offset; };

		//void SetForward(Vector3 forward) { mForward = forward; }
		//void SetRight(Vector3 right) { mRight = right; }
		//void SetUp(Vector3 up) { mUp = up; }

		//Matrix& GetTranslationMatrix() { return mMatTranslation; }
		const Matrix& GetWorldMatrix() { return mWorldMatrix; }

		//void IsCamera(bool cam) { mbCamera = cam; }
		Vector3 GetWorldPosition() { return Vector3(mWorldMatrix._41, mWorldMatrix._42, mWorldMatrix._43); }
		

	private:
		Transform* mParent;

		Vector3 mLocalScale;
		Vector3 mLocalRotation;
		Quaternion mLocalRotationQuaternion;
		Vector3 mLocalPosition;
		Matrix mLocalMatrix;

		Vector3 mLocalRotationOffset;

		Vector3 mWorldScale;
		Vector3 mWorldRotation;
		Quaternion mWorldRotationQuaternion;
		Matrix mWorldMatrix;

		Vector3 mWorldForward;
		Vector3 mWorldRight;
		Vector3 mWorldUp;

		bool mbCameraMode;

		//Vector3 mLocalScale;
		//Vector3 mLocalPosition;
		//Vector3 mLocalRotation;
		//Vector3 mLocalRotationOffset;
		//Matrix  mLocalMatrix;

		//Vector3 mWorldRotation;
		//Vector3 mWorldScale;
		//
		//

		//bool mbCamera;

		//Vector3 mWorldRotation;
		//Quaternion mWorldQuaternion;

		//Matrix mMatScale;
		//Matrix mMatTranslation;
		//Matrix mMatRotation;
		//Matrix mMatRotationOffset;

		//Matrix mWorld;
	};


	inline void Transform::SetLocalRotationQuaternion(const Quaternion& _rot)
	{
		mLocalRotationQuaternion = _rot;
		mLocalRotation = mLocalRotationQuaternion.ToEulerXYZOrder();
	}
}
