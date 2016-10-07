#include "header.h"

using namespace xll;

AddIn xai_function(
	Function(XLL_LPOPER, L"?xll_function", L"XLL.FUNCTION")
	.Arg(XLL_DOUBLE, L"double", L"is a 64-bit floating point number..")
	.Arg(XLL_CSTRING, L"string", L"is a string..")
	.Arg(XLL_LPOPER, L"range", L"is a range..")
	.Arg(XLL_BOOL, L"bool", L"is a boolean value..")
	.Arg(XLL_LONG, L"long", L"is a 32-bit long integer..")
	.Category(L"Category")
	.FunctionHelp(L"Return arguments as a 2x5 array..")
	//.Documentation(L"Optional detailed documentation")
);
LPOPER WINAPI xll_function(double d, const XCHAR* s, const LPOPER po, BOOL b, LONG l)
{
#pragma XLLEXPORT
	static OPER result(5, 2);

	result(0,0) = L"DOUBLE";
	result(0,1) = d;

	result(1,0) = L"CSTRING";
	result(1,1) = s;

	result(2,0) = L"OPER";
	result(2,1) = Excel(xlfConcatenate, 
					Excel(xlfRows, *po),
					OPER(L" x "),
					Excel(xlfColumns, *po));

	result(3,0) = L"BOOL";
	result(3,1) = b ? true : false;

	result(4,0) = L"LONG";
	result(4,1) = l; // really xltypeNum

	return &result;
}
