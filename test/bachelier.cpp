// bachelier.cpp - The Bachelier model
#include "../xll/xll.h"
#define _USE_MATH_DEFINES
#include "prob.h"

using namespace xll;

// In the Bachelier model of stock price as Brownain motion, the value of a call is
// E max{F_t - k, 0} = (f - k) N(d) + sigma sqrt(t) N'(d), where N is the normal cdf
// and d = (f - k)/sigma sqrt(t).
// Implement a function for valuing the call option in the Bachelier model

inline double bachelier_call_value(double f, double sigma, double k, double t)
{
	ensure (f > 0);
	ensure (sigma > 0);
	ensure (k > 0);
	ensure (t > 0);

	double srt = sigma*sqrt(t);
	double d = (f - k)/srt;

	return (f - k) * prob::normal_cdf(d) + srt*prob::normal_pdf(d);
}

// Implement an Excel add-in for this function called BACHELIER.CALL.VALUE.
static AddInX xai_bachelier_call_value(
	FunctionX(XLL_DOUBLE, L"?xll_bachelier_call_value", L"BACHELIER.CALL.VALUE")
	.Arg(XLL_DOUBLE, L"f", L"is the forward underlying value.")
	.Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
	.Arg(XLL_DOUBLE, L"k", L"is the strike.")
	.Arg(XLL_DOUBLE, L"t", L"is the time in years to expiration.")
	.Category(L"XLL")
	.FunctionHelp(L"Bachelier call value.")
);
double WINAPI xll_bachelier_call_value(double f, double sigma, double k, double t)
{
#pragma XLLEXPORT
	double v = std::numeric_limits<double>::quiet_NaN();

	try {
		v = bachelier_call_value(f, sigma, k, t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return v;
}

// Implement tests for the bachelier_call_value function.
TEST_BEGIN(bachelier_call)
// Test f = k
double f = 100;
double sigma = .2;
double k = 100;
double t = 1;
// F_t = f + sigma B_t
// E max{F_t - f, 0} = E max{sigma B_t, 0}
//  = sigma sqrt(t) E max{Z, 0} where Z is standard normal
//  = sigma sqrt(t) int_0^infty z exp(-z^2/2) dz/sqrt(2 pi)
//  = sigma sqrt(t) -exp(-z^2/2)/sqrt(2 pi)|_0^infty
//  = sigma sqrt(t) (0 + 1/sqrt(2 pi))
double v = bachelier_call_value(f, sigma, k, t);
double v_ = sigma*sqrt(t)/M_SQRT2PI;
ensure (v == v_);
TEST_END
// Create a spreadsheet that calls this function.     

