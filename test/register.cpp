// register.cpp - funcdname mangling
#include "../xll/xll.h"

using namespace xll;

XCHAR* funcdname[64];

#define L_(x) L ## x

//#define FOO Auto<Open> xao_foo ## __COUNTER__ ([](){Excelv(xlfRegister,Arguments(L"?foo"

template<class T>
struct xll_traits {
	static const XCHAR* type();
};
template<>
struct xll_traits<double> {
	static const XCHAR* type() { return XLL_DOUBLE; }
};

Auto<Open> xao_foo([]() { return Excelv(xlfRegister, Args(XLL_BOOL, L"?xll_foo", L"XLL.FOO")).xltype == xltypeNum; });
BOOL WINAPI xll_foo()
{
#pragma XLLEXPORT
	return TRUE;
}

Auto<Open> xao_foo1([]() { return Excelv(xlfRegister, Args(XLL_BOOL, L"?xll_foo1", L"XLL.FOO1")
	.Arg(xll_traits<double>::type(), L""))
	.xltype == xltypeNum; });
BOOL WINAPI xll_foo1(double)
{
#pragma XLLEXPORT
	return TRUE;
}


Auto<Open> xao_foo2([]() { 
	return Excelv(xlfRegister, Arguments(L"?xll_foo2", XLL_BOOL XLL_DOUBLE XLL_DOUBLE, L"XLL.FOO2"))
	.xltype == xltypeNum; });
BOOL WINAPI xll_foo2(double,double)
{
#pragma XLLEXPORT
	return TRUE;
}
