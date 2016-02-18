// register.cpp - funcdname mangling
#include "../xll/xll.h"

using namespace xll;

XCHAR* funcdname[64];

#define L_(x) L ## x

//#define FOO Auto<Open> xao_foo ## __COUNTER__ ([](){Excelv(xlfRegister,Arguments(L"?foo"
/*
Auto<Open> xao_foo1([](){
	Excelv(xlfRegister,Arguments(L"?xll_foo1", XLL_BOOL
	XLL_DOUBLE
BOOL WINAPI xll_foo1(
	double)
{
#pragma XLLEXPORT
	return TRUE;
}
*/