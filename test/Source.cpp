#include "Header.h"

using namespace xll;

AddIn xai_test(
    Documentation(LR"(This is documentation for the test add-in.)")
    .FunctionText(L"test")
);

// construct C++ object in Excel
static AddIn xai_base(
    Function(XLL_HANDLE, L"?xll_base", L"XLL.BASE")
    .Arg(XLL_SHORT, L"int", L"is an int")
    .Uncalced() // must specify for handles
    .Category(L"XLL")
    .FunctionHelp(L"Handle example")
    .Documentation(L"Documentation.")
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

#ifdef _DEBUG

void
test_base_derived(void)
{
    base b(1);
    ensure(1 == b.value());

    derived d(2, 3);
    ensure(2 == d.value());
    ensure(3 == d.value2());

    handle<base> hb(new base(1));
    HANDLEX xhb = hb.get();
    handle<base> bb(xhb);
    ensure(bb);
    ensure(1 == bb->value());

    handle<derived> hd(new derived(2, 3));
    HANDLEX xhd = hd.get();
    handle<derived> dd(xhd);
    ensure(2 == dd->value());
    ensure(3 == dd->value2());
     
}
/*
struct foo {
    char* s_;
};
foo* foo_new(const char* s)
{
    foo* pf = new foo;
    pf->s_ = new char[strlen(s) + 1];
    strcpy(pf->s_, s);

    return pf;
}
void foo_delete(foo* pf)
{
    delete[] pf->s_;
    delete pf;
}
void
test_delete(void)
{
    handle<foo> h1(new foo());
    //	handle<foo> h2(foo_new("abc")); // leaks
    handle<foo, void(*)(foo*)> h3(foo_new("def"), foo_delete);
}
static AddIn xai_test_foo(
    "?xll_test_foo", XLL_HANDLE XLL_CSTRING XLL_UNCALCED,
    "TEST.FOO", "Str"
);
HANDLEX WINAPI
xll_test_foo(const char* s)
{
#pragma XLLEXPORT
    handle<foo, void(*)(foo*)> h(foo_new(s), foo_delete);

    return h.get();
}
static AddIn xai_test_foo_get(
    "?xll_test_foo_get", XLL_CSTRING XLL_HANDLE,
    "TEST.FOO.GET", "Str"
);
const char* WINAPI
xll_test_foo_get(HANDLEX h)
{
#pragma XLLEXPORT
    // handle<foo> h_(h); is different bucket
    handle<foo, void(*)(foo*)> h_(h);

    return h_ ? h_->s_ : 0;
}
*/
int
test_handles(void)
{
    try {
        //		_crtBreakAlloc = 1289;
        test_base_derived();
        //test_delete();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return 0;
    }

    return 1;
}
static Auto<Open> xao_handles(test_handles);

#endif // _DEBUG
