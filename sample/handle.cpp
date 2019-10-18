// handle.cpp - Using handles to embed C++ objects in Excel.
#include "../xll/xll.h"

using namespace xll;

AddIn xal_xll_category(
    Document(L"XLL")
    .Category(L"XLL")
    .Documentation(LR"(
This object will generate documentation for the XLL category.
)")
);

namespace xll {

	// Simple base class.
	class base {
		int data_;
	public:
		base(int data = 0)
			: data_(data)
		{ }
		virtual ~base() // for RTTI
		{ }
		int value(void) const
		{
			return data_;
		}
	};

	// Simple derived class.
	class derived : public base {
		int data_;
	public:
		derived(int bdata = 0, int ddata = 0)
			: base(bdata), data_(ddata)
		{ }
		~derived()
		{ }
		int value2(void) const
		{
			return data_;
		}
	};

}

// Construct C++ object in Excel
static AddIn xai_base(
    Function(XLL_HANDLE, L"?xll_base", L"XLL.BASE")
    .Arg(XLL_SHORT, L"int", L"is an int")
    .Uncalced() // Must specify when creating handles.
    .Category(L"XLL")
    .FunctionHelp(L"Handle example")
    .Documentation(L"Return handle to base class.")
);
HANDLEX WINAPI
xll_base(short b)
{
#pragma XLLEXPORT
    handlex h; // lower case handlex defaults to #NUM!
    
    try {
        handle<base> h_(new base(b));
 
        h = h_.get();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
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
    int value = 0;

    try {
        handle<base> b_(h);

        value = b_->value();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return value;
}

// XLL.BASE.VALUE(XLL.DERIVED(b,d)) returns b.
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

        h = h_.get();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
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
    int value2 = 0;

    try {
        // use RTTI
        derived *pd = dynamic_cast<derived*>(handle<base>(h).ptr());
        ensure(pd != nullptr || !"dynamic_cast failed");

        value2 = pd->value2();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return value2;
}