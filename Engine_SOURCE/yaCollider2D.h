//#pragma once
//#include "yaCollider.h"
//#include "yaTransform.h"
//
//namespace ya
//{
//	class Collider3D : public Collider
//	{
//	public:
//		Collider3D();
//		~Collider3D();
//		
//		virtual void Initialize() override;
//		virtual void Update() override;
//		virtual void FixedUpdate() override;
//		virtual void Render() override;
//
//		void OnCollisionEnter(Collider3D* collider);
//		void OnCollisionStay( Collider3D* collider);
//		void OnCollisionExit( Collider3D* collider);
//
//		void OnTriggerEnter(  Collider3D* collider);
//		//void OnTriggerStay(   Collider3D* collider);
//		void OnTriggerExit(   Collider3D* collider);
//
//		void SetType(eColliderType type) { mType = type; }
//		void SetSize(Vector3 size) { mSize = size; }
//		void SetCenter(Vector3 center) { mCenter = center; }
//		void SetRotation(Vector3 rotation) { mRotation = rotation; }
//		void SetRadius(float radius) { mRadius = radius; }
//		bool IsTriiger() { return mbTrigger; }
//		UINT GetID() { return mID; }
//		Vector3 GetPosition() { return mPosition; }
//		Vector3 GetSize() { return mSize; }
//		Vector3 GetCenter() { return mCenter; }
//		eColliderType GetCollider3DType() { return mType; }
//
//		void Active(bool active) { mbActive = active; };
//		bool IsActive() { return mbActive; }
//
//	private:
//		static UINT ColliderNumber;
//		UINT mID;
//		eColliderType mType;
//		Transform* mTransform;
//
//		Vector3 mSize;
//		Vector3 mCenter;
//		Vector3 mPosition;
//		Vector3 mRotation;
//
//		float mRadius;
//		bool mbTrigger;
//		bool mbActive;
//	};
//}
