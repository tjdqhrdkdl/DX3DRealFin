#pragma once

namespace ya::enums
{
	enum class ePlayerState
	{
		None	= 0,
		Idle	= 1,
		Walk	= 2,
		Sprint	= 4,		// 대시
		Jump	= 8,
		Crouch  = 16,		// 웅크리기
		Wall	= 32,		// 벽에 붙기(벽과 상호작용)
		Hang	= 64,		// 매달리기(벽과 상호작용)
		Attack	= 128,		// 공격
		Block	= 256,		// 방어
		Hit		= 512,		// 피격
		FallDown = 1024,	// 쓰러진 상태
		Parrying = 2048,
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