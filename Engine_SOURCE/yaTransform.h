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
		virtual void Start() override;
		virtual void Update() override {};

		void CollisionUpdate();
		void FetchPhysX(const Quaternion& diffQuat, const Vector3& diffPos);

		virtual void FixedUpdate() override;
		virtual void PrevRender() override;
		virtual void Render() override;
		virtual void FrameEnd() override { mbNeedMyUpdate = false; };

		void SetConstantBuffer();

		void SetParent(Transform* parent) { assert(mParent != this); mParent = parent; NeedMyUpdate(); }
		Transform* GetParent() { return mParent; }

		//================================== Local ====================================
		Vector3 GetLocalPosition() { return mPosLocal; }
		Vector3 GetLocalRotation() { return mRotLocal; }
		Quaternion GetLocalRotationQuaternion() { return mQuatLocal; }
		Vector3 GetLocalScale() { return mScaleLocal; };

		
		void SetLocalPosition(const Vector3& position) { mPosLocal = position; NeedMyUpdate(); };
		void SetLocalPosition(float _x, float _y, float _z) { mPosLocal.x = _x; mPosLocal.y = _y; mPosLocal.z = _z; NeedMyUpdate();}


		void SetLocalRotation(const Vector3& degree) { mRotLocal = degree; NeedMyUpdate(); };
		void SetLocalRotation(float _x, float _y, float _z) { mRotLocal.x = _x; mRotLocal.y = _y; mRotLocal.z = _z; NeedMyUpdate(); };

		void SetLocalRotationRadian(const Vector3& radian) { mRotLocal = radian * gRadianToDegreeFactor; NeedMyUpdate(); }
		void SetLocalRotationRadian(float _x, float _y, float _z) { mRotLocal.x = _x; mRotLocal.y = _y; mRotLocal.z = _z; mRotLocal *= gRadianToDegreeFactor; NeedMyUpdate(); }

		inline void SetLocalRotationQuaternion(const Quaternion& _rot);


		void SetLocalScale(const Vector3& scale) { mScaleLocal = scale; };
		void SetLocalScale(float _x, float _y, float _z) { mScaleLocal.x = _x; mScaleLocal.y = _y; mScaleLocal.z = _z; };
		//===============================================================================


		//================================== World ======================================
		Vector3 GetWorldRotation() const { return mRotWorld; }
		Quaternion GetWorldRotationQuaternion() const { return mQuatWorld; }


		void SetWorldPosition(const Vector3& worldPos);
		void SetWorldRotation(const Quaternion& worldRot);
		//===============================================================================


		Vector3 Forward() { return mWorldForward; }
		Vector3 Right() { return mWorldRight; }
		Vector3 Up() { return mWorldUp; }


		Vector3 GetRotationOffset() { return mRotOffset; };

		Vector3 GetWorldScale() { return mScaleWorld; }



		void SetForward(const Vector3& _forward) { mWorldForward = _forward; }
		void SetRight(const Vector3& _right) { mWorldRight = _right; }
		void SetUp(const Vector3& _up) { mWorldUp = _up; }

		void SetCameraMode(bool _isCamera) { mbCameraMode = _isCamera; }

		void SetRotationOffset(const Vector3& offset) { mRotOffset = offset; };

		//void SetForward(Vector3 forward) { mForward = forward; }
		//void SetRight(Vector3 right) { mRight = right; }
		//void SetUp(Vector3 up) { mUp = up; }

		//Matrix& GetTranslationMatrix() { return mMatTranslation; }
		const Matrix& GetWorldMatrix() { return mMatWorld; }

		//void IsCamera(bool cam) { mbCamera = cam; }
		Vector3 GetWorldPosition() { return Vector3(mMatWorld._41, mMatWorld._42, mMatWorld._43); }

		bool IsUpdated() const { return mbNeedMyUpdate; }
		
	private:
		void NeedMyUpdate() { mbNeedMyUpdate = true; }
		void UpdateMyTransform();

	private:
		Transform* mParent;

		Vector3    mScaleLocal;
		Vector3    mRotLocal;
		Vector3	   mRotOffset;
		Quaternion mQuatLocal;
		Vector3    mPosLocal;

		//상속받지 않은 고유 트랜스폼. Scale은 적용, Size는 미적용
		//Matrix  mMatLocal;

		Vector3 mScaleWorld;
		Vector3 mRotWorld;
		Quaternion mQuatWorld;
		Matrix mMatWorld;

		//앞, 위, 오른쪽으로 나타내는 직관적인 방향 정보
		Vector3    mWorldForward;
		Vector3	   mWorldRight;
		Vector3    mWorldUp;


		//상속 형태로 업데이트가 필요한지 여부를 저장.
		//위치가 변하지 않았는데 굳이 월드행렬을 업데이트 할 필요가 없음.
		//mMatRelative를 업데이트 해야하는지 여부가 저장되어있음.
		//자신의 행렬을 업데이트 해야 한다면 반드시 부모 행렬을 받아와야 하기 때문에 m_bNeedParentUpdate도 업데이트 해준다.
		bool    mbNeedMyUpdate;




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
		mQuatLocal = _rot;
		mRotLocal = mQuatLocal.ToEulerXYZOrder() * gRadianToDegreeFactor;
		NeedMyUpdate();
	}
}
