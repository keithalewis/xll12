// registry.h - Non-throwing Windows registry access using optional.
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <windows.h>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace Reg {

	template<class T>
	inline BYTE* byte_ptr(T* p)
	{
		return static_cast<BYTE*>(static_cast<void*>(p));
	}
	template<class T>
	inline const BYTE* const_byte_ptr(const T* p)
	{
		return static_cast<const BYTE*>(static_cast<const void*>(p));
	}

	template<class T>
	inline std::optional<T> QueryValue(HKEY key, LPCTSTR name);

    template<>
    inline std::optional<DWORD> QueryValue(HKEY key, LPCTSTR name)
    {
        DWORD type, value, size{ 0 };

        if (ERROR_SUCCESS != RegQueryValueEx(key, name, 0, &type, byte_ptr(&value), &size))
            return std::optional<DWORD>({});

        if (REG_DWORD != type)
            return std::optional<DWORD>({});

        return std::optional<DWORD>(value);
    }

	template<class T>
    inline LSTATUS SetValue(HKEY key, LPCTSTR name, const T& value);

    // REG_BINARY - Binary data in any form.
    template<>
    inline LSTATUS SetValue(HKEY key, LPCTSTR name, const std::basic_string_view<BYTE>& value)
    {
        DWORD type = REG_BINARY;
        size_t size = value.size();

        return RegSetValueEx(key, name, 0, type, value.data(), static_cast<DWORD>(size));
    }

    // REG_DWORD - A 32 - bit number.
    template<>
	inline LSTATUS SetValue(HKEY key, LPCTSTR name, const DWORD& value)
	{
        DWORD type = REG_DWORD;
        size_t size = sizeof(DWORD);

		return RegSetValueEx(key, name, 0, type, const_byte_ptr(&value), static_cast<DWORD>(size));
	}

    // REG_SZ - A null - terminated string.
    template<>
    inline LSTATUS SetValue(HKEY key, LPCTSTR name, const std::basic_string_view<TCHAR>& value)
    {
        DWORD type = REG_SZ;
        size_t size = value.size() + 1;

        return RegSetValueEx(key, name, 0, type, const_byte_ptr(value.data()), static_cast<DWORD>(size));
    }

    template<class T>
	class Key  
	{
		HKEY key;
		std::basic_string<TCHAR> subkey;
		std::optional<T> value_;
	public:
		Key(HKEY hkey, LPCTSTR subkey, REGSAM sam = KEY_ALL_ACCESS)
			: subkey(subkey)
		{ 
			DWORD disp;

			if (ERROR_SUCCESS == RegCreateKeyEx(hkey, subkey, 0, 0, 0, sam, 0, &key, &disp)) {
				if (REG_OPENED_EXISTING_KEY == disp) {
					value_ = QueryValue<T>(key, subkey);
				}
			}
		}
		Key(const Key&) = delete;
		Key& operator=(const Key&) = delete;
		~Key()
		{
			RegCloseKey(key);
		}

		template<class U>
		constexpr T value_or(U&& u) const&
		{
			return value_.value_or(u);
		}
		constexpr const T& value() const&
		{
			return value_.value();
		}
		Key& operator=(const T& val)
		{
			if (ERROR_SUCCESS == SetValue(key, subkey.c_str(), val))
				value_ = val;

			return *this;
		}
	};

}
