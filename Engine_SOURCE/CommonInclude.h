#pragma once
#include <cstdlib>
#include <cstring>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <type_traits>
#include <vector>
#include <filesystem>

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


#include <cstdint>
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using byte = std::uint8_t;
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;


//#include "yaMath.h"
