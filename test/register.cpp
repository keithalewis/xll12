// register.cpp - funcdname mangling
#include "../xll/xll.h"

using namespace xll;

#include <functional>
#include <xstring>

typedef std::basic_string<XCHAR, std::char_traits<XCHAR>, std::allocator<XCHAR> > StrTy;

template<typename T> struct MapType { operator xcstr(); };
template<> struct MapType<double> { operator xcstr() { return XLL_DOUBLE; } };

StrTy excelName(xcstr name) {
	// sample name modifier
	// L"FOO2_"
	// #VALUE! since FOO2 is the name of a cell
	using namespace std;
	
	StrTy str(name);
	return str + StrTy(L"_v2");
}

template<typename R, typename... Arg>
Args autoReg(R(WINAPI *)(Arg...), xcstr name, xcstr argNames...) {
	using namespace std;
	auto appName = excelName(name);
	Args args(MapType<R>(), (StrTy(L"?") + name).c_str(), appName.c_str());
	args.Arg(MapType<Arg>()..., argNames);
	return args;
}

template<typename R>
Args autoReg(R(WINAPI *)(), xcstr name) {
	using namespace std;
	auto appName = excelName(name);
	Args args(MapType<R>(), (StrTy(L"?") + name).c_str(), appName.c_str());
	return args;
}

// 
#define ON(EVT, F, ...)						\
Auto<EVT> xao_##F##_v2_([] {				\
	return autoReg(&F, L#F, __VA_ARGS__).Register().isNum();	\
});											\
//

double WINAPI foo3(double x)
{
#pragma XLLEXPORT

	return 3 * x;
}
ON(Open, foo3, L"Num");

//
#define ON2(EVT, R, F, ARGS, ...)			\
    R WINAPI F ARGS;						\
	Auto<EVT> xao_##F##_v2_([] {			\
		return autoReg(&F, L#F, __VA_ARGS__).Register().isNum();	\
	});										\
    R WINAPI F ARGS

ON2(Open, double, foo2, (double x), L"Num") {
#pragma XLLEXPORT

	return 2 * x;
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
On<Recalc> xor(L"", L"XLL.CALLER");

#include <random>
//
#define ON3(EVT, OPT, R, F, ARGS, ...)			\
    R WINAPI F ARGS;						\
	Auto<EVT> xao_##F##_v2_([] {			\
		return autoReg(&F, L#F, __VA_ARGS__)	\
			.##OPT##.Register().isNum();	\
	});										\
    R WINAPI F ARGS

ON3(Open, 
	Volatile(),
	double, xll_rand, (void)) {

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