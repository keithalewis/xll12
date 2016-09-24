// black.h - Black option forward pricing model.
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

namespace prob {

	inline double normal_cdf(double x)
	{
		return (1 + erf(x/M_SQRT2))/2;
	}

} // namespace prob

namespace black {

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter
	// P(F <= k) where F = f exp(-s^2t/2 + s B_t)
	inline double P(double f, double s, double k, double t)
	{
		return 0; //!!! use appropriate formula
	}

	// P(F exp(s^2t) <= k) where F = f exp(-s^2t/2 + s B_t)
	inline double P_(double f, double s, double k, double t)
	{
		return 0; //!!! use appropriate formula
	}
#pragma warning(pop)

	// E max(k - F, 0) where F = f exp(-s^2t/2 + s B_t)
	inline double put(double f, double s, double k, double t)
	{
		return k*P(f, s, k, t) - f*P_(f, s, k, t);
	}

} // namespace black
