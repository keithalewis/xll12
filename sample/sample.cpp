// sample.cpp - Simple example of using AddIn.
#include <cmath>
#include "../xll/xll.h"
#include "../xll/shfb/shfb.h"
#include "../xll/shfb/entities.h"

using namespace xll;
using namespace shfb;

AddIn xai_sample(
	Documentation(LR"(
This object will generate a Sandcastle Helpfile Builder project file.
)"));

AddIn xal_sample_category(
    Documentation(LR"(
This object will generate documentation for the Example category.
)")
    .Category(L"Example")
);
// Information Excel needs to register add-in.
AddIn xai_function(
	// Function returning a pointer to an OPER with C++ name xll_function and Excel name XLL.FUNCTION.
	// Don't forget prepend a question mark to the C++ name.
	//                     |
	//                     v
	Function(XLL_LPOPER, L"?xll_function", L"XLL.FUNCTION")
	// First argument is a double called x with an argument description and default value of 2
	.Arg(XLL_DOUBLE, L"x", L"is the first double argument.", L"2")
	// Paste function category.
	.Category(L"Example")
	// Insert Function description.
	.FunctionHelp(L"Help on XLL.FUNCTION goes here.")
	// Create entry for this function in Sandcastle Help File Builder project file.
	.Alias(L"XLL.FUNCTION.ALIAS") // alternate name
    .Documentation(maml()
        .para(
            maml()._(L"Free-form documentation on ").c(L"XLL.FUNCTION")._(L" goes here.")
        )
        .para(L"But you can include MAML directives.")
        .para(maml()
            ._(L"This is ").bold(L"bold")._(L" and so is <legacyBold>this</legacyBold>")
        )
        .para(maml()
            ._(L"Math: ").math(int_).sub(minus_ infin_).sup(infin_)._(L"e").sup(minus_ L"x" sup2_ L"/2")._(L" dx")
        )
    )
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
