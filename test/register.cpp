// register.cpp - funcdname mangling
#include "../xll/xll.h"

using namespace xll;
Auto<Open> xao_foo2_([]{
	Excelv(xlfRegister, Args(XLL_DOUBLE, L"?foo2", L"FOO2_").Arg(XLL_DOUBLE, L"Num"));
	// #VALUE! since FOO2 is the name of a cell
	//	Excelv(xlfRegister, Args(XLL_DOUBLE, L"?foo2", L"FOO2").Arg(XLL_DOUBLE, L"Num"));
	return 1;
});
double WINAPI foo2(double x)
{
#pragma XLLEXPORT

	return 2*x;
}


//Auto<Open> xao_alert([]() { return Excelv(xlfRegister, Args(L"?xll_alert", L"XLL.ALERT")).type() == xltypeNum;});
int WINAPI xll_alert()
{
#pragma XLLEXPORT

	OPER12 o;
	o = XLC(Alert, OPER12(L"Hi"));
	
	return 1;
}
//On<Doubleclick> xlodc(0, L"XLL.ALERT");

XCHAR* funcdname[64];

#define L_(x) L ## x

//#define FOO Auto<Open> xao_foo ## __COUNTER__ ([](){Excelv(xlfRegister,Arguments(L"?foo"
#if 0
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
#endif