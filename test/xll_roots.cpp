// xll_root.cpp - test root finding code
#ifdef _DEBUG
#ifdef assert
#undef assert
#endif
#define assert ensure
#include "xll_roots.h"
#include "../xll/xll.h"

TEST_BEGIN(xll_root)

test_gsl_root_fsolver();
test_gsl_root_fdfsolver();

TEST_END
#endif // _DEBUG