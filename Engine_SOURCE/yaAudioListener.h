#pragma once
#include "yaComponent.h"


namespace ya
{
	class AudioListener : public Component, ComponentType<AudioListener>
	{
	public:
		AudioListener();
		~AudioListener();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}