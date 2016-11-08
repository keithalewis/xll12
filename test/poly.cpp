// poly.cpp - Polynomial functions
#include "poly.h"
#include "../xll/xll.h"

using namespace xll;

AddIn xai_Hermite(
	Function(XLL_DOUBLE, L"?xll_Hermite", L"HERMITE")
	.Arg(XLL_LONG, L"n", L"is the order of the polynomial..")
	.Arg(XLL_DOUBLE, L"x", L"is a number..")
	.Category(L"POLY")
);
double WINAPI xll_Hermite(long n, double x)
{
#pragma XLLEXPORT
	return poly::Hermite(n,x);
}

AddIn xai_Bell(
	Function(XLL_DOUBLE, L"?xll_Bell", L"BELL")
	.Arg(XLL_FP, L"kappa", L"is the vector of cumulants..")
	.Category(L"POLY")
);
double WINAPI xll_Bell(_FP12* kappa)
{
#pragma XLLEXPORT
	int n = kappa->rows*kappa->columns;

	return poly::Bell(n, kappa->array);
}

TEST_BEGIN(hermite)

// Calculate H_3(x) by hand and check at x = -1, 0, 2.
auto eps = std::numeric_limits<double>::epsilon();
auto H3 = [](double x) { return x*x*x - 3*x; };
double x;
x = -1;
ensure (fabs(H3(x) - poly::Hermite(3, x)) < 2*eps);
x = 0;
ensure (fabs(H3(x) - poly::Hermite(3, x)) < 2*eps);
x = 2;
ensure (fabs(H3(x) - poly::Hermite(3, x)) < 2*eps);

TEST_END

TEST_BEGIN(Bell)

// Calculate B_3(x_1, x_2, x_3) and check at x_1 = -1, x_2 = 0, x_3 = 2.
auto B3 = [](double x1, double x2, double x3) { return x1*x1*x1 + 3*x1*x2 + x3; };
double x[] = {-1, 0, 2};
double x1 = x[0], x2 = x[1], x3 = x[2];
ensure (B3(x1, x2, x3) == poly::Bell(3, x));

//!!! [G-K]* Test memoization.
//double x1 = x[0], x2 = x[1], x3 = x[2] = 4;
//double x1 = x[0], x2 = x[1] = 5, x3 = x[2];
//double x1 = x[0] = 6, x2 = x[1], x3 = x[2];


TEST_END
