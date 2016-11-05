// xll_date.h - Daycount fractions
#pragma once
#include "../../xll12/xll/xll.h"

namespace xll {

enum day_count_basis {
	US30_360,
	ACTUAL_ACTUAL,
	ACTUAL_360,
	ACTUAL_365,
	EU30_360,
};

inline double DCF(double begin, double end, day_count_basis dcb)
{
	return xll::Excel(xlfYearfrac, OPER(begin), OPER(end), OPER(dcb));
}

} // namespace xll