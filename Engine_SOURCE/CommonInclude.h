#pragma once
#include <cstdlib>
#include <cstring>
#include <cstring>
#include <type_traits>
#include <vector>
#include <filesystem>
#include <Windows.h>
#include <array>
#include <vector>
#include <bitset>
#include "yaMath.h"
#include "strKeys.h"

namespace std
{
	namespace fs = filesystem;
}

#ifdef _DEBUG
STRKEY gResPath = L"..\\Resources";
STRKEY gMapPath = L"Map\\Mesh";
#else
STRKEY gResPath = L"..\\..\\Resources"
STRKEY gMapPath = L"..\\..\\Resources\\Map\\Mesh";
#endif

#define arraysize(a) (sizeof(a) / sizeof(a[0]))
#define NOTIFICATION_W(_wstr) MessageBoxW(nullptr, _wstr, L"알림", MB_OK)

constexpr float gDegreeToRadFactor = 3.141592654f / 180.f;
constexpr float gRadToDegreeFactor = 180.f / 3.141592654f;



