// xll_date.cpp - Date routines
#include "xll_date.h"
#include "../../xll12/xll/xll.h"

using namespace xll;

XLL_ENUM(US30_360, XLL_US30_360, L"XLL", L"US 30/360 day count basis.");
XLL_ENUM(ACTUAL_ACTUAL, XLL_ACTUAL_ACTUAL, L"XLL", L"actual/actual day count basis.");
XLL_ENUM(ACTUAL_360, XLL_ACTUAL_360, L"XLL", L"actual/360 day count basis.");
XLL_ENUM(ACTUAL_365, XLL_ACTUAL_365, L"XLL", L"actual/365 day count basis.");
XLL_ENUM(EU30_360, XLL_EU30_360, L"XLL", L"Europeean 30/360 day count basis.");

static AddIn xai_dcf(
	Function(XLL_DOUBLE, L"?xll_dcf", L"XLL.DCF")
	.Arg(XLL_DOUBLE, L"begin_date", L"is the first date of the interval.")
	.Arg(XLL_DOUBLE, L"end_date", L"is the last date of the interval")
	.Arg(XLL_LONG, L"dcb", L"is the day count basis..")
	.Category(L"XLL")
	.FunctionHelp(L"Return the day count fraction of an interval using a day count basis.")
);
double WINAPI xll_dcf(double begin, double end, day_count_basis dcb)
{
#pragma XLLEXPORT
	return DCF(begin, end, dcb);
}