// handle.cpp - test handle class
#include "../xll/xll.h"

using namespace xll;

class Base {
	int i_;
public:
	Base(int i)
		: i_(i)
	{ }
	virtual ~Base() // for RTTI
	{ }
	int value(void) const
	{
		return i_;
	}
};

static AddIn xai_base(
	Function(XLL_HANDLE, L"?xll_base", L"XLL.BASE")
	.Arg(XLL_LONG, L"Value", L"is an integer value.")
	.FunctionHelp(L"Return a handle to a Base object.")
	.Uncalced()
	.Category(L"XLL")
);
HANDLEX WINAPI xll_base(LONG v)
{
#pragma XLLEXPORT
		handlex h; // a HANDLEX that defaults to a NaN that returns #NUM!

		try {
			handle<Base> h_(new Base(v));

			h = h_.get();
		}
		catch (const std::exception& ex) {
			XLL_ERROR(ex.what());
		}

		return h;
}

static AddIn xai_base_value(
	Function(XLL_LONG, L"?xll_base_value", L"XLL.BASE.VALUE")
	.Arg(XLL_HANDLE, L"Handle", L"is a handle returned by XLL.BASE.")
	.Category(L"XLL")
	.FunctionHelp(L"Return the value of base class.")
);
LONG WINAPI xll_base_value(HANDLEX h)
{
#pragma XLLEXPORT
	LONG v(0);

	try {
		handle<Base> h_(h);

		v = h_->value();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return v;
}

class Derived : public Base {
	int i2_;
public:
	Derived(int i, int i2)
		: Base(i), i2_(i2)
	{ }
	int value2(void) const
	{
		return i2_;
	}
};

// Put derived classes into the Base bucket.
static AddIn xai_derived(
	Function(XLL_HANDLE, L"?xll_derived", L"XLL.DERIVED")
	.Arg(XLL_LONG, L"Value", L"is an integer value for the base class.")
	.Arg(XLL_LONG, L"Value2", L"is an integer value for the derived class.")
	.Uncalced()
	.Category(L"XLL")
	.FunctionHelp(L"Return a handle to a Derived object.")
);
HANDLEX WINAPI xll_derived(LONG v, LONG v2)
{
#pragma XLLEXPORT
		handlex h;

		try {
			// put derived into Base bag
			handle<Base> h_(new Derived(v, v2));

			h = h_.get();
		}
		catch (const std::exception& ex) {
			XLL_ERROR(ex.what());
		}

		return h;
}

// Use dynamic cast to get the Derived from the Base.
static AddIn xai_derived_value(
	Function(XLL_LONG, L"?xll_derived_value2", L"XLL.DERIVED.VALUE2")
	.Arg(XLL_HANDLE, L"Handle", L"is a handle returned by XLL.DERIVED.")
	.Category(L"XLL")
	.FunctionHelp(L"Return the second value of derived class.")
);
LONG WINAPI xll_derived_value2(HANDLEX h)
{
#pragma XLLEXPORT
	LONG v(0);

	try {
		handle<Base> h_(h);
		const Derived *pd = dynamic_cast<const Derived*>(h_.ptr());
		ensure (pd != 0);

		v = pd->value2();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return v;
}
