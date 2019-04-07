// this.cpp - Return caller's content
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include "../xll/xll.h"

using namespace xll;

static AddIn xai_this(
	Function(XLL_LPXLOPER, XLL_DECORATE(L"xll_this", 0), L"THIS")
    .Uncalced()
	.Category(L"XLL")
	.FunctionHelp(L"Return the contents of the calling cell.")
    /*
	.Documentation(
		L"The contents are the last calculated value for the cell."
	)
    */
);
extern "C" __declspec(dllexport) LPXLOPER12 WINAPI
xll_this(void)
{
	static XLOPER12 x;

	x = Excel(xlCoerce, Excel(xlfCaller));
    x.xltype |= xlbitXLFree;

	return &x;
}