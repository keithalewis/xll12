// xll_date.h - Daycount fractions
/*
Thoughts on extending chrono/date to financial library.
year_month_day is year, month, day
day_point is std::chrono::time_point<days>.
Only need two global functions: roll(date, method) and 
day_count_fraction(date, day_count_basis)
*/
#pragma once
#include "../xll/xll.h"

namespace xll {

constexpr double DAYS_PER_YEAR = 365.25;

enum day_of_week {
	DOW_MON, DOW_MONDAY = DOW_MON,
	DOW_TUE, DOW_TUESDAY = DOW_TUE,
	DOW_WED, DOW_WEDNESDAY = DOW_WED,
	DOW_THU, DOW_THURSDAY = DOW_THU,
	DOW_FRI, DOW_FRIDAY = DOW_FRI,
	DOW_SAT, DOW_SATURDAY = DOW_SAT,
	DOW_SUN, DOW_SUNDAY = DOW_SUN,
};

enum frequency {
	FREQ_ANNUAL = 1,
	FREQ_SEMIANNUAL = 2,
	FREQ_QUARTERLY = 4,
	FREQ_MONTHLY = 12,
};

enum day_count_basis {
	DCB_US30_360,
	DCB_ACTUAL_ACTUAL,
	DCB_ACTUAL_360,
	DCB_ACTUAL_365,
	DCB_EU30_360,
};

// Define a class that behaves like a type.
#define CLASS_TYPE(CLASS, TYPE) struct CLASS \
{ TYPE t; CLASS() = default; CLASS(TYPE t) : t(t) {} operator TYPE() { return t; }};

CLASS_TYPE(excel_date, double) // Excel date - days since 1900/1/1
CLASS_TYPE(years, int)
CLASS_TYPE(months, int)
CLASS_TYPE(days, int)

#undef CLASS_TYPE

inline excel_date date(int year, int month, int day)
{
	return 0 + Excel(xlfDate, OPER(year), OPER(month), OPER(day));
}

inline excel_date date_add(excel_date x, years y)
{
	auto X = OPER(x);
	auto Y = Excel(xlfYear, X);
	auto M = Excel(xlfMonth, X);
	auto D = Excel(xlfDay, X);
	auto T = x - floor(x);

	Y = Y + y;

	return Excel(xlfDate, Y, M, D) + T;
}
inline excel_date date_add(excel_date x, months m)
{
	auto X = OPER(x);
	auto Y = Excel(xlfYear, X);
	auto M = Excel(xlfMonth, X);
	auto D = Excel(xlfDay, X);
	auto T = x - floor(x);

	M = M + m;

	return Excel(xlfDate, Y, M, D) + T;
}
inline excel_date date_add(excel_date x, days d)
{
	auto X = OPER(x);
	auto Y = Excel(xlfYear, X);
	auto M = Excel(xlfMonth, X);
	auto D = Excel(xlfDay, X);
	auto T = x - floor(x);

	D = D + d;

	return Excel(xlfDate, Y, M, D) + T;
}

// Day count fraction.
inline double day_count_fraction(excel_date begin, excel_date end, day_count_basis dcb)
{
	return xll::Excel(xlfYearfrac, OPER(begin), OPER(end), OPER(dcb));
}

// 0 indexed Nth day in month
inline excel_date date_nth_day_of_week(excel_date date, int nth, day_of_week dow) 
{
	excel_date fom = date - xll::Excel(xlfDay, OPER(date)) + 1;
	auto fdom = xll::Excel(xlfWeekday, OPER(fom), OPER(3)); // first day of month with Monday = 0

	return fom + 7*nth + (dow >= fdom ? 0 : 7) + dow - fdom;
}

} // namespace xll