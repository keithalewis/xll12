// auto.cpp - Implement well known Excel interfaces.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#include "ensure.h"
#define NOMINMAX
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "XLCALL.H"
#include "auto.h"

using namespace xll;

template<class X>
int Auto_(const char* caption)
{
	try {
		ensure (Auto<X>::Call());
	}
	catch (const std::exception& ex) {
		MessageBoxA(GetActiveWindow(), ex.what(), caption, MB_OK);

		return FALSE;
	}
	catch (...) {
		MessageBoxA(GetActiveWindow(), "Unknown exception", caption, MB_OK);

		return FALSE;
	}

	return TRUE;
}

// Called by Excel when the xll is opened.
extern "C" 
int __declspec(dllexport) WINAPI
xlAutoOpen(void)
{
	return Auto_<Open>(__FUNCTION__);
}

extern "C"
int __declspec(dllexport) WINAPI
xlAutoClose(void)
{
	return Auto_<Close>(__FUNCTION__);
}

extern "C"
int __declspec(dllexport) WINAPI
xlAutoAdd(void)
{
	return Auto_<Add>(__FUNCTION__);
}

extern "C"
int __declspec(dllexport) WINAPI
xlAutoRemove(void)
{
	return Auto_<Remove>(__FUNCTION__);
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