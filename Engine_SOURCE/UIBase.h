#pragma once
#include "yaGameObject.h"

namespace ya
{
	class UIBase :
		public GameObject
	{
	public:
		UIBase();
		~UIBase();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}


