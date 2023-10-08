#pragma once

namespace ya::enums
{
	enum class ePlayerState
	{
		None		= 0,
		Idle		= 1 << 0,
		Walk		= 1 << 1,
		Sprint		= 1 << 2,		// 대시
		Jump		= 1 << 3,
		Crouch		= 1 << 4,		// 웅크리기
		Wall		= 1 << 5,		// 벽에 붙기(벽과 상호작용)
		Hang		= 1 << 6,		// 매달리기(벽과 상호작용)
		Attack		= 1 << 7,		// 공격
		Block		= 1 << 8,		// 방어
		Hit			= 1 << 9,		// 피격
		FallDown	= 1 << 10,		// 쓰러진 상태
		Death		= 1 << 11,
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