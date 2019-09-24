// sample.cpp - Simple example of using AddIn.
#include <cmath>
#include "../xll/xll.h"
#include "../xll/shfb/entities.h"

using namespace xll;

AddIn xai_sample(
    Document(L"") // top level documentation
	.Documentation(LR"(
This object will generate a Sandcastle Helpfile Builder project file.
)"));

AddIn xal_sample_category(
    Document(L"The Example Category.")
    .Category(L"Example")
    .Documentation(LR"(
This object will generate documentation for the Example category.
)")
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
    .Documentation(
        PARA_(
            L"Free-form documentation for " C_(L"XLL.FUNCTION") L" goes here."
        )
        PARA_(L"But you can include MAML directives.")
        PARA_(L"This is " B_(L"bold") " and so is " B_("this"))
        PARA_(L"Math: " MATH_(int_ SUB_(minus_ infin_) SUP_(infin_) L"e" SUP_(L"x" sup2_ L"/2") L" dx"))
    )
    .Remarks(
        L"This is a remark. "
        L"This is " B_(L"bold") L" and this is " I_(L"italic")
        PARA_(L"This is a paragraph.")
    )
    .Examples(LR"(
This is an example.
</para><para>
It has two paragraphs.
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
