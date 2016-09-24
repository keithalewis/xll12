#include "black.h"
#include "../xll/xll.h"

using namespace xll;
using namespace black;

AddIn xai_normal_cdf(
	Function(XLL_DOUBLE, L"?xll_normal_cdf", L"NORMAL.CDF")
	.Arg(XLL_DOUBLE, L"x", L"is a number..")
	.Category(L"PROB")
);
double WINAPI xll_normal_cdf(double x)
{
#pragma XLLEXPORT
	return prob::normal_cdf(x);
}

TEST_BEGIN(normal_cdf)

// Compare normal_cdf to Excel function NORMSDIST.
for (double x = -6; x <= 6; x += 0.1) {
	double Nx  = xll_normal_cdf(x);
	double Nx_ = Excel(xlfNormsdist, OPER(x));
	ensure (fabs(Nx - Nx_) < std::numeric_limits<double>::epsilon());
} 

TEST_END


//!!! Create an addin function called BLACK.PUT that calls black::put.
//AddIn xai_black_put(...

//TEST_BEGIN(black_put)

//!!! Write a test for xll_black_put using formula (27) of 
// http://kalx.net/BlaSch1973.pdf with values
// f = x = 100, s = v = 0.2, k = c = 100, t = t* = 0.25, and r = 0.  

//TEST_END