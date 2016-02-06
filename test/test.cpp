// test.cpp
#include "../xll/xll.h"

using namespace xll;

int xll_foo(void)
{
	xll::Register xll_foo(L"?foo", XLL_DOUBLE XLL_DOUBLE, L"FOO", L"Num");
	return 1;
}
Auto<Open> xao_foo(xll_foo);
double WINAPI foo(double x)
{
#pragma XLLEXPORT
	return 2*x;
}
