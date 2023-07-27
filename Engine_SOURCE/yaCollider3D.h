//#pragma once
//#include "yaComponent.h"
//#include "yaTransform.h"
//
//namespace ya
//{
//	class Collider3D : public Component
//	{
//	public:
//		Collider3D();
//		~Collider3D();
//
//		virtual void Initalize() override;
//		virtual void Update() override;
//		virtual void FixedUpdate() override;
//		virtual void Render() override;
//
//		virtual void OnCollisionEnter(Collider3D* collider);
//		virtual void OnCollisionStay(Collider3D* collider);
//		virtual void OnCollisionExit(Collider3D* collider);
//
//		virtual void OnTriggerEnter(Collider3D* collider);
//		virtual void OnTriggerStay(Collider3D* collider);
//		virtual void OnTriggerExit(Collider3D* collider);
//
//		void SetType(eColliderType type) { mType = type; }
//		void SetSize(Vector2 size) { mSize = size; }
//		void SetCenter(Vector2 center) { mCenter = center; }
//		void SetRadius(float radius) { mRadius = radius; }
//		bool IsTriiger() { return mbTrigger; }
//		UINT GetID() { return mID; }
//		Vector3 GetPosition() { return mPosition; }
//		Vector2 GetSize() { return mSize; }
//
//	private:
//		static UINT ColliderNumber;
//		UINT mID;
//		eColliderType mType;
//		Transform* mTransform;
//
//		Vector2 mSize;
//		Vector2 mCenter;
//		Vector3 mPosition;
//		float mRadius;
//		bool mbTrigger;
//	};
//}
