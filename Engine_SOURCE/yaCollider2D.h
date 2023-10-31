#pragma once
#include "yaComponent.h"
#include "yaTransform.h"

namespace ya
{
	class Collider2D : public Component
	{
	public:
		Collider2D();
		~Collider2D();
		

		virtual void Initialize() override;
		virtual void Update() override;
		void CollisionUpdate();
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider);
		virtual void OnCollisionStay(Collider2D* collider);
		virtual void OnCollisionExit(Collider2D* collider);

		virtual void OnTriggerEnter(Collider2D* collider);
		virtual void OnTriggerStay(Collider2D* collider);
		virtual void OnTriggerExit(Collider2D* collider);

		void SetType(eColliderType type) { mType = type; }
		void SetSize(Vector3 size) { mSize = size; }
		void SetCenter(Vector3 center) { mCenter = center; }
		void SetRotation(Vector3 rotation) { mRotation = rotation; }
		void SetRadius(float radius) { mRadius = radius; }
		bool IsTriiger() { return mbTrigger; }
		UINT GetID() { return mID; }
		Vector3 GetPosition() { return mPosition; }
		Vector3 GetSize() { return mSize; }
		Vector3 GetWorldScale();
		Vector3 GetCenter() { return mCenter; }
		eColliderType GetColliderType() { return mType; }

		void Active(bool active) { mbActive = active; };
		bool IsActive() { return mbActive; }

	private:
		static UINT ColliderNumber;
		UINT mID;
		eColliderType mType;
		Transform* mTransform;

		Vector3 mSize;
		Vector3 mCenter;
		Vector3 mPosition;
		Vector3 mRotation;

		float mRadius;
		bool mbTrigger;
		bool mbActive;
	};
}
