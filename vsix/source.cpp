#include "header.h"

using namespace xll;

AddIn xai_function(
	Function(XLL_DOUBLE, L"?xll_function", L"XLL.FUNCTION")
	.Arg(XLL_LONG, L"long", L"is a 32-bit long integer..")
	.Category(L"Category")
	.FunctionHelp(L"A sample Excel add-in function.")
	//.Documentation(L"Optional detailed documentation")
);
double WINAPI xll_function(long l)
{
#pragma XLLEXPORT
	return l;
}
