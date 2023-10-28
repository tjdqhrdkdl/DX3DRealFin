#pragma once
#include "yaMath.h"
#include "yaEntity.h"

using namespace ya::enums;
namespace ya
{
	class GameObject;
	class Component : public Entity
	{
	public:
		Component(eComponentType type);
		virtual ~Component();

		virtual void Initialize() = 0;
		virtual void Start() {};
		virtual void Update() {};
		virtual void FixedUpdate() = 0;
		virtual void PrevRender() {};
		virtual void Render() = 0;
		virtual void FrameEnd() {};

		eComponentType GetOrder() { return mType; };

		GameObject* GetOwner() { return mOwner; }
		void SetOwner(GameObject* owner) { mOwner = owner; }

		void SetActive(bool _bActive) { mbActive = _bActive; }
		bool IsActive() const { return mbActive; }

	private:
		const eComponentType mType;
		GameObject* mOwner;
		bool	mbActive;
	};
}
