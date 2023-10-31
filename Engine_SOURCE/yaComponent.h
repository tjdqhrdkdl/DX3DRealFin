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
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual void PrevRender() {};
		virtual void Render() = 0;

		eComponentType GetType() { return mType; }
		GameObject* GetOwner() { return mOwner; }
		void SetOwner(GameObject* owner) { mOwner = owner; }

	private:
		eComponentType mType;
		GameObject* mOwner;
	};

	template <typename T>
	class ComponentType
	{
	public:
		explicit ComponentType(eComponentType type);
		virtual ~ComponentType() = default;

		static eComponentType GetTypeT() { return _componentType; }

	private:
		static eComponentType _componentType;
	};

	template <typename T>
	eComponentType ComponentType<T>::_componentType = eComponentType::End;

	template <typename T>
	ComponentType<T>::ComponentType(eComponentType type)
	{
		if (_componentType == eComponentType::End)
			_componentType = type;
	}
}
