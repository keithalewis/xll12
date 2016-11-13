// black.h - Black option forward pricing model.
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "prob.h"
#include "fms_newton.h"

namespace fms {
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
		auto v = [f,p,k,t](double s) { return -p + put_value(f, s, k, t); };
		auto dv = [f,k,t](double s) { return put_vega(f, s, k, t); };

		double s = .2;

		return root1d::newton<double,double>(s, v, dv);
	}
} // namespace black
} // namespace fms