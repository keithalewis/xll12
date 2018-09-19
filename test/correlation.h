// correlation.h - correlaton matrices
#pragma once
#include <vector>

namespace fms {
    
    template<class X = double>
    class correlation {
        std::vector<X> rho; // (n-1)*n/2 array of unit vectors minus last index
    public:
        enum layout {lower_triangular, compressed_rows};
        correlation(size_t n, const X* rho, layout type)
            : rho(n*n)
        {
            if (type == lower_trangular) {
                rho[0] = X(1);
                for (size_t i = 0; i < n - 1; ++i) {
                    X rho2 = X(0);
                    for (size_t j = 0; j < n - 1; ++j) {
                        rho[n*i + j] = 
                    }
                }
            }
            else {

            }
        }
        ~correlation()
        { }
        X rho(size_t i, size_t j) const
        {
            ensure(i < n && j < n);
            if (i == j) {
                return 1;
            }
            if (i > j) {
                std::swap(i, j);
            }
            if (i == 0) {
                return operator()(0, j);
            }
        }
        X operator()(size_t i, size_t j) const
        {
            if (i == j) {
                return 1;
            }
            if (i > j) {
                std::swap(i, j);
            }
            if (i == 0) {
                return 1;
            }
        }
    };
}
