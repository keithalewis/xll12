// correlation.h - correlation matrices
#pragma once
#include <vector>

namespace fms {
    
    template<class X = double>
    class correlation {
        std::vector<std::vector<X>> rho_;
    public:
        enum layout {
            lower,  // rho_10, 0, ...; rho_20, rho_21, 0 ...;
            packed, // rho_10; rho_20, rho_21; ...
        };
        correlation()
        { }
        correlation(size_t n, const X* rho, layout type = packed)
            : rho_(n)
        {
            if(n == 0)
                return;

            rho_[0] = std::vector<X>{X(1)};

            for (size_t i = 1; i < n; ++i) {
                rho_[i] = std::vector<X>(i+1);
                X rho2 = X(0);
                for (size_t j = 0; j < i; ++j) {
                    size_t off = (type == lower ? (i - 1)*(n - 1) : (i*(i-1))/2);
                    X rij = rho[off + j];
                    rho_[i][j] = rij;
                    rho2 += rij * rij;
                }
                if(rho2 > 1) {
                    throw std::runtime_error("rho not a unit vector");
                }
                rho_[i][i] = sqrt(1 - rho2);
            }
        }
        correlation(const correlation&) = default;
        correlation& operator=(const correlation&) = default;
        ~correlation()
        { }

        size_t size() const
        {
            return rho_.size();
        }

        X operator()(size_t i, size_t j) const
        {
            if (i > j)
                std::swap(i, j);

            return rho_[i][j];
        }

        // correlation
        X rho(size_t i, size_t j) const
        {
            //ensure(i < n && j < n);

            if (i > j) {
                std::swap(i, j);
            }

            X r{0};
            const auto& rho_i = rho_[i];
            const auto& rho_j = rho_[j];
            for (size_t k = 0; k <= i; ++k) {
                r += rho_i[k]*rho_j[k];
            }

            return r;
        }
    };
}
