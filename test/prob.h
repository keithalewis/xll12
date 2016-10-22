// prob.h - Probability related functions.
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "poly.h"

#ifndef M_SQRTPI
#define M_SQRTPI   1.77245385090551602729816748334      /* sqrt(pi) */
#endif
#ifndef M_SQRT2
#define M_SQRT2    1.41421356237309504880168872421      /* sqrt(2) */
#endif

constexpr double M_SQRT2PI = M_SQRT2*M_SQRTPI;

namespace prob {

	inline double normal_pdf(double x)
	{
		return exp(-x*x/2)/(M_SQRT2PI);
	}
	inline double normal_cdf(double x)
	{
		return (1 + erf(x/M_SQRT2))/2;
	}

	// n-th derivative of cumulative normal distribution
	// Phi^(n)(x) = (-1)^{n-1} phi(x) H_{n-1}(x)
	inline double normal_dist(double x, int n)
	{
		if (n == 0)
			return normal_cdf(x);
		if (n == 1)
			return normal_pdf(x);

		return (n%2 == 0 ? -1 : 1)*normal_pdf(x)*poly::Hermite(n-1, x);
	}

	// Formula (3) from http://kalx.net/general-formula-option.pdf
	// where kappa are the cumulants of X. Since cumulants start at 1
	// assume kappa[0] is the first cumulant.
	inline double njr_cdf(double x, int n, const double* kappa)
	{
		double Phi = normal_cdf(x);
		double phi = normal_pdf(x);

		double s = 0;
		double n_ = 1;
		for (int i = 0; i < n; ++i) {
			s += poly::Bell(i + 1, kappa)*poly::Hermite(i, x)/n_;
			n_ *= (i + 2);
		}
		return Phi - phi*s;
	}

} // namespace prob

