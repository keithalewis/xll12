// correlation.cpp
#include "correlation.h"
#include "../xll/xll.h"

using namespace xll;
using namespace fms;

XLL_ENUM_DOC(fms::correlation<>::layout::lower, CORRELATION_LOWER, L"XLL", 
    L"Lower rectangular correlation matrix", L"DOC");
XLL_ENUM_DOC(fms::correlation<>::layout::packed, CORRELATION_PACKED, L"XLL", 
    L"Packed correlation matrix", L"DOC");

AddIn xai_correlation(
    Function(XLL_HANDLE, L"?xll_correlation", L"CORRELATION")
    .Arg(XLL_FP, L"rho", L"is an array of vectors without their last component.")
    .Arg(XLL_WORD, L"layout", L"is either CORRELATION_LOWER(), or CORRELATION_PACKED()")
    .Category(L"XLL")
    .FunctionHelp(L"Return a handle to a correlation object.")
    .Uncalced()
    .Documentation(LR"(
        Create a handle to a correlation object using an array of vectors.
    )")
);
HANDLEX WINAPI xll_correlation(const _FP12* prho, fms::correlation<>::layout type)
{
#pragma XLLEXPORT
    handlex result;

    try {
        using corr = fms::correlation<double>;
        ensure(prho->rows == prho->columns);
        handle<corr> h(new corr(prho->rows + 1, begin(*prho), type));
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
    .Documentation(LR"(Every correlation matrix correponds to a set of unit vectors.)")
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

#ifdef _DEBUG

#pragma warning(disable: 4456) // declaration of 'corr' hides previous local declaration)
xll::test test_correlation([]() {
    double eps = std::numeric_limits<double>::epsilon();
    //using fms::correlation<double>::packed();
    {
        fms::correlation<> corr;
    }
    {
        fms::correlation<> corr(1, nullptr);
        ensure (corr.size() == 1);
        ensure (corr.rho(0, 0) == 1);
    }
    {
        double rho = 0.5;
        fms::correlation<> corr(2, &rho, fms::correlation<>::packed);
        ensure (corr.size() == 2);
        ensure (corr.rho(0, 0) == 1);
        ensure (corr.rho(0, 1) == 0.5);
        ensure (corr.rho(1, 0) == 0.5);
        ensure (fabs(corr.rho(1, 1) - 1) <= eps);
    }
    {
        double rho[] = {0.5,  0.4, 0.3};
        fms::correlation<> corr(3, rho);
        ensure (corr.size() == 3);
        ensure (corr.rho(0, 0) == 1);
        ensure (corr.rho(0, 1) == 0.5);
        ensure (corr.rho(0, 2) == 0.4);
        ensure (corr.rho(1, 0) == 0.5);
        ensure (fabs(corr.rho(1, 1) - 1) <= eps);
        ensure (corr.rho(1, 2) == 0.5*0.4 + sqrt(1-0.5*0.5)*0.3);
        ensure (corr.rho(2, 0) == 0.4);
        ensure (corr.rho(2, 1) == 0.5*0.4 + sqrt(1-0.5*0.5)*0.3);
        ensure (fabs(corr.rho(2, 2) - 1) <= eps);
    }
    {
        double rho[] = {0.5, 0, 0.4, 0.3};
        fms::correlation<> corr(3, rho, fms::correlation<>::lower);
        ensure (corr.size() == 3);
        ensure (corr.rho(0, 0) == 1);
        ensure (corr.rho(0, 1) == 0.5);
        ensure (corr.rho(0, 2) == 0.4);
        ensure (corr.rho(1, 0) == 0.5);
        ensure (fabs(corr.rho(1, 1) - 1) <= eps);
        ensure (corr.rho(1, 2) == 0.5*0.4 + sqrt(1-0.5*0.5)*0.3);
        ensure (corr.rho(2, 0) == 0.4);
        ensure (corr.rho(2, 1) == 0.5*0.4 + sqrt(1-0.5*0.5)*0.3);
        ensure (fabs(corr.rho(2, 2) - 1) <= eps);
    }
});

#endif // _DEBUG