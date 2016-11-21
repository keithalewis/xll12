// xll_pwflat.h - piecewise flat curves
#pragma once
#include "fms_pwflat.h"
#include "../xll/xll.h"

namespace xll {

	class fp_pwflat {
		FP12 t_, f_;
		double _f;
	public:
		fp_pwflat()
			: _f(std::numeric_limits<double>::quiet_NaN())
		{ }
		fp_pwflat(size_t n, const double* t, const double* f, double _f = std::numeric_limits<double>::quiet_NaN())
			: t_(1,static_cast<COL>(n)), f_(1,static_cast<COL>(n)), _f(_f)
		{
			std::copy(t, t + n, &t_[0]);
			std::copy(f, f + n, &f_[0]);
		}
		fp_pwflat(const fp_pwflat&) = default;
		fp_pwflat& operator=(const fp_pwflat&) = default;
		~fp_pwflat()
		{ }

		size_t size() const
		{
			return t_.size();
		}
		const double* time() const
		{
			return &t_[0];
		}
		const double* forward() const
		{
			return &f_[0];
		}
		fp_pwflat& push_back(double t, double f)
		{
			t_.push_back(t);
			f_.push_back(f);

			return *this;
		}

		double value(double u) const
		{
			return fms::pwflat::value(u, size(), time(), forward(), _f);
		}
		double spot(double u) const
		{
			return fms::pwflat::spot(u, size(), time(), forward(), _f);
		}
		double discount(double u) const
		{
			return fms::pwflat::discount(u, size(), time(), forward(), _f);
		}
	};

} // namespace xll

