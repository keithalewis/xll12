// xll_date.cpp - Date routines
#include "xll_date.h"
#include "../../xll12/xll/xll.h"

using namespace xll;

XLL_ENUM(DAYS_PER_YEAR, DAYS_PER_YEAR, L"XLL", L"Number of days per year.");

XLL_ENUM(DCB_US30_360, DCB_US30_360, L"XLL", L"US 30/360 day count basis.");
XLL_ENUM(DCB_ACTUAL_ACTUAL, DCB_ACTUAL_ACTUAL, L"XLL", L"actual/actual day count basis.");
XLL_ENUM(DCB_ACTUAL_360, DCB_ACTUAL_360, L"XLL", L"actual/360 day count basis.");
XLL_ENUM(DCB_ACTUAL_365, DCB_ACTUAL_365, L"XLL", L"actual/365 day count basis.");
XLL_ENUM(DCB_EU30_360, DCB_EU30_360, L"XLL", L"European 30/360 day count basis.");

XLL_ENUM(FREQ_ANNUAL, FREQ_ANNUAL, L"XLL", L"One payment per year.");
XLL_ENUM(FREQ_SEMIANNUAL, FREQ_SEMIANNUAL, L"XLL", L"Two payments per year.");
XLL_ENUM(FREQ_QUARTERLY, FREQ_QUARTERLY, L"XLL", L"Four payments per year.");
XLL_ENUM(FREQ_MONTHLY, FREQ_MONTHLY, L"XLL", L"Twelve payments per year.");

AddIn xai_date_add_years(
	Function(XLL_DOUBLE, L"?xll_date_add_years", L"DATE.ADD.YEARS")
	.Arg(XLL_DOUBLE, L"date", L"is the date.")
	.Arg(XLL_LONG, L"years", L"is the number of years to add to date.")
	.Category(L"XLL")
	.FunctionHelp(L"Add years to date.")
);
#ifdef _DEBUG
TEST_BEGIN(date_add_years)

excel_date d = Excel(xlfDate, OPER(2016), OPER(7), OPER(8));
excel_date d1;
d1 = date_add(d, years(1));
ensure (Excel(xlfYear, OPER(d1)) == 2016 + 1);
d1 = date_add(d, years(-2));
ensure (Excel(xlfYear, OPER(d1)) == 2016 - 2);
TEST_END
#endif // _DEBUG

double WINAPI xll_date_add_years(double x, years y)
{
#pragma XLLEXPORT
	return date_add(x, y);
}
AddIn xai_date_add_months(
	Function(XLL_DOUBLE, L"?xll_date_add_months", L"DATE.ADD.MONTHS")
	.Arg(XLL_DOUBLE, L"date", L"is the date.")
	.Arg(XLL_LONG, L"months", L"is the number of months to add to date.")
	.Category(L"XLL")
	.FunctionHelp(L"Add months to date.")
);
#ifdef _DEBUG
TEST_BEGIN(date_add_months)
ensure ("!!! [Zha*] add tests");
TEST_END
#endif // _DEBUG

double WINAPI xll_date_add_months(double x, months m)
{
#pragma XLLEXPORT
	return date_add(x, m);
}
AddIn xai_date_add_days(
	Function(XLL_DOUBLE, L"?xll_date_add_days", L"DATE.ADD.DAYS")
	.Arg(XLL_DOUBLE, L"date", L"is the date.")
	.Arg(XLL_LONG, L"days", L"is the number of days to add to date.")
	.Category(L"XLL")
	.FunctionHelp(L"Add days to date.")
);
double WINAPI xll_date_add_days(double x, days d)
{
#pragma XLLEXPORT
	return date_add(x, d);
}
#ifdef _DEBUG
TEST_BEGIN(date_add_days)
ensure ("!!! [Zha*] add tests");
TEST_END
#endif // _DEBUG

AddIn xai_day_count_fraction(
	Function(XLL_DOUBLE, L"?xll_day_count_fraction", L"DAY.COUNT.FRACTION")
	.Arg(XLL_DOUBLE, L"begin", L"is the first date of the interval.")
	.Arg(XLL_DOUBLE, L"end", L"is the last date of the interval")
	.Arg(XLL_LONG, L"dcb", L"is the day count basis..")
	.Category(L"XLL")
	.FunctionHelp(L"Return the day count fraction of an interval using a day count basis.")
);
double WINAPI xll_day_count_fraction(double begin, double end, day_count_basis dcb)
{
#pragma XLLEXPORT
	return day_count_fraction(begin, end, dcb);
}
#ifdef _DEBUG
TEST_BEGIN(day_count_fraction)
ensure ("!!! [Zho*] Google some references and add tests");
TEST_END
#endif // _DEBUG
