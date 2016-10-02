// xll_math.h - wrappers for gsl_math.h to make value-type functions usable in the GSL
#pragma once
#include <functional>
#include "gsl/gsl_math.h"

namespace gsl {

	class function {
		// the GSL struct
		gsl_function _f;

		// the corresponding C++ object is the void* parameter
		std::function<double(double)> f;

		// the function pointer needed by GSL
		static double static_function(double x, void* params)
		{
			const std::function<double(double)>& f = *static_cast<std::function<double(double)>*>(params);

			return f(x);
		}
	public:
		function()
		{ }
		function(const std::function<double(double)>& f_)
			: f(f_)
		{
			_f.function = static_function;
			_f.params = &f;
		}
		function(const function& F)
			: f(F.f)
		{
			_f.function = static_function;
			_f.params = &f;
		}
		function& operator=(const function& F)
		{
			if (this != &F) {
				f = F.f;
				_f.function = static_function;
				_f.params = &f;
			}

			return *this;
		}
		~function()
		{ }

		// for GSL interoperability
		gsl_function* operator&()
		{
			return &_f;
		}
		double operator()(double x) const
		{
			return f(x);
		}
		double call(double x)
		{
			return GSL_FN_EVAL(&_f, x);
		}
	};

	class function_fdf {
		// function and its derivative
		using function = std::function<double(double)>;
		using fdfunction = std::tuple<function,function>;

		gsl_function_fdf _fdf;
		fdfunction fdf;

		// provide pointers for gsl_function_fdf
		static double static_f(double x, void* params)
		{
			const fdfunction& f = *static_cast<fdfunction*>(params);

			return std::get<0>(f)(x);
		}
		static double static_df(double x, void* params)
		{
			const fdfunction& f = *static_cast<fdfunction*>(params);

			return std::get<1>(f)(x);
		}
		static void static_fdf(double x, void* params, double* fx, double* dfx)
		{
			const fdfunction& f = *static_cast<fdfunction*>(params);

			*fx = std::get<0>(f)(x);
			*dfx = std::get<1>(f)(x);
		}
	public:
		function_fdf()
		{
			_fdf.f = static_f;
			_fdf.df = static_df;
			_fdf.fdf = static_fdf;
		}
		function_fdf(const fdfunction& F)
			: function_fdf()
		{
			fdf = F;
			_fdf.params = &fdf;
		}
		function_fdf(const function_fdf& F)
			: function_fdf(F.fdf)
		{ }
		function_fdf(const function& f_, const function& df_)
			: function_fdf(std::make_tuple(f_, df_))
		{ }
		function_fdf& operator=(const function_fdf& F)
		{
			if (this != &F) {
				fdf = F.fdf;
				_fdf.params = &fdf;
			}

			return *this;
		}
		~function_fdf()
		{ }

		gsl_function_fdf* operator&()
		{
			return &_fdf;
		}
		void call(double x, double& f, double& df)
		{
			GSL_FN_FDF_EVAL_F_DF(&_fdf, x, &f, &df);
		}
	};

}

#ifdef _DEBUG
#include <cassert>
#include <vector>
#include "gsl/gsl_roots.h"

inline void test_xll_math()
{
	{
		gsl::function f;
		gsl::function g([](double x) { return x; });
		assert (g(1) == 1);
		f = g;
		assert (f(2) == 2);

		gsl_function* pf = &f;
		assert (3 == GSL_FN_EVAL(pf, 3));
		assert (4 == f.call(4));		
	}
	{
		std::vector<double> p{1,2,3};
		gsl::function f([p](double x) { return p[0] + x*(p[1] + x*p[2]); });
		assert (f(0) == 1);
		assert (f.call(0) == 1);

		gsl::function g(f);
		assert (g(0) == 1);
		assert (g.call(0) == 1);

		gsl::function h;
		h = f;
		assert (h(0) == 1);
		assert (h.call(0) == 1);
	}
	{
		auto f = [](double x) { return x*x; };
		auto df = [](double x) { return 2*x; };
		
		gsl::function_fdf g;
		gsl::function_fdf h(f, df);
		gsl::function_fdf k(std::make_tuple(f, df));
		g = k;

		double x = 1, y, dy;
		GSL_FN_FDF_EVAL_F_DF(&g, x, &y, &dy);
		assert (y == 1);
		assert (dy == 2);

		x = 2;
		y = GSL_FN_FDF_EVAL_F(&g, x);
		dy = GSL_FN_FDF_EVAL_DF(&g, x);
		assert (y == 4);
		assert (dy == 4);
	}
}

#endif // _DEBUG