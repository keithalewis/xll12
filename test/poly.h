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

	// C(n,k) = n!/(k! (n - k)!)
	// n/1 * (n-1)/2 * ... * (n - k + 1)/k
	inline long long C(int n, int k) {
		long long Cnk = 1;
		for (int k_ = 1; k_ <= k; ++k_, --n) {
			Cnk *= n;
			Cnk /= k_;
		}
		return Cnk;
	};

	// B_{n+1)(x_1,..., x_{n+1}) = 
	//   sum_{k=0}^n C(n,k) B_{n-k}(x_1,...,x_{n-k}) x_{k+1}
	// so
	// B_n(x_1,..., x_n) = 
	//   sum_{k=0}^{n-1} C(n-1,k) B_{n-1-k}(x_1,...,x_{n-1-k}) x_{k+1}
	// Assumes x_i = x[i-1] and x has n values.
	inline double Bell2(int n, const double* x)
	{
		if (n == 0)
			return 1;

		double Bn = 0;
		for (int k = 0; k < n; ++k) {
			Bn += C(n - 1, k)*Bell2(n - 1 - k, x)*x[k];
		}

		return Bn;
	}

	inline double Bell(size_t n, const double* x)
	{
		static size_t n_ = 0;
		static const double* x_ = 0;
		static std::vector<double> B{1};

		//!!! [A-C]* if x and x_ agree up to k, reuse B[1], ... B[k]
		// and discard B[n] for n > k.

		// Remember previous values
		n_ = n;
		x_ = x;
		
		if (n < B.size())
			return B[n];

		double Bn = 0;
		for (size_t k = 0; k < n; ++k) {
			Bn += C(n - 1, k)*Bell(n - 1 - k, x)*x[k];
		}

		if (n >= B.size()) {
			B.resize(n+1);
			B[n] = Bn;
		}

		return Bn;
	}
} // namespace poly
