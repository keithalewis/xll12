// sample.cpp - AddIn example
#include "../xll/xll.h"

using namespace xll;

AddIn xai_square(
    Function(XLL_DOUBLE, L"?xll_square", L"XLL.SQUARE")
    .Arg(XLL_DOUBLE, L"x", L"is the number to be doubled.")
    .FunctionHelp(L"Double a number.")
    .Category(L"XLL")
);
double WINAPI xll_square(double x)
{
#pragma XLLEXPORT

    return x*x;
}