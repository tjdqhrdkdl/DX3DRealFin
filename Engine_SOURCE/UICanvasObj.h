#pragma once
#include "yaGameObject.h"

namespace ya
{
	class UICanvasObj :
		public GameObject
	{
	public:
		UICanvasObj();
		virtual ~UICanvasObj();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}


