// xll_bootstrap.cpp - Bootstrap a piecewise flat forward curve.
#include <vector>
#include "../xll/ensure.h"
#include "fms_bootstrap.h"
#include "../xll/xll.h"

using namespace std;
using namespace fms;
using namespace xll;

#ifdef _DEBUG
TEST_BEGIN(test_fms_root1d_newton)

test_fms_root1d_newton();

TEST_END

TEST_BEGIN(test_fms_bootstrap)

double eps = numeric_limits<double>::epsilon();
vector<double> u{.5, 1.5, 2.5}, c{3, 2, 1};
vector<double> t{1,2,3}, f{.01, .02, .03};

size_t n = t.size();

double p1 = pwflat::present_value(1, &u[0], &c[0], n, &t[0], &f[0]);
double f0 = pwflat::bootstrap(p1, 1, &u[0], &c[0], 0, &t[0], &f[0]);
ensure (fabs(f0 - f[0]) < 2*eps);

double p2 = pwflat::present_value(2, &u[0], &c[0], n, &t[0], &f[0]);
double f1 = pwflat::bootstrap(p2, 2, &u[0], &c[0], 1, &t[0], &f[0]);
ensure (fabs(f1 - f[1]) < 2*eps);

double p3 = pwflat::present_value(3, &u[0], &c[0], n, &t[0], &f[0]);
double f2 = pwflat::bootstrap(p3, 3, &u[0], &c[0], 2, &t[0], &f[0]);
ensure (fabs(f2 - f[2]) < 4*eps);

TEST_END
#endif