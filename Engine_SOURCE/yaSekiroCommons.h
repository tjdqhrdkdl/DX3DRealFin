#pragma once
#include "yaEngine.h"

// player parts
#define ARM L"AM"
#define BODY L"BD"
#define FACE L"FC"
#define LEG L"LG"
#define WEAPON L"WP"
#define WEAPONCASE L"WP_1"

namespace ya
{
	/*static int Random(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}*/

	static int RandomNumber(int ieast, int Max)
	{
		int result = 0;
		if (Max - ieast + 1 == 0)
			return 1;
		result = (rand() % (Max - ieast + 1)) + ieast;

		return result;
	}
}