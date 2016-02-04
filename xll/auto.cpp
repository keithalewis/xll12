// auto.cpp - Implement well known Excel interfaces.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
//#include "xll.h"
#include "auto.h"
#include <Windows.h>
#include "XLCALL.H"

// Called by Excel when the xll is opened.
extern "C" 
int __declspec(dllexport) WINAPI
xlAutoOpen(void)
{

	return 1;
}

extern "C"
int __declspec(dllexport) WINAPI
xlAutoClose(void)
{

	return 1;
}

extern "C"
int __declspec(dllexport) WINAPI
xlAutoAdd(void)
{

	return 1;
}

extern "C"
int __declspec(dllexport) WINAPI
xlAutoRemove(void)
{

	return 1;
}

extern "C"
void __declspec(dllexport) WINAPI
xlAutoFree12(LPXLOPER12 px)
{
	if (px->xltype & xlbitDLLFree)
		delete px;
	else if (px->xltype & xlbitXLFree)
		Excel12(xlFree, 0, 1, px);
}

extern "C"
LPXLOPER12 __declspec(dllexport) WINAPI 
xlAutoRegister12(LPXLOPER12 pxName)
{
	static XLOPER12 xResult;

	xResult = *pxName;
	return &xResult;
}


