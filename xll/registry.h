// registry.h - Non-throwing Windows registry access using optional.
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <windows.h>
#include <string>
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

    class Key  
	{
        HKEY key;
        DWORD disp;
	public: 
        Key()
            : key{}, disp{0}
        { }
		Key(HKEY hkey, LPCTSTR subkey, REGSAM sam = KEY_ALL_ACCESS)
		{ 
            // open existing or create new key
            RegCreateKeyEx(hkey, subkey, 0, 0, 0, sam, 0, &key, &disp);
		}
		Key(const Key&) = delete;
        Key(Key&& key_)
            : key(key_.key), disp(key_.disp)
        {
            disp = 0;
        }
        Key& operator=(Key&& key_)
        {
            key = key_.key;
            disp = key_.disp;
            key_.disp = 0;

            return *this;
        }
		Key& operator=(const Key&) = delete;
		~Key()
		{
            if (disp != 0) {
    	        RegCloseKey(key);
            }
		}

        bool isNew() const
        {
            return disp == REG_CREATED_NEW_KEY;
        }
        bool isExisting() const
        {
            return disp == REG_OPENED_EXISTING_KEY;
        }

        operator HKEY() const
        {
            return key;
        }
	};

    template<class T> 
    struct entry_traits {
        static const DWORD type;
        static const BYTE* data(const T&);
        static BYTE* data(T&);
        static DWORD size(const T&);
    };
    template<>
    struct entry_traits<DWORD> {
        static const DWORD type =  REG_DWORD;
        static const BYTE* data(const DWORD& value)
        {
            return const_byte_ptr(&value);
        }
        static BYTE* data(DWORD& value)
        {
            return byte_ptr(&value);
        }
        static DWORD size(const DWORD&)
        {
            return sizeof(DWORD);
        }
    };
    template<>
    struct entry_traits<std::basic_string<TCHAR>> {
        static const DWORD type = REG_SZ;
        static const BYTE* data(const std::basic_string<TCHAR>& value)
        {
            return const_byte_ptr(value.data());
        }
        static BYTE* data(std::basic_string<TCHAR>& value)
        {
            return byte_ptr(&value[0]);
        }
        static DWORD size(const std::basic_string<TCHAR>& value)
        {
            return static_cast<DWORD>(sizeof(TCHAR)*(value.size() + 1)) ;
        }
    };

    template<class T>
    inline LSTATUS SetValue(HKEY hkey, LPCTSTR name, const T& value)
    {
        return RegSetValueEx(hkey, name, 0, entry_traits<T>::type, entry_traits<T>::data(value), entry_traits<T>::size(value));
    }

    template<class T>
    inline LSTATUS QueryValue(HKEY hkey, LPCTSTR name, T& value)
    {
        DWORD size{0};
        DWORD type = entry_traits<T>::type;

        if constexpr (entry_traits<T>::type == REG_SZ) {
            LSTATUS status;
            status = RegQueryValueEx(hkey, name, 0, &type, 0, &size);
            if (status != ERROR_SUCCESS) {
                return status;
            }
            value.resize(static_cast<size_t>(size) + 1);
        }

        return RegQueryValueEx(hkey, name, 0, &type, entry_traits<T>::data(value), &size);
    }

    template<class T>
    class Entry {
        HKEY hkey;
        std::basic_string<TCHAR> name;
    public:
        Entry(HKEY hkey, LPCTSTR name)
            : hkey(hkey), name(name)
        { }
        operator T()
        {
            T value;

            QueryValue(hkey, name.c_str(), value);

            return value;
        }
        Entry& operator=(const T& value)
        {
            SetValue(hkey, name.c_str(), value);

            return *this;
        }
    };


}
