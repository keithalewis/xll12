// function.cpp
#include <functional>
#include "../xll/xll.h"

using namespace xll;

AddIn xai_call_function(
    Function(XLL_DOUBLE, L"?xll_call_function", L"CALL.FUNCTION")
    .Arg(XLL_HANDLE, L"handle", L"is a handle to a function.")
    .Arg(XLL_DOUBLE, L"arg", L"is the function argument.")
    .FunctionHelp(L"Call a function")
);
double WINAPI xll_call_function(HANDLEX h, double arg)
{
#pragma XLLEXPORT
    xll::handle<std::function<double(double)>> h_(h);

    return (*h_)(arg);
}

double foo(double x)
{
    return 2*x;
}
AddIn xai_foo_function(
    Function(XLL_HANDLE, L"?xll_foo_function", L"FOO.FUNCTION")
    .Uncalced()
    .FunctionHelp(L"Return a handle to a function.")
);
HANDLEX WINAPI xll_foo_function()
{
#pragma XLLEXPORT
    xll::handle<std::function<double(double)>> h(new std::function<double(double)>(foo));

    return h.get();
}
