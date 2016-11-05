// xll_date.cpp - Howard Hinnant's date library
#include "date.h"
#include "../../xll12/xll/xll.h"

using namespace xll;
using namespace date;

static AddIn xai_date(
	Function(XLL_DOUBLE, L"?xll_date", L"XLL.DATE")
	.Arg(XLL_LONG, L"Year", L"is the year.")
	.Arg(XLL_WORD, L"Month", L"is the month.")
	.Arg(XLL_WORD, L"Day", L"is the day..")
	.Category(L"XLL")
	.FunctionHelp(L"Excel compatible date.")
);
double WINAPI xll_date(LONG y, WORD m, WORD d)
{
#pragma XLLEXPORT
	static auto delta = 1970_y/1/1 - 1900_y/1/1;

	return std::chrono::duration_cast<day>(year_month_day(year(y),month(m),day(d)) - 1900_y/1/1).count();
}