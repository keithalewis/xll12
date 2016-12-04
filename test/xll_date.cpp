// xll_date.cpp - Date routines
#include "xll_date.h"
#include "../xll/xll.h"

using namespace xll;

#ifndef CATEGORY
#define CATEGORY L"XLL"
#endif

XLL_ENUM(DAYS_PER_YEAR, DAYS_PER_YEAR, CATEGORY, L"Number of days per year.");

XLL_ENUM(DCB_US30_360, DCB_US30_360, CATEGORY, L"US 30/360 day count basis.");
XLL_ENUM(DCB_ACTUAL_ACTUAL, DCB_ACTUAL_ACTUAL, CATEGORY, L"actual/actual day count basis.");
XLL_ENUM(DCB_ACTUAL_360, DCB_ACTUAL_360, CATEGORY, L"actual/360 day count basis.");
XLL_ENUM(DCB_ACTUAL_365, DCB_ACTUAL_365, CATEGORY, L"actual/365 day count basis.");
XLL_ENUM(DCB_EU30_360, DCB_EU30_360, CATEGORY, L"European 30/360 day count basis.");

XLL_ENUM(DOW_MONDAY, DOW_MONDAY, CATEGORY, L"Monday");
XLL_ENUM(DOW_TUESDAY, DOW_TUESDAY, CATEGORY, L"Tuesday");
XLL_ENUM(DOW_WEDNESDAY, DOW_WEDNESDAY, CATEGORY, L"Wednesday");
XLL_ENUM(DOW_THURSDAY, DOW_THURSDAY, CATEGORY, L"Thursday");
XLL_ENUM(DOW_FRIDAY, DOW_FRIDAY, CATEGORY, L"Friday");
XLL_ENUM(DOW_SATURDAY, DOW_SATURDAY, CATEGORY, L"Saturday");
XLL_ENUM(DOW_SUNDAY, DOW_SUNDAY, CATEGORY, L"Sunday");
XLL_ENUM(DOW_MONDAY, DOW_MON, CATEGORY, L"Monday");
XLL_ENUM(DOW_TUESDAY, DOW_TUE, CATEGORY, L"Tuesday");
XLL_ENUM(DOW_WEDNESDAY, DOW_WED, CATEGORY, L"Wednesday");
XLL_ENUM(DOW_THURSDAY, DOW_THU, CATEGORY, L"Thursday");
XLL_ENUM(DOW_FRIDAY, DOW_FR, CATEGORY, L"Friday");
XLL_ENUM(DOW_SATURDAY, DOW_SAT, CATEGORY, L"Saturday");
XLL_ENUM(DOW_SUNDAY, DOW_SUN, CATEGORY, L"Sunday");

XLL_ENUM(FREQ_ANNUAL, FREQ_ANNUAL, CATEGORY, L"One payment per year.");
XLL_ENUM(FREQ_SEMIANNUAL, FREQ_SEMIANNUAL, CATEGORY, L"Two payments per year.");
XLL_ENUM(FREQ_QUARTERLY, FREQ_QUARTERLY, CATEGORY, L"Four payments per year.");
XLL_ENUM(FREQ_MONTHLY, FREQ_MONTHLY, CATEGORY, L"Twelve payments per year.");

AddIn xai_date_add_years(
	Function(XLL_DOUBLE, L"?xll_date_add_years", L"DATE.ADD.YEARS")
	.Arg(XLL_DOUBLE, L"date", L"is the date.")
	.Arg(XLL_LONG, L"years", L"is the number of years to add to date.")
	.Category(CATEGORY)
	.FunctionHelp(L"Add years to date.")
);
#ifdef _DEBUG
TEST_BEGIN(date_add_years)

excel_date d = Excel(xlfDate, OPER(2016), OPER(7), OPER(8));
excel_date d1;
d1 = date_add(d, years{1});
ensure (Excel(xlfYear, OPER(d1)) == 2016 + 1);
d1 = date_add(d, years{-2});
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
	.Category(CATEGORY)
	.FunctionHelp(L"Add months to date.")
);
#ifdef _DEBUG
TEST_BEGIN(date_add_months)

excel_date d = Excel(xlfDate, OPER(2016), OPER(7), OPER(8));
excel_date d1;
d1 = date_add(d, months{1});
ensure(Excel(xlfMonth, OPER(d1)) == 7 + 1);
d1 = date_add(d, months{-2});
ensure(Excel(xlfMonth, OPER(d1)) == 7 - 2);
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
	.Category(CATEGORY)
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

AddIn xai_date_nth_dow(
	Function(XLL_DOUBLE, L"?xll_date_nth_dow", L"DATE.NTH.DOW")
	.Arg(XLL_DOUBLE, L"date", L"is the date.")
	.Arg(XLL_LONG, L"nth", L"is ordinal increment starting from 1.")
	.Arg(XLL_LONG, L"dow", L"is the day of the week ")
	.Category(CATEGORY)
	.FunctionHelp(L"Return the n-th day of the week for a date year and month.")
);
double WINAPI xll_date_nth_dow(double x, LONG nth, day_of_week dow)
{
#pragma XLLEXPORT
	return date_nth_day_of_week(x, nth, dow);
}

AddIn xai_day_count_fraction(
	Function(XLL_DOUBLE, L"?xll_day_count_fraction", L"DAY.COUNT.FRACTION")
	.Arg(XLL_DOUBLE, L"begin", L"is the first date of the interval.")
	.Arg(XLL_DOUBLE, L"end", L"is the last date of the interval")
	.Arg(XLL_LONG, L"dcb", L"is the day count basis..")
	.Category(CATEGORY)
	.FunctionHelp(L"Return the day count fraction of an interval using a day count basis.")
);
double WINAPI xll_day_count_fraction(excel_date begin, excel_date end, day_count_basis dcb)
{
#pragma XLLEXPORT
	return day_count_fraction(begin, end, dcb);
}
#ifdef _DEBUG
TEST_BEGIN(day_count_fraction)
ensure ("!!! [Zho*] Google some references and add tests");
TEST_END
#endif // _DEBUG

#ifdef _DEBUG
TEST_BEGIN(date_nth_day_of_week)

excel_date nth, date = Excel(xlfDate, OPER(2017), OPER(1), OPER(1));
ensure (Excel(xlfWeekday, OPER(date), OPER(3)) == DOW_SUN);

nth = date_nth_day_of_week(date, 0, DOW_SUN);
ensure (Excel(xlfDay, OPER(nth)) == 1);
nth = date_nth_day_of_week(date, 1, DOW_SUN);
ensure (Excel(xlfDay, OPER(nth)) == 8);
nth = date_nth_day_of_week(date, 2, DOW_SUN);
ensure (Excel(xlfDay, OPER(nth)) == 15);
nth = date_nth_day_of_week(date, 3, DOW_SUN);
ensure (Excel(xlfDay, OPER(nth)) == 22);
nth = date_nth_day_of_week(date, 4, DOW_SUN);
ensure (Excel(xlfDay, OPER(nth)) == 29);

nth = date_nth_day_of_week(date, 0, DOW_MON);
ensure (Excel(xlfDay, OPER(nth)) == 2);
nth = date_nth_day_of_week(date, 4, DOW_MON);
ensure (Excel(xlfDay, OPER(nth)) == 30);

ensure (Excel(xlfDay, OPER(date_nth_day_of_week(date, 2, DOW_WED))) == 18);
TEST_END
#endif // _DEBUG
