// xll_roots.h - GSL 1-dim root finding
// http://www.gnu.org/software/gsl/manual/html_node/One-dimensional-Root_002dFinding.html#One-dimensional-Root_002dFinding
#pragma once
#include <functional>
#include <memory>
#include <stdexcept>
#include <tuple>
#include "gsl/gsl_errno.h"
#include "gsl/gsl_roots.h"
#include "xll_math.h"

namespace gsl {

namespace root {

	// root bracketing solvers
	class fsolver {
		gsl_root_fsolver* s;
		gsl::function f;
	public:
		explicit fsolver(const gsl_root_fsolver_type * type)
			: s{gsl_root_fsolver_alloc(type)}
		{ }
		fsolver(const fsolver&) = delete;
		fsolver& operator=(const fsolver&) = delete;
		~fsolver()
		{
			gsl_root_fsolver_free(s);
		}

		// needed for gsl_root_fsolver_* routines
		gsl_root_fsolver* get() const
		{
			return s;
		}
		// syntactic sugar
		operator gsl_root_fsolver*() const
		{
			return get();
		}

		int set(const std::function<double(double)>& f_, double lo, double hi)
		{
			f = f_;

			return gsl_root_fsolver_set(s, &f, lo, hi);
		}

		// forward to gsl_root_fsolver_* functions
		int iterate()
		{
			return gsl_root_fsolver_iterate(s);
		}
		double x_lower() const
		{
			return gsl_root_fsolver_x_lower(s);
		}
		double x_upper() const
		{
			return gsl_root_fsolver_x_upper(s);
		}
		double root() const
		{
			return gsl_root_fsolver_root(s);
		}

		// specify convergence condition
		double solve(const std::function<bool(const fsolver&)>& done)
		{
			while (GSL_SUCCESS == iterate()) {
				if (done(*this))
					break;
			}

			return root();
		}
	};

	// convergence helper functions
	inline auto test_interval(double epsabs, double epsrel)
	{
		return [epsabs,epsrel](const fsolver& s) {
			return GSL_SUCCESS == gsl_root_test_interval(s.x_lower(), s.x_upper(), epsabs, epsrel);
		};
	}

	// root finding using derivatives
	class fdfsolver {
		gsl_root_fdfsolver* s;

		// function and its derivative
		gsl::function_fdf FdF;
	public:
		fdfsolver(const gsl_root_fdfsolver_type * type)
			: s{gsl_root_fdfsolver_alloc(type)}
		{ }
		fdfsolver(const fdfsolver&) = delete;
		fdfsolver& operator=(const fdfsolver&) = delete;
		~fdfsolver()
		{
			gsl_root_fdfsolver_free(s);
		}
		// needed for gsl_root_fdfsolver_* routines
		gsl_root_fdfsolver* get() const
		{
			return s;
		}
		// syntactic sugar
		operator gsl_root_fdfsolver*() const
		{
			return s;
		}

		int set(const std::function<double(double)>& f, const std::function<double(double)>& df, double x0)
		{
			FdF = gsl::function_fdf(f, df);

			return gsl_root_fdfsolver_set(s, &FdF, x0);
		}

		// forward to gsl_root_fdfsolver_* functions
		int iterate()
		{
			return gsl_root_fdfsolver_iterate(s);
		}
		double root() const
		{
			return gsl_root_fdfsolver_root(s);
		}

		// specify convergence condition given previous root
		double solve(const std::function<bool(const fdfsolver&,double)>& done)
		{
			double x = root();
			while (GSL_SUCCESS == iterate()) {
				if (done(*this, x))
					break;
				x = root();
			}

			return root();
		}
	};

	// convergence helper functions
	inline auto test_delta(double epsabs, double epsrel)
	{
		return [epsabs,epsrel](const fdfsolver& s, double x) -> bool {
			return GSL_SUCCESS == gsl_root_test_delta(x, s.root(), epsabs, epsrel);
		};
	}

	} // root
} // gsl

#ifdef _DEBUG
#include <cassert>
#include <vector>
//#include <gsl/gsl_poly.h>

// http://www.gnu.org/software/gsl/manual/html_node/Root-Finding-Examples.html#Root-Finding-Examples

// a x^2 + b x + c
struct quadratic_params {
	double a, b, c;
};

// This is bad. Why???
inline double quadratic(double x, void *params)
{
	struct quadratic_params *p 
		= (struct quadratic_params *) params;

	double a = p->a;
	double b = p->b;
	double c = p->c;

	return (a * x + b) * x + c;
}

inline double quadratic_deriv(double x, void *params)
{
	struct quadratic_params *p 
		= (struct quadratic_params *) params;

	double a = p->a;
	double b = p->b;
//	double c = p->c;

	return 2.0 * a * x + b;
}

inline void quadratic_fdf(double x, void *params, double *y, double *dy)
{
	struct quadratic_params *p 
		= (struct quadratic_params *) params;

	double a = p->a;
	double b = p->b;
	double c = p->c;

	*y = (a * x + b) * x + c;
	*dy = 2.0 * a * x + b;
}

inline void test_gsl_root_fsolver()
{
	// GSL example
	{
		gsl_root_fsolver* s = gsl_root_fsolver_alloc(gsl_root_fsolver_brent);
		
		quadratic_params params{1.,0.,-5.}; // x^2 - 5
		gsl_function F;
		F.function = quadratic;
		F.params = &params;
		double x_lo = 0.0, x_hi = 5.0;
		double epsabs = 0, epsrel = 1e-5;

		gsl_root_fsolver_set(s, &F, x_lo, x_hi);
		while (GSL_SUCCESS == gsl_root_fsolver_iterate(s)) {
			x_lo = gsl_root_fsolver_x_lower(s);
			x_hi = gsl_root_fsolver_x_upper(s);

			// |x_lo - x_hi| < epsabs + epsrel * min(|x_lo|, |x_hi|)
			if (GSL_SUCCESS == gsl_root_test_interval(x_lo, x_hi, epsabs, epsrel))
				break;
		}
		double root = gsl_root_fsolver_root(s);
		double sqrt5 = sqrt(5.);
		assert (fabs(root - sqrt5) < sqrt5*epsrel);

		gsl_root_fsolver_free(s);
	}
	// root_fsolver class with function<double(double)>
	{
		gsl::root::fsolver s(gsl_root_fsolver_brent);

		std::vector<double> params{-5,0,1}; // -5 + x^2
		auto F = [params](double x) { return params[0] + x*(params[1] + x*params[2]); };
		gsl::function F_(F);
		assert (F_(0) == -5);
		assert (F_.call(0) == -5);
		double x_lo = 0.0, x_hi = 5.0, epsrel = 1e-6;
		s.set(F, x_lo, x_hi);
		
		double root = s.solve(gsl::root::test_interval(0, epsrel));
		double sqrt5 = sqrt(5.);
		assert (fabs(root - sqrt5) < sqrt5*epsrel);
	}
}
inline void test_gsl_root_fdfsolver()
{
	// root_fdfsolver class with function<tuple<double,double>(double)>
	{
		gsl::root::fdfsolver s(gsl_root_fdfsolver_newton);

		// F(x) = x^2 - 5
		auto F = [](double x) {
			return x*x - 5;
		};
		auto dF = [](double x) { 
			return 2*x;
		};
		double x = 5.0, epsrel = 1e-6;
		s.set(F, dF, x);

		double root = s.solve(gsl::root::test_delta(0, epsrel));
		double sqrt5 = sqrt(5.);
		assert (fabs(root - sqrt5) < sqrt5*epsrel);
	}
}

#endif // _DEBUG