// error.h - Error reporting functions
// Copyright (c) 2005-2011 KALX, LLC. All rights reserved.
#pragma once

#ifndef _LIB

//#pragma comment(linker, "/include:?xll_alert_level@@3V?$Object@DK@Reg@@A")

#endif // _LIB

enum { 
	XLL_ALERT_ERROR   = 1,
	XLL_ALERT_WARNING = 2, 
	XLL_ALERT_INFO    = 4,
//	XLL_ALERT_LOG     = 8	// turn on logging (should be separate!!!)
};

/// OKCANCEL message box. Cancel turns off error bit
int XLL_ERROR(const char* e, bool force = false);

/// OKCANCEL message box. Cancel turns off warning bit
int XLL_WARNING(const char* e, bool force = false);

/// OKCANCEL message box. Cancel turns off info bit
int XLL_INFO(const char* e, bool force = false);
