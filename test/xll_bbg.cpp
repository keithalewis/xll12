// xll_bbg.cpp
#include "../xll/xll.h"
#include "fms_bbg.h"

using namespace xll;
using namespace fms;

TEST_BEGIN(bbg)

excel_date today = Excel(xlfToday);
auto cd = bbg::parse_ticker(L"US0003M", 0.01, today);
ensure (cd->size() == 1);
ensure (fabs(cd->time(0) - 0.25) < 1./365);
ensure (cd->cash(0) - (1 + 0.01/4) < 0.001);

TEST_END