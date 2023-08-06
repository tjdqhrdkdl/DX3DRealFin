#pragma once

namespace ya::enums
{
	enum class ePlayerState
	{
		None	= 0,
		Idle	= 1,
		Walk	= 2,
		Sprint	= 4,
		Jump	= 8,
		Crouch  = 16,
		Wall	= 32,
		Hang	= 64,
		Attack,
		Block,
	};

	enum class eProsthetics
	{
		None,
		Shuriken,	// 수리검
		Axe,		// 도끼
		Spear,		// 창
		End,
	};


}