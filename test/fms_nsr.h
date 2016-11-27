// fms_nsr.h - Normal short rate model.
#pragma once
#include <cmath>

namespace fms {
namespace nsr {

	// D_t(u) is the price at time t of a zero maturing at u.
	// From http://kalx.net/columbia/nsr.html we have for constant sigma
	// -log D_t(u) = \int_t^u f(s) ds + sigma^2 ut(u - t)/2 + sigma (u - t) B_t.
	// Let Dt = D_0(t) and Du = D_0(u).
	
	// The expected value of log D_t(u)
	template<class T, class X>
	inline X E_logD(X sigma, X Dt, X Du, T t, T u)
	{
		// D_0(t) = exp(-int_0^t f(s) ds)
		// D_0(u)/D_0(t) = exp(-int_t^u f(s) ds)
		// - int_t^u f(s) ds - sigma^2 ut(u - t)/2 + 0
		// log D_0(u)/D_0(t) - sigms...
	
		return log(Du/Dt) - sigma*sigma*u*t*(u - t)/2;
	}
	// The variance of log D_t(u)
	template<class T, class X>
	inline X Var_logD(X sigma, X Dt, X Du, T t, T u)
	{
		return sigma*sigma*(u - t)*(u - t)*t;
	}

	// Covariance
	template<class C, class X = decltype(**C)>
	inline X Cov(size_t n, C cov)
	{
		X x{0};

		for (size_t i = 0; i < n; ++i) {
			x += cov[i][i];
			for (size_t j = i + 1; j < n; ++j)
				x += 2*cov[i][j];
		}

		return x;
	}

} // namespace nsr
} // namespace fms
