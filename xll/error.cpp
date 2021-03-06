#pragma warning(disable: 4996)
#include <stdexcept>
#include "xll.h"
#include "registry.h"

class reg_alert_level {
	DWORD value;
public:
	reg_alert_level()
		: value(0x7)
	{
		DWORD size = sizeof(DWORD);
		LSTATUS status = RegGetValue(
			HKEY_CURRENT_USER, 
			TEXT("Software\\KALX\\xll"),
			TEXT("xll_alert_level"), 
			RRF_RT_REG_DWORD, 0,
			&value, &size);
		if (ERROR_SUCCESS != status) 
		{
			operator=(value);
		}
	}
	reg_alert_level& operator=(DWORD level)
	{
		Reg::CreateKey key(HKEY_CURRENT_USER, TEXT("Software\\KALX\\xll"));
		key[TEXT("xll_alert_level")] = level;
        value = level;

		return *this;
	}
	operator DWORD() const
	{
		return value;
	}
} xll_alert_level;

DWORD XLL_ALERT_LEVEL(DWORD level)
{
    DWORD olevel = xll_alert_level;
    
    xll_alert_level = level;

    return olevel;
}

static xll::AddIn xai_set_alert_level(
    xll::Function(XLL_LONG, L"?xll_set_alert_level", L"XLL.ALERT.LEVEL.SET")
    .Arg(XLL_LONG, L"level", L"is the alert level mask to set.")
    .FunctionHelp(L"Set the current alert level using a mask having bits for ERROR(1), WARNING(2), and INFORMATION(4).")
    .Category(L"XLL")
);
DWORD WINAPI xll_set_alert_level(DWORD w)
{
#pragma XLLEXPORT
    xll_alert_level = w;

    return xll_alert_level;
}
static xll::AddIn xai_get_alert_level(
    xll::Function(XLL_LONG, L"?xll_get_alert_level", L"XLL.ALERT.LEVEL.GET")
    .FunctionHelp(L"Get the current alert level mask having bits for ERROR(1), WARNING(2), and INFORMATION(4).")
    .Category(L"XLL")
);
DWORD WINAPI xll_get_alert_level()
{
#pragma XLLEXPORT
    return xll_alert_level;
}
int 
XLL_ALERT(const char* text, const char* caption, DWORD level, UINT type, bool force)
{
	try {
		if ((xll_alert_level&level) || force) {
			if (IDCANCEL == MessageBoxA(GetForegroundWindow(), text, caption, MB_OKCANCEL|type))
				xll_alert_level = (xll_alert_level & ~level);
		}
	}
	catch (const std::exception& ex) {
		MessageBoxA(GetForegroundWindow(), ex.what(), "Alert", MB_OKCANCEL| MB_ICONERROR);
	}

	return static_cast<int>(xll_alert_level);
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

#ifdef _DEBUG

struct test_dword {
	Reg::CreateKey key;
	test_dword()
		: key(HKEY_CURRENT_USER, TEXT("tmp\\key"))
	{
		key[TEXT("foo")] = 123;
		DWORD dw;
		dw = key[TEXT("foo")];
		if (dw != 123) {
			MessageBox(0, L"dword failed", L"Error", MB_OK);
		}
	}
	~test_dword()
	{
		RegDeleteKey(key, TEXT("tmp\\key"));
	}
};
test_dword test_dword_{};
#if 0
struct test_registry {
    Reg::Key key;
	test_registry()
	{
		key = Reg::Key(HKEY_CURRENT_USER, TEXT("tmp\\key"));
        LSTATUS status;
        status = RegDeleteKey(key, TEXT("tmp\\key"));
        using tstring = std::basic_string<TCHAR>;
        Reg::Key key2(HKEY_CURRENT_USER, TEXT("tmp\\string"));
        Reg::Entry<tstring> value(key2, TEXT("value"));
        tstring s = value;
        value = TEXT("text");
        s = value;
        s = s;
    }
	~test_registry()
	{
	}
};
test_registry _{};
#endif // 0
#endif // _DEBUG