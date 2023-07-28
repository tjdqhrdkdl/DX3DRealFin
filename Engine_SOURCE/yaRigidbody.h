#pragma once
#include "yaComponent.h"



using namespace ya::math;

namespace ya
{

	class Rigidbody : public Component
	{
	public:
		Rigidbody();

		~Rigidbody();

		virtual void Initalize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	};
}