#pragma warning(disable: 4996)
#include "error.h"
#include <Windows.h>

int xll_alert_level(int set = 0)
{
	static int level = 0;

	if (set != 0) {
		// write to registry
		level = set;
	}
	else if (level == 0) {
		// read from registry
		level = XLL_ALERT_ERROR|XLL_ALERT_WARNING|XLL_ALERT_INFO;
	}

	return level;
}

int 
XLL_ALERT(const char* text, const char* caption, WORD level, UINT type, bool force)
{
	if ((xll_alert_level()&level) || force) {
		if (IDCANCEL == MessageBoxA(GetForegroundWindow(), text, caption, MB_OKCANCEL|type))
			xll_alert_level(xll_alert_level() & ~level);
	}

	return xll_alert_level();
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
