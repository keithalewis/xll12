// fms_correlation.h - correlation matrix
#pragma once
#include <vector>

namespace fms {
namespace correlation {

	// An n x n correlation matrix is determined by n unit vectors, u_i.
	// The i,j entry is the dot product rho_i,j = u_i . u_j.
	template<class X>
	class matrix {
		std::vector<X> u;
	public:
		matrix(size_t n)
			: u((n*(n+1))/2)
		{
			for (size_t i = 0; i < n; ++i)
				operator()(i,i) = X{1};
		}
		const X& operator()(size_t i, size_t j) const
		{

		}
	};

} // namespace correlation
} // namespace fms
