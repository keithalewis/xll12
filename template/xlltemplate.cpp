// xlltemplate.cpp
#include <cmath>
#include "xlltemplate.h"

using namespace xll;

AddIn xai_template(
	Documentation(LR"(
This object will generate a Sandcastle Helpfile Builder project file.
)"));

// Information Excel needs to register add-in.
AddIn xai_function(
	// Function returning a pointer to an OPER with C name xll_function and Excel name XLL.FUNCTION.
	// Don't forget prepend a question mark to the C name.
	//                     v
    Function(XLL_LPOPER, L"?xll_function", L"XLL.FUNCTION")
	// First argument is a double called x with an argument description.
    .Arg(XLL_DOUBLE, L"x", L"is the first double argument.")
	// Paste function category.
    .Category(CATEGORY)
    .FunctionHelp(L"Help on XLL.FUNCTION goes here.")
	.Documentation(LR"(
Documentation on XLL.FUNCTION goes here.
    )")
);
// Calling convention *must* be WINAPI (aka __stdcall) for Excel.
LPOPER WINAPI xll_function(double x)
{
// Be sure to export your function.
#pragma XLLEXPORT
	static OPER result;

	try {
		ensure(x >= 0);
		result = sqrt(x); // OPER's act like Excel cells.
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		result = OPER(xlerr::Num);
	}

	return &result;
}
