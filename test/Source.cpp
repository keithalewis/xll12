#include "Header.h"

using namespace xll;

AddIn xai_test(
    Documentation(LR"(This is documentation for the test add-in.)")
);

// construct C++ object in Excel
static AddIn xai_base(
    Function(XLL_HANDLE, L"?xll_base", L"XLL.BASE")
    .Arg(XLL_SHORT, L"int", L"is an int")
    .Uncalced() // must specify for handles
    .Category(L"XLL")
    .FunctionHelp(L"Handle example")
    .Documentation(L"Return handle to base class.")
);
HANDLEX WINAPI
xll_base(short b)
{
#pragma XLLEXPORT
    handlex h; // lower case defaults to #NUM!
    
    try {
        handle<base> h_(new base(b));
        ensure(h_);

        h = h_.get();
    }
    catch (const std::exception& ex) {
        ex.what();

        return 0;
    }

    return h;
}
// member function
static AddInX xai_base_value(
    Function(XLL_LONG, L"?xll_base_value", L"XLL.BASE.VALUE")
    .Arg(XLL_HANDLE, L"Handle", L"is a handle")
    .Category(L"XLL")
    .Documentation(L"Return base value.")
);
LONG WINAPI
xll_base_value(HANDLEX h)
{
#pragma XLLEXPORT
    int value;

    try {
        handle<base> b_(h);
        ensure(b_);

        value = b_->value();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return 0;
    }

    return value;
}

// BASE.VALUE(DERIVED(i1,i2)) returns i2.
static AddInX xai_derived(
    Function(XLL_HANDLE, L"?xll_derived", L"XLL.DERIVED")
    .Arg(XLL_SHORT, L"int", L"is an int") 
    .Arg(XLL_SHORT, L"int2", L"is an int")
    .Uncalced()
    .Category(L"XLL")
    .Documentation(L"Return handle to derived class.")
);
HANDLEX WINAPI
xll_derived(short b, short d)
{
#pragma XLLEXPORT
    handlex h;

    try {
        // put pointer in the base bucket
        handle<base> h_(new derived(b, d));
        ensure(h_);

        h = h_.get();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return 0;
    }

    return h;
}
// member function in derived class
static AddIn xai_derived_value(
    Function(XLL_LONG , L"?xll_derived_value", L"XLL.DERIVED.VALUE2")
    .Arg(XLL_HANDLE, L"handle", L"is a handle")
    .Category(L"XLL")
    .Documentation(L"Return derived second value.")
);
LONG WINAPI
xll_derived_value(HANDLEX h)
{
#pragma XLLEXPORT
    int value2;

    try {
        // use RTTI
        derived *pd = dynamic_cast<derived*>(handle<base>(h).ptr());
        ensure(pd);
        value2 = pd->value2();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return 0;
    }

    return value2;
}