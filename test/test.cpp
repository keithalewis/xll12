// test.cpp
#include "../xll/xll.h"

using namespace xll;

AddIn xai_modulo(Function(XLL_LONG, L"?xll_modulo", L"MODULO")
	.Arg(XLL_LONG, L"x")
	.Arg(XLL_LONG, L"y")
	.Category(L"Category")
	.FunctionHelp(L"Return x%y")
);
LONG WINAPI xll_modulo(LONG x, LONG y)
{
#pragma XLLEXPORT
	return x%y;
}

#include <vector>
AddIn xai_new(Function(XLL_FP, L"?xll_new", L"PTR"));
_FP12* WINAPI xll_new()
{
#pragma XLLEXPORT
	static char buf[sizeof(_FP12) + 64*sizeof(double)];
	static _FP12* p = (_FP12*)&buf;
	p->rows = 1;
	p->columns = 64;

	union {
		void* p;
		uint64_t u;
	} pu;
	pu.p = new std::vector<int>();

	for (int i = 0; i < 64; ++i) {
		p->array[i] = (int)((pu.u)&1);
		pu.u = pu.u >> 1;
	}
		
	return p;
}

AddIn xai_nan(
	Function(XLL_DOUBLE, L"?xll_nan", L"NAN")
);
double WINAPI xll_nan()
{
#pragma XLLEXPORT
	return std::numeric_limits<double>::quiet_NaN();
}
AddIn xai_isnan(
	Function(XLL_BOOL, L"?xll_isnan", L"ISNAN")
);
BOOL WINAPI xll_isnan(double x)
{
#pragma XLLEXPORT
	return 0 != _isnan(x);
}
AddIn xai_fp(
	Function(XLL_FP, L"?xll_fp", L"FP")
	.Arg(XLL_FP, L"fp", L"is an array.")
	.Arg(XLL_BOOL, L"NaN", L"put a NaN in 0,0..")
);
_FP12* WINAPI xll_fp(_FP12* pfp, BOOL b)
{
#pragma XLLEXPORT

	if (b)
		pfp->array[0] = std::numeric_limits<double>::quiet_NaN();

	return pfp;
}
