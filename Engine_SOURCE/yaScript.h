#pragma once
#include "yaComponent.h"
#include "yaCollider2D.h"
#include "yaCollider3D.h"
#include "yaTransform.h"

namespace ya
{
	class GameObject;
	class Script : public Component
	{
	public:
		Script();
		virtual ~Script();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		virtual void OnCollisionEnter(GameObject* _otherObj , const Vector3& _hitPoint) {};
		virtual void OnCollisionStay(GameObject* _otherObj, const Vector3& _hitPoint) {};
		virtual void OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint) {};

		virtual void OnTriggerEnter(GameObject* _otherObj) {};
		virtual void OnTriggerExit(GameObject* _otherObj) {};

	private:
	};
}
