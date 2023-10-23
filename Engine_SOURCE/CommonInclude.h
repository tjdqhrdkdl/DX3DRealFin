#pragma once
#include <cstdlib>
#include <cstring>
#include <cstring>
#include <type_traits>
#include <vector>
#include <filesystem>
#include <Windows.h>
#include "yaMath.h"

namespace std
{
	namespace fs = filesystem;
}
#define STRKEY constexpr const wchar_t*

#ifdef _DEBUG
STRKEY gResPath = L"..\\Resources";
STRKEY gMapPath = L"Map\\Mesh";
#else
STRKEY gResPath = L"..\\..\\Resources"
STRKEY gMapPath = L"..\\..\\Resources\\Map\\Mesh";
#endif

#define arraysize(a) (sizeof(a) / sizeof(a[0]))
#define NOTIFICATION_W(_wstr) MessageBoxW(nullptr, _wstr, L"알림", MB_OK)




