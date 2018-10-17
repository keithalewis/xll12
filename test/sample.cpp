// sample.cpp - AddIn example
#include <cmath>
#include "../xll/xll.h"

using namespace xll;

// Register an add-in with Excel.
AddIn xai_square(
    // Return a double from xll_square and call it XLL.SQUARE
    Function(XLL_DOUBLE, L"?xll_square", L"XLL.SQUARE")
    // Take a double argument called x and provide Function Wizard help.  
    .Arg(XLL_DOUBLE, L"x", L"is the number to be doubled.")
    .FunctionHelp(L"Double a number.")
    .Category(L"XLL")
);
// WINAPI specifies the calling convention Excel uses.
double WINAPI xll_square(double x)
{
// Required boilerplate to make xll_square visible to Excel.
#pragma XLLEXPORT

    return x*x;
}

AddIn xai_sqrt(
    Function(XLL_LPOPER, L"?xll_sqrt", L"XLL.SQRT")
    .Arg(XLL_DOUBLE, L"x", L"is the number for which you want the square root.")
    .FunctionHelp(L"Returns the square root of a number.")
    .Category(L"XLL")
);
LPOPER WINAPI xll_sqrt(double x)
{
#pragma XLLEXPORT
    static OPER result;

    try {
        ensure(x >= 0); // like assert but throws runtime_exception
        result = sqrt(x);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        result = OPER(xlerr::Num);
    }

    return &result;
}