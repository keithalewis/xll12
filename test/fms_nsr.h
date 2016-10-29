// fms_nsr.h - Normal short rate model.
#pragma once

namespace fms {
namespace nsr {

	// D_t(u) is the price at time t of a zero maturing at u.
	// From http://kalx.net/columbia/nsr.html we have for constant sigma
	// -log D_t(u) = \int_t^u f(s) ds + sigma^2 ut(u - t)/2 + sigma (u - t) B_t.
	// Let Dt = D_0(t) and Du = D_0(u).
	
	// !!! The expected value of log D_t(u)
	template<class T, class X>
	inline X E_logD(X sigma, X Dt, X Du, T t, T u)
	{
		return 0;
	}
	// !!! The variance of log D_t(u)
	template<class T, class X>
	inline X Var_logD(X sigma, X Dt, X Du, T t, T u)
	{
		return 0;
	}

} // namespace nsr
} // namespace fms
