// test.cpp
#include "../xll/xll.h"

using namespace xll;

AddIn xai_modulo(Function(XLL_LONG, L"?xll_modulo", L"MODULO")
	.Arg(XLL_LONG, L"x")
	.Arg(XLL_LONG, L"y")
	.Category(L"Category")
	.FunctionHelp(L"Return x%y")
);
LONG WINAPI xll_modulo(LONG x, LONG y)
{
#pragma XLLEXPORT
	return x%y;
}

