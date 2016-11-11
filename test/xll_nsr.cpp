// xll_nsr.cpp - Normal short rate add-in
#include "fms_nsr.h"
#include "../xll/xll.h"

using namespace xll;

// Implement NSR.E.LOGD
static AddIn xai_nsr_e_logd(
	Function(XLL_DOUBLE, L"?xll_nsr_e_logd", L"NSR.E.LOGD")
	.Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
	.Arg(XLL_DOUBLE, L"Dt", L"is discount to t.")
	.Arg(XLL_DOUBLE, L"Du", L"is discount to u.")
	.Arg(XLL_DOUBLE, L"t", L"is time for the price.")
	.Arg(XLL_DOUBLE, L"u", L"is maturity.")
	.Category(L"NSR")
	.FunctionHelp(L"Expected value of log price ....")
);
double WINAPI xll_nsr_e_logd(double sigma, double Dt, double Du, double t, double u)
{
#pragma XLLEXPORT
	return fms::nsr::E_logD(sigma, Dt, Du, t, u);
}

// Implement NSR.VAR.LOGD
static AddIn xai_nsr_var_logd(
	Function(XLL_DOUBLE, L"?xll_nsr_var_logd", L"NSR.VAR.LOGD")
	.Arg(XLL_DOUBLE, L"sigma", L"is the volatility.")
	.Arg(XLL_DOUBLE, L"Dt", L"is discount to t.")
	.Arg(XLL_DOUBLE, L"Du", L"is discount to u.")
	.Arg(XLL_DOUBLE, L"t", L"is time for the price.")
	.Arg(XLL_DOUBLE, L"u", L"is maturity.")
	.Category(L"NSR")
	.FunctionHelp(L"Variance of log price ....")
);
double WINAPI xll_nsr_var_logd(double sigma, double Dt, double Du, double t, double u)
{
#pragma XLLEXPORT
	return fms::nsr::E_logD(sigma, Dt, Du, t, u);
}

