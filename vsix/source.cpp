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
	.FunctionHelp(L"Return arguments as a 5 x 2 array.")
	//.Documentation(L"Optional detailed documentation")
);
LPOPER WINAPI xll_function(double d, const XCHAR* s, const LPOPER po, BOOL b, LONG l)
{
#pragma XLLEXPORT
	static OPER result;

	try {
		result.resize(5,3);

		result(0,0) = L"DOUBLE";
		result(0,1) = d;
		ensure (result(0,1).type() == xltypeNum);

		result(1,0) = L"CSTRING";
		result(1,1) = s;
		ensure (result(1,1).xltype == xltypeStr);

		result(2,0) = L"OPER";
		result(2,1) = Excel(xlfConcatenate, 
						    Excel(xlfRows, *po),
						    OPER(L" x "),
						    Excel(xlfColumns, *po),
							OPER(L" range"));

		result(3,0) = L"BOOL";
		result(3,1) = b ? true : false;
		ensure (result(3,1).xltype == xltypeBool);

		result(4,0) = L"LONG";
		result(4,1) = l; // Integer types are converted to floating point.
		ensure (result(4,1).xltype == xltypeNum);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		result = OPER(xlerr::NA);
	}

	return &result;
}

static Auto<Open> xao_function([](){
	try {
		OPER r(2,3);
		OPER o = *xll_function(1.23, L"foo", &r, false, 123);
		ensure (o(0,0) == L"DOUBLE"); ensure (o(0,1) == 1.23);
		ensure (o(1,0) == L"CSTRING"); ensure (o(1,1) == L"foo");
		ensure (o(2,0) == L"OPER"); ensure (o(2,1) == L"2 x 3 range");
		ensure (o(3,0) == L"BOOL"); ensure (o(3,1) == false);
		ensure (o(4,0) == L"LONG"); ensure (o(4,1) == 123);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return FALSE;
	}
	return TRUE;
});