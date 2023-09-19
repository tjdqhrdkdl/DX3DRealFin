#pragma once
#include "yaScript.h"

namespace ya
{
	class Player;
	class UIHPBarScript :
		public Script
	{
	public:
		UIHPBarScript();
		virtual ~UIHPBarScript();

		virtual void Initialize() override;
		virtual void Update() override;

	private:
		Player* mPlayer;
	};
}


