// registry.h - Non-throwing Windows registry access using optional.
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
        DWORD type, value, size{0};

		if (ERROR_SUCCESS != RegQueryValueEx(key, name, 0, &type, byte_ptr(&value), &size))
			return std::optional<DWORD>({});

		if (REG_DWORD != type)
			return std::optional<DWORD>({});

		return std::optional<DWORD>(value);
	}

	template<class T>
	inline LSTATUS SetValue(HKEY key, LPCTSTR name, const T& value);

	template<>
	inline LSTATUS SetValue(HKEY key, LPCTSTR name, const DWORD& value)
	{
		DWORD type = REG_DWORD, size = sizeof(DWORD);

		return RegSetValueEx(key, name, 0, type, const_byte_ptr(&value), size);
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
