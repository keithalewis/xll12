// register.cpp - funcdname mangling
#include "../xll/xll.h"

using namespace xll;

Auto<Open> xao_foo2_([]{
	return Args(XLL_DOUBLE, L"?foo2", L"FOO2_").Arg(XLL_DOUBLE, L"Num").Register().isNum();
	// #VALUE! since FOO2 is the name of a cell
	//	Excelv(xlfRegister, Args(XLL_DOUBLE, L"?foo2", L"FOO2").Arg(XLL_DOUBLE, L"Num"));
});
double WINAPI foo2(double x)
{
#pragma XLLEXPORT

	return 2*x;
}

AddIn xai_foo3(
Function(XLL_DOUBLE, L"?foo3", L"FOO3_")
	.Arg(XLL_DOUBLE, L"Num")
	.Category(L"MyCategory")
	.FunctionHelp(L"Call foo3")
);
double WINAPI foo3(double x)
{
#pragma XLLEXPORT

	return 3*x;
}

Auto<Open> xao_alert([]() { return Args(L"?xll_alert", L"XLL.ALERT").Register().isNum();});
int WINAPI xll_alert()
{
#pragma XLLEXPORT

	OPER12 o;
	o = XLC(Alert, OPER12(L"Hi"));
	
	return 1;
}
//On<Doubleclick> xlodc(L"", L"XLL.CALLER");
On<Key> xlodc(ON_CTRL L"{F2}", L"XLL.CALLER");

AddIn xao_caller(Macro(L"?xll_caller", L"XLL.CALLER"));
int WINAPI xll_caller()
{
#pragma XLLEXPORT
	Excel(xlcAlert, OPER12(L"Hi"));
	/*
		Excel(xlfConcatenate, 
			OPER12(L"You double clicked from: "),
			Excel(xlfReftext, 
				  Excel(xlfCaller), 
				  OPER12(true) // A1 style
			)
		)
	); 
	*/
	return TRUE;
}
//On<Recalc> xor(L"", L"XLL.CALLER");

#include <random>
AddIn xai_rand(Function(XLL_DOUBLE, L"?xll_rand", L"XLL.RAND").Volatile());
double WINAPI xll_rand(void)
{
#pragma XLLEXPORT
	static std::default_random_engine dre;
	static std::uniform_real_distribution<double> u(0,1);

	return u(dre);
}

#if 0
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
#endif