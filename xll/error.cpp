#pragma warning(disable: 4996)
#include "error.h"
#include <Windows.h>
#include <stdexcept>
#include <string>

struct RegKey {
	HKEY hkey, h;
	std::basic_string<TCHAR> subkey;
	REGSAM sam;
	DWORD disposition;

	RegKey(HKEY hkey, LPCTSTR subkey, REGSAM sam = KEY_ALL_ACCESS)
		: hkey(hkey), h(0), subkey(subkey), sam(sam)
	{ }
	RegKey(const RegKey&) = delete;
	RegKey& operator=(const RegKey&) = delete;
	~RegKey()
	{
		Close();
	}

	void Close()
	{
		if (h)
			RegCloseKey(h);
		h = 0;
	}

	LONG Create()
	{
		return RegCreateKeyEx(hkey, subkey.c_str(), 0, 0, 0, sam, 0, &h, &disposition);
	}
	LONG Open()
	{
		return RegOpenKeyEx(hkey, subkey.c_str(), 0, sam, &h);
	}
	LONG QueryValue(LPCTSTR name, LPDWORD type, LPBYTE data, LPDWORD cbdata) 
	{
		return RegQueryValueEx(hkey, name, 0, type, data, cbdata);
	}
	LONG QueryValue(LPCTSTR name, LPDWORD value)
	{
		DWORD type;
		LONG ret = QueryValue(name, &type, 0, 0);
		if (ret != ERROR_SUCCESS)
			return ret;
		if (type != REG_DWORD)
			return ERROR_INVALID_PARAMETER;

		DWORD size;
		return QueryValue(name, &type, (LPBYTE)value, &size);
	}
	LONG SetValue(LPCTSTR name, DWORD type, LPCVOID data, DWORD cbdata)
	{
		return RegSetKeyValue(hkey, subkey.c_str(), name, type, data, cbdata);
	}
	LONG SetValue(LPCTSTR name, DWORD value)
	{
		return SetValue(name, REG_DWORD, (LPCVOID)&value, sizeof(DWORD));
	}
};

struct xll_alert {
	enum : DWORD { NOT_SET = (DWORD)-1 };
	DWORD level;
	xll_alert()
		: level(NOT_SET)
	{
		level = load();
		if (level == NOT_SET)
			level = make(XLL_ALERT_ERROR|XLL_ALERT_WARNING|XLL_ALERT_INFO);
	}
	// fail if exists
	bool make(DWORD value)
	{
		RegKey rk(HKEY_CURRENT_USER, TEXT("KALX\\xll"), KEY_SET_VALUE);
		if (ERROR_SUCCESS != rk.Create()) {
			return false;
		}
		if (rk.disposition != REG_CREATED_NEW_KEY)
			return false;
		
		return ERROR_SUCCESS == rk.SetValue(TEXT("xll_alert_level"), value);
	}
	// existing key
	DWORD load()
	{
		DWORD value{NOT_SET};

		RegKey rk(HKEY_CURRENT_USER, TEXT("KALX\\xll"), KEY_READ);
		rk.QueryValue(TEXT("xll_alert_level"), &value);

		return value;
	}
	bool save(DWORD value)
	{
		RegKey rk(HKEY_CURRENT_USER, TEXT("KALX\\xll"), KEY_SET_VALUE);
		if (ERROR_SUCCESS != rk.Open()) {
			XLL_ERROR("Open key failed: KALX\\xll", true);
			return false;
		}

		return ERROR_SUCCESS == rk.SetValue(TEXT("xll_alert_level"), value);
	}
	// return old value
	// unchanged if registry operations fail
	DWORD set(DWORD _level)
	{
		DWORD level_ = level;
		if (!make(_level))
			if (!save(_level))
				return NOT_SET;

		level = _level;

		return level_;
	}
} xll_alert_;

int 
XLL_ALERT(const char* text, const char* caption, WORD level, UINT type, bool force)
{
	if ((xll_alert_.level&level) || force) {
		if (IDCANCEL == MessageBoxA(GetForegroundWindow(), text, caption, MB_OKCANCEL|type))
			xll_alert_.set(xll_alert_.level & ~level);
	}
	// set registry value for next session
	xll_alert_.save(xll_alert_.level | level);

	return xll_alert_.level;
}

int 
XLL_ERROR(const char* e, bool force)
{
	return XLL_ALERT(e, "Error", XLL_ALERT_ERROR, MB_ICONERROR, force);
}
int 
XLL_WARNING(const char* e, bool force)
{
	return XLL_ALERT(e, "Warning", XLL_ALERT_WARNING, MB_ICONWARNING, force);
}
int 
XLL_INFO(const char* e, bool force)
{
	return XLL_ALERT(e, "Information", XLL_ALERT_INFO, MB_ICONINFORMATION, force);
}
