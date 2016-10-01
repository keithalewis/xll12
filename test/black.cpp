#include "black.h"
#include "../xll/xll.h"

using namespace xll;
using namespace black;

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

AddIn xai_black_put_value(
	Function(XLL_DOUBLE, L"?xll_black_put_value", L"PUT.VALUE")
	.Arg(XLL_DOUBLE, L"f", L"is the forward..")
	.Arg(XLL_DOUBLE, L"k", L"is the strike..")
	.Arg(XLL_DOUBLE, L"sigma", L"is the volatility..")
	.Arg(XLL_DOUBLE, L"t", L"is the time in years to expiration")
	.Category(L"BLACK")
	.FunctionHelp(L"The forward value of a put option using the Black model")
	.Documentation()
);
double WINAPI xll_black_put_value(double f, double k, double s, double t)
{
#pragma XLLEXPORT
	double value;

	try {
		ensure (f > 0);
		ensure (k > 0);
		ensure (s > 0);
		ensure (t > 0);

		value = black::put_value(f, s, k, t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		value = std::numeric_limits<double>::quiet_NaN();
	}

	return value;
}

TEST_BEGIN(black_put)

//!!! Write a test for xll_black_put using formula (27) of 
// http://kalx.net/BlaSch1973.pdf for the values
// f = x = 100, s = v = 0.2, k = c = 100, t = t* = 0.25, and r = 0. 

	double f = 100, k = 100, s = 0.2, t = 0.25, r = 0;
	double d1 = (log(f/k) + (r + s*s/2)*t)/(s*sqrt(t));
	double d2 = (log(f/k) + (r - s*s/2)*t)/(s*sqrt(t));
	double N1 = prob::normal_cdf(-d1);
	double N2 = prob::normal_cdf(-d2);
	double v = -f*N1 + k*N2;

	ensure (v == black::put_value(f, s, k, t));

TEST_END