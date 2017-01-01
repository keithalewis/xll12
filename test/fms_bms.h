// fms_bms.h - Black Merton Scholes model
#pragma once
#include "fms_black.h"

namespace fms {
namespace bms {

	// The Black, Merton, and Scholes put option value is
	// v_bms = D E max{k - S_t, 0}, where S_t = s/D exp(-sigma^2 t/2 + sigma B_t),
	// where D is the discount to expiration time t, s is the current stock price,
	// sigma is the volatility and k is the put strike.
	// Using the cost of carry formula for the forward f = s/D we have
	//  v_bms = D E {k - S_t, 0}
	//        = D E {k - F_t, 0}
	// Use the put_value formula *from fms_black.h* to value the option.
	double put_value(double D, double s, double sigma, double k, double t)
	{
		double f = s / D;

		return D*black::put_value(f, sigma, k, t); 
	}

	// Use the put_delta formula *from fms_black.h* to value the option delta.
	// Delta is d/ds v_bms = d/ds v_black = D*black::put_delta df/ds = black::put_delta.
	double put_delta(double D, double s, double sigma, double k, double t)
	{
		double f = s / D;

		return D*black::put_delta(f, sigma, k, t);
	}

} // namespace bms
} // namespace fms
