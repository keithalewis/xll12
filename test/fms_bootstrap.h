// fms_bootstrap.h - Bootstrap a piecewise flat forward curve.
#pragma once
#include <array>
#include "fms_newton.h"
#include "fms_pwflat.h"

namespace fms {
namespace pwflat {
	
	// Extrapolate curve to match price with present value.
	template<class T, class F>
	inline F bootstrap(F p, size_t m, const T* u, const F* c, size_t n, const T* t, const F* f)
	{
		auto pv = [p, m, u, c, n, t, f](double _f) {
			return -p + present_value(m, u, c, n, t, f, _f);
		};
		auto dpv = [m, u, c, n, t, f](double _f) {
			return duration_extrapolated(m, u, c, n, t, f, _f);
		};

		// Use fms::root1d::newtion to solve for the extrapolated value.
		F _f = 0;
		if (n > 0)
			_f = f[n-1];

		_f = root1d::newton<F,F>(_f, pv, dpv);

		return _f;
	}

}
} // namespace fms
