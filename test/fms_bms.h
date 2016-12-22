// fms_bms.h - Black Merton Scholes model
#pragma once

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
	double put_value(double r, double s, double sigma, double k, double t)
	{
		return 0; 
	}

	// Use the put_delta formula *from fms_black.h* to value the option delta.

} // namespace bms
} // namespace fms
