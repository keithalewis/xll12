// poly.h - Polynomials
#pragma once
#include <vector>

namespace poly {

	// H_{n+1}(x) = x H_n(x) - n H_{n-1}(x)
	// so
	// H_n(x) = x H_{n-1}(x) - (n-1} H_{n-2}(x)
	inline auto Hermite(int n, double x)
	{
		if (n == 0)
			return 1.;

		if (n == 1)
			return x;

		return x*Hermite(n - 1, x) - (n-1)*Hermite(n - 2, x);
	}

	// B_{n+1)(x_1,..., x_{n+1}) = 
	//   sum_{k=0}^n C(n,k) B_{n-k}(x_1,...,x_{n-k}) x_{k+1}
	// so
	// B_n(x_1,..., x_n) = 
	//   sum_{k=0}^{n-1} C(n-1,k) B_{n-1-k}(x_1,...,x_{n-1-k}) x_{k+1}
	// Assumes x_i = x[i-1] and x has n values.
	inline double Bell(int n, const double* x)
	{
			// C(n,k) = n!/(k! (n - k)!)
			// n/1 * (n-1)/2 * ... * (n - k + 1)/k
		auto C = [](int n, int k) {
			long Cnk = 1;
			for (int k_ = 1; k_ <= k; ++k_, --n) {
				Cnk *= n;
				Cnk /= k_;
			}
			return Cnk;
		};

		if (n == 0)
			return 1;

		double Bn = 0;
		for (int k = 0; k < n; ++k) {
			Bn += C(n - 1, k)*Bell(n - 1 - k, x)*x[k];
		}

		return Bn;
	}

} // namespace poly
