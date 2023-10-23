#pragma once
#include "yaComponent.h"

namespace ya
{
	using namespace math;
	class Collider : public Component
	{
	public:
		Collider(eComponentType _type) : Component(_type) {};
		virtual ~Collider() {};
	};
}


