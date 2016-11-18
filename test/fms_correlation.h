// fms_correlation.h - correlation matrix
#pragma once
#ifndef ensure
#include <cassert>
#define ensure assert
#endif
#include <vector>

namespace fms {
namespace correlation {

	// An n x n correlation matrix is determined by n unit vectors, u_i.
	// The i,j entry is the dot product rho_i,j = u_i . u_j.
	// Setting rho_i,j = rho will rotate u_j towards u_i.
	template<class X = double>
	class matrix {
		struct proxy {
			matrix& u;
			size_t i, j;
			proxy(matrix& u, size_t i, size_t j)
				: u(u), i(i), j(j)
			{ }
			operator const X&() const
			{
				return u[i][j];
			}
			// rho(i,j) = .1;
			// rho.operator()(i,j) = .1;
			// proxy(u,i,j) = .1;
			// proxy(u,i,j).operator=(.1);
			void operator=(const X& rho)
			{
				// rotate u_j to u'_j in the direction of u_i
				//
				// u'_j = a u_j + b u_i
				//
				// 1 = a^2 + 2 u_j . u_i ab + b^2
				// rho = u'_j . u_i = (u_j . u_i) a + b
				//
				// 1 = a^2 + 2 rij a(rho - rij a) + (rho - rij a)^2
				// 1 = a^2 + 2 rij rho a - rij^2 a^2 + rho^2 - 2 rho rij a + rij^2 a^2
				// 1 = a^2 + rho^2
				// a = sqrt(1 - rho^2)
				// b = rho
				ensure (-1 <= rho && rho <= 1);

				X a = sqrt(1 - rho*rho);
				for (size_t k = 0; k < i; ++k)
					u[j][k] = a*u[j][k] + rho*u[i][k];
				for (size_t k = i; k < j; ++k)
					u[j][k] = a*u[j][k];
			}
		};
		size_t n;
		std::vector<X> u;
	public:
		matrix(size_t n)
			: n(n), u((n*(n+1))/2)
		{
			// set diagonal to 1
			for (size_t i = 0; i < n; ++i)
				(*this)[i][i] = X{1};
		}
		// address of u_i
		X* operator[](size_t i)
		{
			ensure (i < n);

			return &u[(i*(i+1))/2];
		}
		const X* operator[](size_t i) const
		{
			return &u[(i*(i+1))/2];//!!! operator[](i);
		}
		const X& operator()(size_t i, size_t j) const
		{
			ensure (i < n);
			ensure (j < n);
			if (i > j)
				std::swap(i,j);

			return j <= i ? u[i][j] : 0;
		}
		proxy operator()(size_t i, size_t j)
		{
			ensure (i < n);
			ensure (j < n);
			if (i > j)
				std::swap(i,j);

			return matrix<X>::proxy(*this, i, j);
		}
	};

} // namespace correlation
} // namespace fms

#ifdef _DEBUG

inline void test_fms_correlation()
{
	using fms::correlation::matrix;

	{
		matrix<> rho(2);
		ensure (rho(0,0) == 1);
		ensure (rho(0,1) == 0);
		ensure (rho(1,0) == 0);
		ensure (rho(1,1) == 1);
		/*
		double r;
		r = 1;
		rho(0,1) = r;
		ensure (rho(0,0) == 1);
		ensure (rho(0,1) == r);
		ensure (rho(1,0) == r);
		ensure (rho(1,1) == 1);
		*/
	}
}

#endif // _DEBUG