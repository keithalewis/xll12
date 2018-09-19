// correlation.cpp
#include "correlation.h"
#include "../xll/xll.h"

using namespace xll;
using namespace fms;
using layout = correlation<double>::layout;

AddIn xai_correlation(
    Function(XLL_HANDLE, L"?xll_correlation", L"CORRELATION")
    .Arg(XLL_FP, L"rho", L"is an array of init vectors without their last component.")
    .Arg(XLL_LONG, L"layout?", L"is an optional layout argument.")
    .Category(L"XLL")
    .FunctionHelp(L"Return a handle to a correlation object.")
    .Uncalced()
);
HANDLEX WINAPI xll_correlation(const _FP12* prho, layout type)
{
#pragma XLLEXPORT
    handlex result;

    try {
        ensure(prho->rows == prho->columns);
        type = fms::correlation<double>::layout::lower_triangular; // !!! for now
        handle<fms::correlation<double>> h(new fms::correlation<double>(prho->rows + 1, begin(*prho), type));
        result = h.get();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return result;
}

AddIn xai_correlation_rho(
    Function(XLL_DOUBLE, L"?xll_correlation_rho", L"CORRELATION.RHO")
    .Arg(XLL_HANDLE, L"rho", L"is a handle to a correlation object.")
    .Arg(XLL_WORD, L"i", L"is the first index of the correlation.")
    .Arg(XLL_WORD, L"j", L"is the second index of the correlation.")
    .Category(L"XLL")
    .FunctionHelp(L"Return the dot product of the i-th and j-th unit vector.")
);
double WINAPI xll_correlation_rho(HANDLEX h, WORD i, WORD j)
{
#pragma XLLEXPORT
    handlex result;

    try {
        handle<fms::correlation<double>> h_(h);
        ensure(h_);
        result = h_->rho(i, j);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return result;
}