#include "StrConverter.h"
#include <winstring.h>

void StrConverter::ConvertUTF8ToUnicode(__in const std::string_view _src, __out std::wstring& _dest)
{
	_dest.clear();

	int srcsize = (int)_src.size();
	int len = ::MultiByteToWideChar(CP_UTF8, 0, _src.data(), srcsize, nullptr, 0);

	_dest.resize(len);
	::MultiByteToWideChar(CP_UTF8, 0, _src.data(), srcsize, _dest.data(), (int)_dest.size());
}

void StrConverter::ConvertUnicodeToUTF8(__in const std::wstring_view _src, __out std::string& _dest)
{
	_dest.clear();

	int srcsize = (int)_src.size();
	int len = ::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);

	_dest.resize(len);
	::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, _dest.data(), (int)_dest.size(), nullptr, nullptr);
}

std::wstring StrConverter::ConvertUTF8ToUnicode(const std::string_view _src)
{
	std::wstring result;

	int srcsize = (int)_src.size();
	int len = ::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, nullptr, 0);

	result.resize(len);
	::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, result.data(), (int)result.size());

	return result;
}

std::string StrConverter::ConvertUnicodeToUTF8(const std::wstring_view _src)
{
	std::string result;

	int srcsize = (int)_src.size();
	int len = ::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);

	result.resize(len);

	::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, result.data(), (int)result.size(), nullptr, nullptr);

	return result;
}

void StrConverter::ConvertUnicodeToANSI(__in const std::wstring_view _src, __out std::string& _dest)
{
	_dest.clear();

	int srcsize = (int)_src.size();
	int len = ::WideCharToMultiByte(CP_ACP, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);

	_dest.resize(len);
	::WideCharToMultiByte(CP_ACP, 0, _src.data(), srcsize, _dest.data(), (int)_dest.size(), nullptr, nullptr);
}

void StrConverter::ConvertANSIToUnicode(__in const std::string_view _src, __out std::wstring& _dest)
{
	_dest.clear();

	int srcsize = (int)_src.size();
	int len = ::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, nullptr, 0);

	_dest.resize(len);
	::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, _dest.data(), (int)_dest.size());
}

std::wstring StrConverter::ConvertANSIToUnicode(const std::string_view _src)
{
	std::wstring result;

	int srcsize = (int)_src.size();
	int len = ::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, nullptr, 0);

	result.resize(len);
	::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, result.data(), (int)result.size());

	return result;
}

std::string StrConverter::ConvertUnicodeToANSI(const std::wstring_view _src)
{
	std::string result;

	int srcsize = (int)_src.size();
	int len = ::WideCharToMultiByte(CP_ACP, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);

	result.resize(len);

	::WideCharToMultiByte(CP_ACP, 0, _src.data(), srcsize, result.data(), (int)result.size(), nullptr, nullptr);

	return result;
}