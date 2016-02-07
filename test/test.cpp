// test.cpp
#include "../xll/xll.h"

using namespace xll;

int xll_test()
{
	OPER12 o1 = Arguments(L"?foo", XLL_DOUBLE XLL_DOUBLE, L"FOO", L"Num");
	OPER12 o2 = Args(XLL_DOUBLE, L"?foo2", L"FOO2").Arg(XLL_DOUBLE, L"Num");
	ensure (o1 == o2);
	{
//		Excelv(xlfRegister, o1);
	}
	{
//		Excelv(xlfRegister, o2);
	}

	return 1;
}
Auto<Open> xao_test(xll_test);

double WINAPI foo(double x)
{
#pragma XLLEXPORT
	return 2*x;
}
double WINAPI foo2(double x)
{
#pragma XLLEXPORT
	return 2*x;
}
/*
Auto<Open> xao_foo2([]{
	Excelv(xlfRegister, Args(XLL_DOUBLE, L"?foo", L"FOO2").Arg(XLL_DOUBLE, L"Num"));
	return 1;
});
*/
Auto<Open> xao_foo([]{
	Excelv(xlfRegister, Arguments(L"?foo", XLL_DOUBLE XLL_DOUBLE, L"FOO", L"Num")); return 1;
});
