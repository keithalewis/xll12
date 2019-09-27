// function.cpp
#include <functional>
#include "../xll/xll.h"

using namespace xll;

AddIn xai_call_function(
    Function(XLL_DOUBLE, L"?xll_call_function", L"FOO.CALL")
    .Arg(XLL_HANDLE, L"handle", L"is a handle to a function.")
    .Arg(XLL_DOUBLE, L"arg", L"is the function argument.")
    .FunctionHelp(L"Call a function")
    .Documentation(L"")
);
double WINAPI xll_call_function(HANDLEX h, double arg)
{
#pragma XLLEXPORT
    xll::handle<std::function<double(double)>> h_(h);

    return (*h_)(arg);
}

double foo(double x)
{
    return 2 * x;
}
AddIn xai_foo_function(
    Function(XLL_HANDLE, L"?xll_foo_function", L"FOO.FUNCTION")
    .Uncalced()
    .FunctionHelp(L"Return a handle to a function.")
);
HANDLEX WINAPI xll_foo_function()
{
#pragma XLLEXPORT
    xll::handle<std::function<double(double)>> h(new std::function{ foo });

    return h.get();
}

AddIn xai_foo_lambda(
    Function(XLL_HANDLE, L"?xll_foo_lambda", L"FOO.LAMBDA")
    .Uncalced()
    .FunctionHelp(L"Return a handle to a lambda.")
);
HANDLEX WINAPI xll_foo_lambda()
{
#pragma XLLEXPORT
    xll::handle<std::function<double(double)>> h(new std::function{ [](double x) { return 3 * x; } });

    return h.get();
}

struct Foo {
    double x;
    Foo(double x) : x(x) {}
    double operator()(double y)
    {
        return x * y;
    }
};
AddIn xai_foo_member(
    Function(XLL_HANDLE, L"?xll_foo_member", L"FOO.MEMBER")
    .Uncalced()
    .FunctionHelp(L"Return a handle to a member.")
);
HANDLEX WINAPI xll_foo_member()
{
#pragma XLLEXPORT

    xll::handle<std::function<double(double)>> h(new std::function{ Foo(4) });

    return h.get();
}
