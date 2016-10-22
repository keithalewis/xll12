// fms_newton.h - Self containted Newton root finder.
#pragma once
#include <functional>
#include <limits>

namespace fms {
namespace root1d {

	// Solve for f(x) = 0 using the newton method.
	template<class X, class Y>
	X newton(X x0, const std::function<Y(X)>& f, const std::function<Y(X)>& df, X eps = 2, int iter = 100) 
	{
		Y dfx = df(x0);

		// Error if dfx is less that machine epsilon.
		if (1 + dfx == 1)
			return std::numeric_limits<X>::quiet_NaN();
		
		X x = x0 - f(x0)/dfx;
		while (fabs(x - x0) > eps*std::numeric_limits<X>::epsilon()) {
			x0 = x;
			dfx = df(x0);
			if (1 + dfx == 1 || 0 == iter--)
				return std::numeric_limits<X>::quiet_NaN();

			x = x0 - f(x)/df(x);
		}

		return x;
	}

} // namespace root1d
} // namespace fms

#ifdef _DEBUG
#include <cassert>

void test_fms_root1d_newton()
{
	double x = 1;
	auto f = [](double x) { return x*x - 5; };
	auto df = [](double x) { return 2*x; };

	double sqrt5 = fms::root1d::newton<double,double>(x, f, df);
	assert (fabs(sqrt5 - sqrt(5)) < 2*std::numeric_limits<double>::epsilon());

}

#endif // _DEBUG