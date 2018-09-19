// correlation.h - correlation matrices
#pragma once
#include <vector>

namespace fms {
    
    template<class X = double>
    class correlation {
        size_t n;
        std::vector<X> rho_;
    public:
        // layouts do not include the rho_ii values
        enum layout { 
            lower_triangular, // lower triangular without the diagonal
            compressed_rows   // rho_10, rho_20, rho_21, rho_30, rho_31, rho_32, ...
        };
        correlation()
        { }
        correlation(size_t n, const X* rho, layout type)
            : n(n), rho_(n*n)
        {
            rho_[0] = X(1);
            for (size_t i = 1; i < n; ++i) {
                X rho2 = X(0);
                for (size_t j = 0; j < i; ++j) {
                    size_t off = j + (type == lower_triangular) ? (i - 1)*n : (i - 1)*i / 2;
                    X rij = rho[off];
                    rho_[n*i + j] = rij;
                    rho2 += rij * rij;
                }
                rho_[n*i + i] = sqrt(rho2);
            }
        }
        correlation(const correlation&) = default;
        correlation& operator=(const correlation&) = default;
        ~correlation()
        { }

        X operator()(size_t i, size_t j) const
        {
            return rho_[n*i + j];
        }

        // correlation
        X rho(size_t i, size_t j) const
        {
            //ensure(i < n && j < n);
            if (i == j) {
                return 1;
            }

            if (i > j) {
                std::swap(i, j);
            }

            X r{ 0 };
            for (size_t k = 0; k <= i; ++k) {
                r += operator()(i, k)*operator()(j, k);
            }

            return r;
        }
    };
}
