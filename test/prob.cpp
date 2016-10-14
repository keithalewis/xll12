// prob.cpp
#include "prob.h"
#include "../xll/xll.h"

using namespace xll;

AddIn xai_normal_pdf(
	Function(XLL_DOUBLE, L"?xll_normal_pdf", L"NORMAL.PDF")
	.Arg(XLL_DOUBLE, L"x", L"is a number..")
	.Category(L"PROB")
);
double WINAPI xll_normal_pdf(double x)
{
#pragma XLLEXPORT
	return prob::normal_pdf(x);
}

AddIn xai_normal_cdf(
	Function(XLL_DOUBLE, L"?xll_normal_cdf", L"NORMAL.CDF")
	.Arg(XLL_DOUBLE, L"x", L"is a number..")
	.Category(L"PROB")
);
double WINAPI xll_normal_cdf(double x)
{
#pragma XLLEXPORT
	return prob::normal_cdf(x);
}

TEST_BEGIN(normal_cdf)

// Compare normal_cdf to Excel function NORMSDIST.
for (double x = -6; x <= 6; x += 0.1) {
	double Nx  = xll_normal_cdf(x);
	double Nx_ = Excel(xlfNormsdist, OPER(x));
	ensure (fabs(Nx - Nx_) < std::numeric_limits<double>::epsilon());
} 

TEST_END

AddIn xai_njr_cdf(
	Function(XLL_DOUBLE, L"?xll_njr_cdf", L"NJR.CDF")
	.Arg(XLL_DOUBLE, L"x", L"is a number..")
	.Arg(XLL_FP, L"kappa", L"is the array of cumulants")
	.Category(L"PROB")
	.FunctionHelp(L"")
);
double WINAPI xll_njr_cdf(double x, _FP12* kappa)
{
#pragma XLLEXPORT
	double value = std::numeric_limits<double>::quiet_NaN();

	try {
		//!!! call prob::njr_cdf  
		value = x*kappa->array[0];
		ensure (value);
	}
	catch  (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return value;
}

/*!!! Uncomment and implement tests
TEST_BEGIN(njr_cdf)

//!!! Test for the cases kappa = {0, 1} (standard normal) and kappa = {2,3}
// (normal mean 2 variance 3).

TEST_END
*/