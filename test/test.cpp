// test.cpp
#include "../xll/xll.h"

using namespace xll;

int xll_test()
{
	try {
		OPER12 o1 = Arguments(L"?foo", XLL_DOUBLE XLL_DOUBLE, L"FOO", L"Num");
		OPER12 o2 = Args(XLL_DOUBLE, L"?foo", L"FOO").Arg(XLL_DOUBLE, L"Num");
		ensure (o1 == o2);
		Excelv(xlfRegister, o1);
		o2[ARG::FunctionText].val.str[3] = L'P';
		Excelv(xlfRegister, o2);

		{
			OPER12 foo(L"foo");
			OPER12 foo2 = Excel(xlfLeft, Excel(xlfConcatenate, foo, foo), OPER12(4));
			ensure (foo2 == L"foof");
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return FALSE;
	}
	catch (...) {
		XLL_ERROR("unknown exception");
		return FALSE;
	}

	return TRUE;
}
Auto<Open> xao_test(xll_test);

double WINAPI foo2(double x)
{
#pragma XLLEXPORT
	const char* f = __FUNCDNAME__;
	f = f;
	return 2*x;
}
/*
Auto<Open> xao_foo2([]{
	Excelv(xlfRegister, Args(XLL_DOUBLE, L"?foo", L"FOO2").Arg(XLL_DOUBLE, L"Num"));
	return 1;
});
*/
Auto<Open> xao_foo([]{
	Excelv(xlfRegister, Arguments(L"?foo", XLL_DOUBLE 
		XLL_BOOL 
		XLL_DOUBLE 
		XLL_CSTRING
		XLL_PSTRING
		XLL_USHORT
		XLL_WORD
		XLL_SHORT
		XLL_LONG
		XLL_FP
		XLL_LPOPER
//		XLL_LPXLOPER
		, L"FOO", L"0,0,0,0,0,0,0,0,0,0,0")); return 1;
});
double WINAPI foo(
	BOOL xbool 
	,double num 
	,XCHAR* cstr
	,XCHAR* pstr
	,USHORT us
	,WORD word
	,SHORT s
	,LONG l
	,FP12* fp
	,LPXLOPER12 po
//	,LPXLOPER12 px
)
{
#pragma XLLEXPORT
	//const char* f = __FUNCDNAME__;
	//funcdname[__COUNTER__] = __FUNCDNAME__;
	xbool = xbool;	// "?foo@@YGNH@Z"
	num = num;		// "?foo@@YGNHN@Z"
	cstr = cstr;	// "?foo@@YGNHNPA_W@Z"
	pstr = pstr;	// "?foo@@YGNHNPA_W0@Z"
	us = us;		// "?foo@@YGNHNPA_W0G@Z"
	word = word;	// "?foo@@YGNHNPA_W0GG@Z"
	s = s;			// "?foo@@YGNHNPA_W0GGF@Z"
	l = l;			// "?foo@@YGNHNPA_W0GGFJ@Z"
	fp = fp;		// "?foo@@YGNHNPA_W0GGFJPAU_FP12@@@Z"
	po = po;		// "?foo@@YGNHNPA_W0GGFJPAU_FP12@@PAUxloper12@@@Z
//	px = px;		// "?foo@@YGNHNPA_W0GGFJPAU_FP12@@PAUxloper12@@2@Z"

	return 0;
}
/*
const XCHAR* demangle;
#define L_(x) L#x
// Register baz
//Auto<Open> xao_baz([]() { Excelv(xlfRegister, Demangle(demangle)); return 1; });
double WINAPI baz(BOOL b)
{
#pragma XLLEXPORT
	demangle = L_(__FUNCDNAME__);

	return 1 - b;
}
*/