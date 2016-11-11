// xll_pwflat.cpp - Addin for piecewise flat curves.
#include "../xll/xll.h"
#include "fms_pwflat.h"

using namespace fms;
using namespace xll;

#ifdef _DEBUG
TEST_BEGIN(fms_pwflat)

test_fms_pwflat();

TEST_END
#endif 

AddIn xai_pwflat_value(
	Function(XLL_DOUBLE, L"?xll_pwflat_value", L"PWFLAT.VALUE")
	.Arg(XLL_DOUBLE, L"u", L"is the time at which to value the curve.")
	.Arg(XLL_FP, L"times", L"are the forward curve times.")
	.Arg(XLL_FP, L"forwards", L"are the forward curve values.")
	.Arg(XLL_DOUBLE, L"_f", L"is the extrapolated forward..")
	.Category(L"PWFLAT")
	.FunctionHelp(L"Value of piecewise flat forward curve.")
);
double WINAPI xll_pwflat_value(double u, const _FP12* pt, const _FP12* pf, double _f)
{
#pragma XLLEXPORT
	double fu = std::numeric_limits<double>::quiet_NaN();

	try {
		int n = size(*pt);
		ensure (n == size(*pf));

		fu = pwflat::value(u, n, pt->array, pf->array, _f);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return fu;
}

AddIn xai_pwflat_integral(
	Function(XLL_DOUBLE, L"?xll_pwflat_integral", L"PWFLAT.INTEGRAL")
	.Arg(XLL_DOUBLE, L"u", L"is the time at which to value the integral the curve.")
	.Arg(XLL_FP, L"times", L"are the forward curve times.")
	.Arg(XLL_FP, L"forwards", L"are the forward curve values..")
	.Category(L"PWFLAT")
	.FunctionHelp(L"Integral of piecewise flat forward curve.")
);
double WINAPI xll_pwflat_integral(double u, const _FP12* pt, const _FP12* pf)
{
#pragma XLLEXPORT
	double fu = std::numeric_limits<double>::quiet_NaN();

	try {
		int n = size(*pt);
		ensure (n == size(*pf));

		fu = pwflat::integral(u, n, pt->array, pf->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return fu;
}

// Implement PWFLAT.DISCOUNT

// Implement PWFLAT.SPOT

AddIn xai_pwflat_present_value(
	Function(XLL_DOUBLE, L"?xll_pwflat_present_value", L"PWFLAT.PRESENT_VALUE")
	.Arg(XLL_FP, L"time", L"are the cash flow times.")
	.Arg(XLL_FP, L"cash", L"are the cash flows.")
	.Arg(XLL_FP, L"times", L"are the forward curve times.")
	.Arg(XLL_FP, L"forwards", L"are the forward curve values..")
	.Category(L"PWFLAT")
	.FunctionHelp(L"Value of piecewise flat forward curve.")
);
double WINAPI xll_pwflat_present_value(const _FP12* pu, const _FP12* pc, const _FP12* pt, const _FP12* pf)
{
#pragma XLLEXPORT
	double pv = std::numeric_limits<double>::quiet_NaN();

	try {
		int m = size(*pu);
		ensure (m == size(*pc));
		int n = size(*pt);
		ensure (n == size(*pf));

		pv = pwflat::present_value(m, pu->array, pc->array, n, pt->array, pf->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pv;
}

// Implement PWFLAT.DURATION

// Implement PWFLAT.DURATION_EXTRAPOLATED
