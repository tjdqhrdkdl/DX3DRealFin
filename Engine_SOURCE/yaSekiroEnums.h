#pragma once

namespace ya::enums
{
	enum class ePlayerState
	{
		Idle		= 0,
		Walk		= 1 << 0,
		Sprint		= 1 << 1,		// 대시
		Jump		= 1 << 2,
		Crouch		= 1 << 3,		// 웅크리기
		Wall		= 1 << 4,		// 벽에 붙기(벽과 상호작용)
		Hang		= 1 << 5,		// 매달리기(벽과 상호작용)
		Hook		= 1 << 6,		// 밧줄 날리기
		Stealth		= 1 << 7,		// 은신
		Attack		= 1 << 8,		// 공격
		Block		= 1 << 9,		// 방어
		Hit			= 1 << 10,		// 피격
		DeathBlow	= 1 << 11,
		Death		= 1 << 12,
		Invincible	= 1 << 13,		// 무적상태
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