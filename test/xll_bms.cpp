// xll_bms.cpp - Black Merton Scholes model
#include "fms_bms.h"
#include "../xll/xll.h"

using namespace fms;
using namespace xll;

// Implement add-in functions BMS.PUT.VALUE and BMS.PUT.DELTA
// using functions from fms_bms.h.
AddIn xai_bms_put_value(
	Function(XLL_DOUBLE, L"?xll_bms_put_value", L"BMS.PUT.VALUE")
	.Arg(XLL_DOUBLE, L"D", L"is the discount to option expiration.")
	.Arg(XLL_DOUBLE, L"s", L"is the spot.")
	.Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
	.Arg(XLL_DOUBLE, L"k", L"is the strike.")
	.Arg(XLL_DOUBLE, L"t", L"is the time in years to expiration.")
	.Category(L"BMS")
	.FunctionHelp(L"The value of a put option using the Black/Merton/Scholes model")
	.Documentation()
);
double WINAPI xll_bms_put_value(double D, double s, double sigma, double k, double t)
{
#pragma XLLEXPORT
	double value;

	try {
		ensure(D > 0);
		ensure(s > 0);
		ensure(sigma > 0);
		ensure(k > 0);
		ensure(t > 0);

		value = bms::put_value(D, s, sigma, k, t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		value = std::numeric_limits<double>::quiet_NaN();
	}

	return value;
}

AddIn xai_bms_put_delta(
	Function(XLL_DOUBLE, L"?xll_bms_put_delta", L"BMS.PUT.DELTA")
	.Arg(XLL_DOUBLE, L"D", L"is the discount to option expiration.")
	.Arg(XLL_DOUBLE, L"s", L"is the spot.")
	.Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
	.Arg(XLL_DOUBLE, L"k", L"is the strike.")
	.Arg(XLL_DOUBLE, L"t", L"is the time in years to expiration.")
	.Category(L"BMS")
	.FunctionHelp(L"The value of a put delta using the Black/Merton/Scholes model")
	.Documentation()
);
double WINAPI xll_bms_put_delta(double D, double s, double sigma, double k, double t)
{
#pragma XLLEXPORT
	double value;

	try {
		ensure(D > 0);
		ensure(s > 0);
		ensure(sigma > 0);
		ensure(k > 0);
		ensure(t > 0);

		value = bms::put_delta(D, s, sigma, k, t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		value = std::numeric_limits<double>::quiet_NaN();
	}

	return value;
}

// Find and cite values from an independent source to verify
// the formulas for the Black Scholes Mertion put value and put delta.

// Create a spreadsheet calling these two functions using
// D = .9, s = 100, sigma = .20, k = 100, and t = 0.25
// Calculate the symmetric difference (v(s + h) - v(s - h))/2h.
// Find the value of h that minimizes the difference from the
// analytic put_delta.