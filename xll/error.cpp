#pragma warning(disable: 4996)
#include "error.h"
#include "registry.h"

static LPCTSTR XLL_ALERT_LEVEL_ = TEXT("Software\\KALX\\xll\\xll_alert_level");
static Reg::Key<DWORD> xll_alert_level(HKEY_CURRENT_USER, XLL_ALERT_LEVEL_);

DWORD XLL_ALERT_LEVEL(DWORD level)
{
	DWORD olevel = xll_alert_level.value_or(-1);

	if (level != -1)
		xll_alert_level = level;

	return olevel;
}

int 
XLL_ALERT(const char* text, const char* caption, DWORD level, UINT type, bool force)
{
	try {
		if ((xll_alert_level.value()&level) || force) {
			if (IDCANCEL == MessageBoxA(GetForegroundWindow(), text, caption, MB_OKCANCEL|type))
				xll_alert_level = (xll_alert_level.value() & ~level);
		}
	}
	catch (const std::exception& ex) {
		MessageBoxA(GetForegroundWindow(), ex.what(), "Alert", MB_OKCANCEL| MB_ICONERROR);
	}

	return static_cast<int>(xll_alert_level.value());
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
	test_registry()
	{
		Reg::Key<DWORD> key(HKEY_CURRENT_USER, TEXT("tmp\\key"));
	}
	~test_registry()
	{
	}
};
test_registry _{};

#endif // _DEBUG