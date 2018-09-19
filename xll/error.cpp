#pragma warning(disable: 4996)
#include "error.h"
#include "registry.h"

static Reg::Key xal(HKEY_CURRENT_USER, TEXT("Software\\KALX\\xll"));
static Reg::Entry<DWORD> xll_alert_level(xal, TEXT("xll_alert_level"));
struct xal_initialize {
    xal_initialize()
    {
        if (xal.isNew()) {
            xll_alert_level = 7;
        }
    }
};
xal_initialize xal_initialize_{};

DWORD XLL_ALERT_LEVEL(DWORD level)
{
    DWORD olevel = xll_alert_level;
    
    xll_alert_level = level;

    return olevel;
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

struct test_registry {
    Reg::Key key;
	test_registry()
	{
		key = Reg::Key(HKEY_CURRENT_USER, TEXT("tmp\\key"));
        RegDeleteKey(key, TEXT("tmp\\key"));
    }
	~test_registry()
	{
	}
};
test_registry _{};

#endif // _DEBUG