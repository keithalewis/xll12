// fms_pwflat.h - piecewise flat curve
/*
	f(t) = f[i] if t[i-1] < t <= t[i]; 
	     = _f if t > t[n-1];
	and undefined if t < 0

	|                                   _f
	|        f[1]             f[n-1] o--------
	| f[0] o------          o--------x
	x------x      ... ------x   
	|
	0-----t[0]--- ... ---t[n-2]---t[n-1]
*/
#pragma once
#include <cmath>     // exp
#include <algorithm> // adjacent_find
#include <limits>    // quiet_Nan()
#include <numeric>   // upper/lower_bound

namespace fms {
namespace pwflat {

	// strictly increasing values
	template<class I>
	inline bool monotonic(I b, I e)
	{
		using T = std::iterator_traits<I>::value_type;

		return e == std::adjacent_find(b, e, [](const T& t0, const T&t1) { return t0 >= t1; });
	}
	template<class T>
	inline bool monotonic(size_t n, const T* t)
	{
		return monotonic(t, t + n);
	}

	// piecewise flat curve
	// return f[i] if t[i-1] < u <= t[i], _f if u > t[n-1]
	// assumes t[i] monotonically increasing
	template<class T, class F>
	inline F value(const T& u, size_t n, const T* t, const F* f, const F& _f = std::numeric_limits<F>::quiet_NaN())
	{
#ifdef _DEBUG
		if (!monotonic(n, t))
			return std::numeric_limits<F>::quiet_NaN();
#endif
		if (u < 0)
			return std::numeric_limits<F>::quiet_NaN();
		if (n == 0)
			return _f;

		auto ti = std::lower_bound(t, t + n, u);		

		return ti == t + n ? _f : f[ti - t];
	}

	// int_0^u f(t) dt
	template<class T, class F>
	inline F integral(const T& u, size_t n, const T* t, const F* f, const F& _f = std::numeric_limits<F>::quiet_NaN())
	{
		if (u < 0)
			return std::numeric_limits<T>::quiet_NaN();

		F I{0};
		T t_{0};

		size_t i;
		for (i = 0; i < n && t[i] <= u; ++i) {
			I += f[i] * (t[i] - t_);
			t_ = t[i];
		}
		I += (n == 0 || u > t[n-1] ? _f : f[i]) *(u - t_);

		return I;
	}

	// discount D(u) = exp(-int_0^u f(t) dt)
	template<class T, class F>
	inline F discount(const T& u, size_t n, const T* t, const F* f, const F& _f = std::numeric_limits<F>::quiet_NaN())
	{
		return exp(-integral(u, n, t, f, _f));
	}

	// spot r(u) = (int_0^u f(t) dt)/u
	template<class T, class F>
	inline F spot(const T& u, size_t n, const T* t, const F* f, const F& _f = std::numeric_limits<F>::quiet_NaN())
	{
		return u <= t[0] ? f[0] : integral(u, n, t, f, _f)/u;
	}

	// value of instrument having cash flow c[i] at time u[i]
	template<class T, class F>
	inline F present_value(size_t m, const T* u, const F* c, size_t n, const T* t, const F* f, const F& _f = std::numeric_limits<F>::quiet_NaN())
	{
		F p{0};

		for (size_t i = 0; i < m; ++i)
			p += c[i]*pwflat::discount(u[i], n, t, f, _f);

		return p;
	}

	// derivative of present value wrt parallel shift of forward curve
	template<class T, class F>
	inline F duration(size_t m, const T* u, const F* c, size_t n, const T* t, const F* f, const F& _f = std::numeric_limits<F>::quiet_NaN())
	{
		F d{0};

		for (size_t i = 0; i < m; ++i) {
			d -= u[i]*c[i]*pwflat::discount(u[i], n, t, f, _f);
		}

		return d;
	}

	// derivative of present value wrt parallel shift of forward curve after last curve time
	template<class T, class F>
	inline F duration_extrapolated(size_t m, const T* u, const F* c, size_t n, const T* t, const F* f, const F& _f = std::numeric_limits<F>::quiet_NaN())
	{
		F d{0};

		// first cash flow past end of forward curve
		size_t i0 = (n == 0) ? 0 : std::lower_bound(u, u + m, t[n-1]) - u;
		double t0 = (n == 0) ? 0 : t[n -  1];
		for (size_t i = i0; i < m; ++i) {
			d -= (u[i] - t0)*c[i]*pwflat::discount(u[i], n, t, f, _f);
		}

		return d;
	}
} // pwflat
} // fms

#ifdef _DEBUG
#include <cassert>
#include <vector>

inline void test_fms_pwflat()
{
	using namespace fms::pwflat;

	std::vector<double> t{1,2,3}, f{.1,.2,.3};
	std::vector<double> t_2{ 1 }, f_2{ .1 };
	
	{ // monotonic
		assert (monotonic(std::begin(t), std::end(t)));
		assert (monotonic(std::begin(f), std::end(f)));
		double f2 = f[2];
		f[2] = -1;
		assert (!monotonic(std::begin(f), std::end(f)));
		f[2] = f2;
		assert (!monotonic(std::rbegin(f), std::rend(f)));
	}
	{ // forward
		//0, 0, null, null, null
		assert (isnan(value<int,double>(0, 0, nullptr, nullptr)));
		//1, 0, null, null, null
		assert(isnan(value<int, double>(1, 0, nullptr, nullptr)));
		//-1, 0, null, null, null
		assert(isnan(value<int, double>(-1, 0, nullptr, nullptr)));
		//-1, 0, null, null, 0.2
		assert(isnan(value<int, double>(-1, 0, nullptr, nullptr, 0.2)));
		
		int u;
		u = 1;
		double x{ 0.2 }, x_;
		//1, 0, null, null, 0.2
		x_ = fms::pwflat::value<int, double>(u, 0, nullptr, nullptr, x);
		assert(x_ == x);

		double u_ [] = { -1, 0, 0.5, 1, 1.5 };
		double a_ [] = { 0, 0.1, 0.1, 0.1, 0.2 };

		for (int i = 0; i < 5; i++) {
			if (i == 0 || i == 4) {
				assert(isnan(value<double, double>(u_[i], t_2.size(), t_2.data(), f_2.data())));
			}
			else {
				x_ = fms::pwflat::value<double, double>(u_[i], t_2.size(), t_2.data(), f_2.data());
				assert(x_ == a_[i]);
			}
		}

		for (int i = 0; i < 5; i++) {
			if (i == 0)
				assert(isnan(value<double, double>(u_[i], t_2.size(), t_2.data(), f_2.data(), 0.2)));
			else {
				x_ = fms::pwflat::value<double, double>(u_[i], t_2.size(), t_2.data(), f_2.data(), 0.2);
				assert(x_ == a_[i]);
			}
		}

		for (int i = 0; i < 3; ++i)
			assert (f[i] == value(t[i], t.size(), t.data(), f.data()));
	}
	{ // integral
		double u;
		u = -1;
		assert (isnan(integral(u, t.size(), t.data(), f.data())));
		u = 4;
		assert (isnan(integral(u, t.size(), t.data(), f.data())));
		u = 0;
		assert (0 == integral(u, t.size(), t.data(), f.data()));
		u = 0.5;
		assert (.1*.5 == integral(u, t.size(), t.data(), f.data()));
		u = 1;
		assert (.1 == integral(u, t.size(), t.data(), f.data()));
		u = 1.5;
		assert (.1 + .2*.5 == integral(u, t.size(), t.data(), f.data()));
		u = 2.5;
		assert (.1 + .2 + .3*.5 == integral(u, t.size(), t.data(), f.data()));
		u = 3;
		assert (fabs(.1 + .2 + .3 - integral(u, t.size(), t.data(), f.data())) < 1e-10);
		assert (.1 + .2 + .3 != .6); 
	}
	{ // discount
		double u_[] = { -.5, 0, .5, 1, 1.5, 2, 2.5, 3, 3.5 };
		double f_[] = {0, 0, .05, .1, .2, .3, .45, .6, .7};
		for (int i = 0; i < 9; i++) {
			if (i == 0 || i == 8)
				assert(isnan(discount(u_[i], t.size(), t.data(), f.data())));
			else
				assert(fabs(exp(-f_[i]) - discount(u_[i], t.size(), t.data(), f.data())) < 1e-10);
		}

		for (int i = 0; i < 9; i++) {
			if (i == 0)
				assert(isnan(discount(u_[i], t.size(), t.data(), f.data(), 0.2)));
			else
				assert(fabs(exp(-f_[i]) - discount(u_[i], t.size(), t.data(), f.data(), 0.2)) < 1e-10);
		}
	}
	{ // spot
		double u_[] = { -.5, 0, .5, 1, 1.5, 2, 2.5, 3, 3.5 };
		double f_[] = { .1, .1, .1, .1, .2/1.5, .3/2, .45/2.5, .6/3, .7/3.5 };
		for (int i = 0; i < 9; i++) {
			if (i == 8)
				assert(isnan(spot(u_[i], t.size(), t.data(), f.data())));
			else
				assert(fabs(f_[i] - spot(u_[i], t.size(), t.data(), f.data())) < 1e-10);
		}

		for (int i = 0; i < 9; i++) {
			assert(fabs(f_[i] - spot(u_[i], t.size(), t.data(), f.data(), 0.2)) < 1e-10);
		}
	}
	{ // present_value
		double u_[] = { 0, 1, 2, 3, 4};
		double d_[] = { 0,
						discount(u_[1], t.size(), t.data(), f.data(), 0.2),
						discount(u_[2], t.size(), t.data(), f.data(), 0.2),
						discount(u_[3], t.size(), t.data(), f.data(), 0.2),
						discount(u_[4], t.size(), t.data(), f.data(), 0.2)
		};
		double c_[] = { 0, 1, 2, 3, 4 };

		//assert(isnan(present_value(1, u_, c_, t.size(), t.data(), f.data())));
		//assert(isnan(present_value(1, u_, c_, t.size(), t.data(), f.data(), 0.2)));

		double sum = 0;
		for (int i = 0; i < 5; i++) {
			sum += c_[i] * d_[i];
			if (i == 4) {
				double tmp = present_value<double, double>(i + 1, u_, c_, t.size(), t.data(), f.data(), 0.2);
				assert(tmp == tmp);
				assert(fabs(sum - present_value(i + 1, u_, c_, t.size(), t.data(), f.data(), 0.2)) < 1e-10);
				assert(isnan(present_value(i + 1, u_, c_, t.size(), t.data(), f.data())));
			}
			else {
				double tmp = present_value<double, double>(i + 1, u_, c_, t.size(), t.data(), f.data(), 0.2);
				assert(tmp == tmp);
				assert(fabs(sum - present_value(i + 1, u_, c_, t.size(), t.data(), f.data(), 0.2)) < 1e-10);
				assert(fabs(sum - present_value(i + 1, u_, c_, t.size(), t.data(), f.data())) < 1e-10);
			}
		}
		
	}
}

#endif // _DEBUG