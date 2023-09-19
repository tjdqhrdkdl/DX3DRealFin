#pragma once
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace ya::utils
{
	class String
	{
	public:
		inline static std::string ConvertUnicodeToUTF8(const std::wstring& _src) 
		{ 
			return WideCharToMultibyte(_src, CP_UTF8); 
		}
		inline static std::string ConvertUnicodeToANSI(const std::wstring& _src)
		{
			return WideCharToMultibyte(_src, CP_ACP);
		}
		inline static std::wstring ConvertUTF8ToUnicode(const std::string& _src)
		{
			return MultibyteToWideChar(_src, CP_UTF8);
		}
		inline static std::wstring ConvertANSIToUnicode(const std::string& _src)
		{
			return MultibyteToWideChar(_src, CP_ACP);
		}


		//새 string을 만들어서 return 해줌
		inline static std::string UpperCaseReturn(const std::string_view _str);
		//새 wstring을 만들어서 return 해줌
		inline static std::wstring UpperCaseReturn(const std::wstring_view _wstr);

		inline static std::string& UpperCase(std::string& _str);
		inline static std::wstring& UpperCase(std::wstring& _wstr);

	private:
		inline static std::string WideCharToMultibyte(const std::wstring& _src, int _codePage);
		inline static std::wstring MultibyteToWideChar(const std::string& _src, int _codePage);

	private:
		String() = delete;
		~String() = delete;
	};




	inline std::string String::UpperCaseReturn(const std::string_view _str)
	{
		std::string converted(_str);

		size_t size = converted.size();
		for (size_t i = 0; i < size; ++i)
		{
			//첫 번째 비트가 0일 경우 ASCII 코드
			if ((char)0 == (converted[i] >> 7))
			{
				converted[i] = std::toupper(converted[i]);
			}
		}

		return converted;
	}

	inline std::wstring String::UpperCaseReturn(const std::wstring_view _wstr)
	{
		std::wstring converted(_wstr);

		size_t size = converted.size();
		for (size_t i = 0; i < size; ++i)
		{
			converted[i] = (wchar_t)std::toupper((wchar_t)converted[i]);
		}

		return converted;
	}

	inline std::string& String::UpperCase(std::string& _str)
	{
		size_t size = _str.size();
		for (size_t i = 0; i < size; ++i)
		{
			//첫 번째 비트가 0일 경우 ASCII 코드
			if ((char)0 == (_str[i] >> 7))
			{
				_str[i] = std::toupper(_str[i]);
			}
		}

		return _str;
	}

	inline std::wstring& String::UpperCase(std::wstring& _wstr)
	{
		size_t size = _wstr.size();
		for (size_t i = 0; i < size; ++i)
		{
			_wstr[i] = (wchar_t)std::toupper((int)_wstr[i]);
		}

		return _wstr;
	}

	inline std::string String::WideCharToMultibyte(const std::wstring& _src, int _codePage)
	{
		std::string result;

		int srcsize = (int)_src.size();
		int len = ::WideCharToMultiByte(_codePage, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);

		result.resize(len);

		::WideCharToMultiByte(_codePage, 0, _src.data(), srcsize, result.data(), (int)result.size(), nullptr, nullptr);

		return result;
	}

	inline std::wstring String::MultibyteToWideChar(const std::string& _src, int _codePage)
	{
		std::wstring result;

		int srcsize = (int)_src.size();
		int len = ::MultiByteToWideChar(_codePage, 0, _src.data(), srcsize, nullptr, 0);

		result.resize(len);
		::MultiByteToWideChar(_codePage, 0, _src.data(), srcsize, result.data(), (int)result.size());

		return result;
	}
}

