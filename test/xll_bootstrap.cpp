// xll_bootstrap.cpp - Bootstrap a piecewise flat forward curve.
#include <vector>
#include "xll_pwflat.h"
#include "xll_instrument.h"
#include "fms_bootstrap.h"

using namespace std;
using namespace fms;
using namespace xll;

static AddIn xai_xll_bootstrap(
	Function(XLL_HANDLE, L"?xll_bootstrap", L"PWFLAT.BOOTSTRAP")
	.Arg(XLL_FP, L"instruments", L"is an array of instrument handles created from INSTRUMENT.* functions.")
	.Arg(XLL_FP, L"prices", L"is an array of corresponding instrument prices.")
	.Uncalced()
	.Category(L"XLL")
	.FunctionHelp(L"Return a handle to a piecewise flat forward curve.")
);
HANDLEX WINAPI xll_bootstrap(const _FP12* pi, const _FP12* pp)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<fp_pwflat> h_(new fp_pwflat{});
		
		for (auto i = 0; i < size(*pi); ++i) {
			handle<fp_instrument> i_(pi->array[i]);
			ensure (i_);
			double t = i_->maturity();
			double f = pwflat::bootstrap(pp->array[i], 
				i_->size(), i_->time(), i_->cash(), 
				h_->size(), h_->time(), h_->forward());
			h_->push_back(t, f);
		}

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

#ifdef _DEBUG

TEST_BEGIN(test_fms_bootstrap)

test_fms_root1d_newton();

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

#endif // _DEBUG