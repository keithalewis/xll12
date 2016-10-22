// fms_newton.h - Self containted Newton root finder.
#pragma once
#include <functional>
#include <limits>

namespace fms {
namespace root1d {

	// function from X to Y
	template<class X, class Y>
	using function = const std::function<Y(X)>&;

	// Solve for f(x) = 0 using the newton method.
	template<class X, class Y>
	X newton(X x0, function<X,Y> f, function<X,Y> df, X eps = 5, int iter = 100) 
	{
		Y dfx = df(x0);

		// Error if dfx is less that machine epsilon.
		if (1 + dfx == 1)
			return std::numeric_limits<X>::quiet_NaN();
		
		X x = x0 - f(x)/dfx;
		while (fabs(x - x0) > eps*std::numeric_limits<X>::epsilon()) {
			x0 = x;
			dfx = df(x0);
			if (1 + dfx == 1)
				return std::numeric_limits<X>::quiet_NaN();

			x = x0 - f(x)/df(x);
		}
	}

} // namespace root1d
} // namespace fms
