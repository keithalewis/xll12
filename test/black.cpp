#include "black.h"
#include "../xll/xll.h"


using namespace xll;
using namespace black;

// Set up GSL error handling.
Auto<Open> xao_gsl([] {
	auto handler = [](const char* reason, const char* file, int line, int err) {
		char buf[1024];
		sprintf_s(buf, "%s\nfile: %s\nline: %d\nerrno: %d", reason, file, line, err);
		MessageBoxA(0, buf, "GSL Error", MB_OK);
	};
	using handler_t = void(__cdecl*)(const char*, const char*, int, int);
	gsl_set_error_handler(static_cast<handler_t>(handler));

	return TRUE;
});

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

AddIn xai_black_put_value(
	Function(XLL_DOUBLE, L"?xll_black_put_value", L"PUT.VALUE")
	.Arg(XLL_DOUBLE, L"f", L"is the forward..")
	.Arg(XLL_DOUBLE, L"sigma", L"is the volatility..")
	.Arg(XLL_DOUBLE, L"k", L"is the strike..")
	.Arg(XLL_DOUBLE, L"t", L"is the time in years to expiration")
	.Category(L"BLACK")
	.FunctionHelp(L"The forward value of a put option using the Black model")
	.Documentation()
);
double WINAPI xll_black_put_value(double f, double s, double k, double t)
{
#pragma XLLEXPORT
	double value;

	try {
		ensure (f > 0);
		ensure (s > 0);
		ensure (k > 0);
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

// Test for xll_black_put using formula (27) of  http://kalx.net/BlaSch1973.pdf 
// f = x = 100, s = v = 0.2, k = c = 100, t = t* = 0.25, and r = 0. 

	double f = 100, k = 100, s = 0.2, t = 0.25, r = 0;
	double d1 = (log(f/k) + (r + s*s/2)*t)/(s*sqrt(t));
	double d2 = (log(f/k) + (r - s*s/2)*t)/(s*sqrt(t));
	double N1 = prob::normal_cdf(-d1);
	double N2 = prob::normal_cdf(-d2);
	double v = -f*N1 + k*N2;

	ensure (v == black::put_value(f, s, k, t));

TEST_END

AddIn xai_black_put_vega(
	Function(XLL_DOUBLE, L"?xll_black_put_vega", L"PUT.VEGA")
	.Arg(XLL_DOUBLE, L"f", L"is the forward..")
	.Arg(XLL_DOUBLE, L"sigma", L"is the volatility..")
	.Arg(XLL_DOUBLE, L"k", L"is the strike..")
	.Arg(XLL_DOUBLE, L"t", L"is the time in years to expiration")
	.Category(L"BLACK")
	.FunctionHelp(L"The forward vega of a put option using the Black model")
	.Documentation()
);
double WINAPI xll_black_put_vega(double f, double s, double k, double t)
{
#pragma XLLEXPORT
	double value = std::numeric_limits<double>::quiet_NaN();

	try {
		ensure (f > 0);
		ensure (s > 0);
		ensure (k > 0);
		ensure (t > 0);

		value = black::put_vega(f, s, k, t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return value;
}
#pragma warning(push)
#pragma warning(disable: 100 702)

//!!! Add arguments, category, and function help.
AddIn xai_black_implied_volatilty(
	Function(XLL_DOUBLE, L"?xll_black_implied_volatility", L"IMPLIED.VOLATILITY")
);
double WINAPI xll_black_implied_volatility(double f, double p, double k, double t)
{
#pragma XLLEXPORT
	double s = std::numeric_limits<double>::quiet_NaN();

	try {
		//!!! ensure parameters are valid

		//!!! compute the implied volatility
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return s;
}

TEST_BEGIN(implied_volatility)

//!!! Test the implied volatility for the values
// f = 100, s = 0.2, k = 100, and t  = 0.25. 

TEST_END
