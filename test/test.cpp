// test.cpp
#include "../xll/xll.h"

using namespace xll;

int xll_test()
{
	OPER12 o1 = Arguments(L"?foo2", XLL_DOUBLE XLL_DOUBLE, L"FOO2", L"Num");
	OPER12 o2 = Args(XLL_DOUBLE, L"?foo", L"FOO").Arg(XLL_DOUBLE, L"Num");
//	ensure (o1 == o2);
//	Excelv(xlfRegister, o1);
//	Excelv(xlfRegister, o2);

	return 1;
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
const char* funcdname[100];
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
	const char* f = __FUNCDNAME__;
	funcdname[__COUNTER__] = __FUNCDNAME__;
	f = f;
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

const char* demangle;
// Register baz
Auto<Open> xao_baz([]() { Excelv(xlfRegister, Demangle(mbstowcs(demangle).c_str())); return 1; });
double WINAPI baz(BOOL b)
{
#pragma XLLEXPORT
	demangle = __FUNCDNAME__;

	return 1 - b;
}