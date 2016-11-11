// xll_date.h - Daycount fractions
#pragma once
#include "../xll/xll.h"

namespace xll {

constexpr double DAYS_PER_YEAR = 365.25;

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
{ TYPE t; CLASS(TYPE t = 0) : t(t) {} operator TYPE() { return t; }};

CLASS_TYPE(excel_date, double) // Excel date - days since 1900/1/1
CLASS_TYPE(years, int)
CLASS_TYPE(months, int)
CLASS_TYPE(days, int)

#undef CLASS_TYPE

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

} // namespace xll