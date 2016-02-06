// auto.cpp - Implement well known Excel interfaces.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#include "ensure.h"
#include "auto.h"
#include "XLCALL.H"

using namespace xll;

// Called by Excel when the xll is opened.
extern "C" 
int __declspec(dllexport) WINAPI
xlAutoOpen(void)
{
	try {
		ensure (Auto<Open>::Call());
//		ensure (AddIn::RegisterAll());
		ensure (Auto<OpenAfter>::Call());

		// register OnXXX macros
		/*
		ensure ((On<Data,XLOPER>::Open()));
		ensure ((On<Doubleclick,XLOPER>::Open()));
		ensure ((On<Entry,XLOPER>::Open()));
		ensure ((On<Key,XLOPER>::Open()));
		ensure ((On<Recalc,XLOPER>::Open()));
		ensure ((On<Sheet,XLOPER>::Open()));
		ensure ((On<Time,XLOPER>::Open()));
		ensure ((On<Window,XLOPER>::Open()));
		*/
	}
	catch (const std::exception& ex) {
		MessageBoxA(GetActiveWindow(), ex.what(), "xlAutoOpen", MB_OK);

		return FALSE;
	}
	catch (...) {
		MessageBoxA(GetActiveWindow(), "Unknown exception", "xlAutoOpen", MB_OK);

		return FALSE;
	}

	return TRUE;
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