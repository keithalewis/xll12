// black.h - Black option forward pricing model.
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "prob.h"
#include "xll_roots.h"


namespace black {

	// P(F <= k) where F = f exp(-s^2t/2 + s B_t)
	// B_t/sqrt(t) <= (log k/f - s^2t/2)/s sqrt(t)
	inline double P(double f, double s, double k, double t)
	{
		auto srt = s*sqrt(t);

		return prob::normal_cdf(log(k/f)/srt + srt/2);
	}

	// P(F exp(s^2t) <= k) where F = f exp(-s^2t/2 + s B_t)
	inline double P_(double f, double s, double k, double t)
	{
		auto srt = s*sqrt(t);

		return prob::normal_cdf(log(k/f)/srt - srt/2);
	}

	// E max(k - F, 0) where F = f exp(-s^2t/2 + s B_t)
	inline double put_value(double f, double s, double k, double t)
	{
		return k*P(f, s, k, t) - f*P_(f, s, k, t);
	}
	// Derivative of put value with respect to volatility.
	inline double put_vega(double f, double s, double k, double t)
	{
		auto srt = sqrt(t);

		return f*prob::normal_pdf(log(k/f)/(s*srt) + s*srt/2)*srt;
	}
	// Calculate volatility given price.
	inline double implied_volatility(double f, double p, double k, double t)
	{
		auto v = [f,p,k,t](double s) { return p - put_value(f, s, k, t); };

		double s_lo = 0.1, s_hi = 0.3;

		while (v(s_lo) < 0)
			s_lo /= 2;
		while (v(s_hi) > 0)
			s_hi *= 2;

		gsl::root::fsolver solver(gsl_root_fsolver_brent);

		solver.set(v, s_lo, s_hi);

		double epsrel = 1e-8;

		return solver.solve(gsl::root::test_interval(0, epsrel));
	}
#pragma warning(push)
#pragma warning(disable: 100 101)
	// Calculate volatility given price using Newton method.
	inline double implied_volatility2(double f, double p, double k, double t)
	{
		auto v = [f,p,k,t](double s) { return p - put_value(f, s, k, t); };
		auto dv = [f,p,k,t](double s) { return -put_vega(f, s, k, t); };
	
		double s0 = 0.2;

		gsl::root::fdfsolver solver(gsl_root_fdfsolver_newton);

		solver.set(v, dv, s0);

		double epsrel = 1e-8;

		return solver.solve(gsl::root::test_delta(0, epsrel));
	}
#pragma warning(pop) 
} // namespace black
