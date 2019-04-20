// sample.cpp - Simple example of using AddIn.
#include <cmath>
#include "../xll/xll.h"
#include "../xll/shfb/entities.h"

using namespace xll;

AddIn xai_sample(
	Documentation(LR"(
This object will generate a Sandcastle Helpfile Builder project file.
)"));

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
	.Category(L"Sample")
	// Insert Function description.
	.FunctionHelp(L"Help on XLL.FUNCTION goes here.")
	// Create entry for this function in Sandcastle Help File Builder project file.
	.Documentation(LR"zzz(
Free-form documentation on XLL.FUNCTION goes here.

Newlines are ignored when building documentation with Sandcastle Help File Builder.
<para>
But you can include MAML directives.
</para>
    )zzz"
    L"We can also use C prepocessor string pasting "
    L"to write documentation.\n This newline is also ignored."
    L"Here is " B_(L"bold") L", " I_(L"italic") L", and " U_(L"underlined") L" text."
    L"<para>"
        int_ SUB_(minus_ infin_) SUP_(infin_) I_(L"e") SUP_(L"-x" SUP_(L"2") L"/2") L" dx"
    L"</para>"
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
