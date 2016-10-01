// xll_roots.cpp - GSL 1-dim root finding
#include "xll_roots.h"
#include "xll_gsl.h"

using namespace xll;

using function = std::function<double(double)>;

XLL_ENUM_DOC(p2h<const gsl_root_fsolver_type>(gsl_root_fsolver_bisection),GSL_ROOT_FSOLVER_BISECTION, CATEGORY, _T("Bisection method solver"), _T("Documentation"));
XLL_ENUM_DOC(p2h<const gsl_root_fsolver_type>(gsl_root_fsolver_brent),GSL_ROOT_FSOLVER_BRENT, CATEGORY, _T("Brent method solver"), _T("Documentation"));
XLL_ENUM_DOC(p2h<const gsl_root_fsolver_type>(gsl_root_fsolver_falsepos),GSL_ROOT_FSOLVER_FALSEPOS, CATEGORY, _T("False position method solver"), _T("Documentation"));

static AddInX xai_root_fsolver(
	FunctionX(XLL_HANDLEX, _T("?xll_root_fsolver"), PREFIX _T("ROOT.FSOLVER"))
	.Arg(XLL_HANDLEX, _T("Type"), _T("is the type of solver from the GSL_ROOT_FSOLVER_* enumeration"))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a gsl::root::fsolver object."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_root_fsolver(HANDLEX type)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<gsl::root::fsolver> h_(new gsl::root::fsolver(h2p<gsl_root_fsolver_type>(type)));

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_root_fsolver_set(
	FunctionX(XLL_HANDLEX, _T("?xll_root_fsolver_set"), PREFIX _T("ROOT.FSOLVER.SET"))
	.Arg(XLL_HANDLEX, _T("Solver"), _T("is a handle returned by ") PREFIX _T("ROOT.FSOLVER"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle used by XLL.FUNCTION."))
	.Arg(XLL_DOUBLEX, _T("Lo"), _T("is the lower bound of the search interval."))
	.Arg(XLL_DOUBLEX, _T("Hi"), _T("is the upper bound of the search interval."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a gsl::root::fsolver object with function and bounds set."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_root_fsolver_set(HANDLEX h, HANDLEX f, double lo, double hi)
{
#pragma XLLEXPORT
	try {
		handle<gsl::root::fsolver> h_(h);
		handle<function> f_(f);

		h_->set(*f_, lo, hi);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_root_fsolver_iterate(
	FunctionX(XLL_HANDLEX, _T("?xll_root_fsolver_iterate"), PREFIX _T("ROOT.FSOLVER.ITERATE"))
	.Arg(XLL_HANDLEX, _T("Solver"), _T("is a handle returned by ") PREFIX _T("ROOT.FSOLVER"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a gsl::root::fsolver object after one iteration step."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_root_fsolver_iterate(HANDLEX h)
{
#pragma XLLEXPORT
	try {
		handle<gsl::root::fsolver> h_(h);

		ensure (GSL_SUCCESS == h_->iterate());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_root_fsolver_root(
	FunctionX(XLL_HANDLEX, _T("?xll_root_fsolver_root"), PREFIX _T("ROOT.FSOLVER.ROOT"))
	.Arg(XLL_HANDLEX, _T("Solver"), _T("is a handle returned by ") PREFIX _T("ROOT.FSOLVER"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the current root estimate of a gsl::root::fsolver object."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_root_fsolver_root(HANDLEX h)
{
#pragma XLLEXPORT
	handlex root;

	try {
		handle<gsl::root::fsolver> h_(h);

		root = h_->root();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return root;
}

static AddInX xai_root_fsolver_x_lower(
	FunctionX(XLL_HANDLEX, _T("?xll_root_fsolver_x_lower"), PREFIX _T("ROOT.FSOLVER.X.LOWER"))
	.Arg(XLL_HANDLEX, _T("Solver"), _T("is a handle returned by ") PREFIX _T("ROOT.FSOLVER"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the current lower root bound of a gsl::root::fsolver object."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_root_fsolver_x_lower(HANDLEX h)
{
#pragma XLLEXPORT
	handlex lo;

	try {
		handle<gsl::root::fsolver> h_(h);

		lo = h_->x_lower();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return lo;
}

static AddInX xai_root_fsolver_x_upper(
	FunctionX(XLL_HANDLEX, _T("?xll_root_fsolver_x_upper"), PREFIX _T("ROOT.FSOLVER.X.UPPER"))
	.Arg(XLL_HANDLEX, _T("Solver"), _T("is a handle returned by ") PREFIX _T("ROOT.FSOLVER"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the current upper root bound of a gsl::root::fsolver object."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_root_fsolver_x_upper(HANDLEX h)
{
#pragma XLLEXPORT
	handlex lo;

	try {
		handle<gsl::root::fsolver> h_(h);

		lo = h_->x_upper();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return lo;
}

XLL_ENUM_DOC(p2h<const gsl_root_fdfsolver_type>(gsl_root_fdfsolver_newton),GSL_ROOT_FDFSOLVER_NEWTON, CATEGORY, _T("Newton method solver"), _T("Documentation"));
XLL_ENUM_DOC(p2h<const gsl_root_fdfsolver_type>(gsl_root_fdfsolver_secant),GSL_ROOT_FDFSOLVER_SECANT, CATEGORY, _T("Secant method solver"), _T("Documentation"));
XLL_ENUM_DOC(p2h<const gsl_root_fdfsolver_type>(gsl_root_fdfsolver_steffenson),GSL_ROOT_FDFSOLVER_STEFFENSON, CATEGORY, _T("Steffenson position method solver"), _T("Documentation"));

static AddInX xai_root_fdfsolver(
	FunctionX(XLL_HANDLEX, _T("?xll_root_fdfsolver"), PREFIX _T("ROOT.FDFSOLVER"))
	.Arg(XLL_HANDLEX, _T("Type"), _T("is the type of solver from the GSL_ROOT_FDFSOLVER_* enumeration"))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a gsl::root::fdfsolver object."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_root_fdfsolver(HANDLEX type)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<gsl::root::fdfsolver> h_(new gsl::root::fdfsolver(h2p<gsl_root_fdfsolver_type>(type)));

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_root_fdfsolver_set(
	FunctionX(XLL_HANDLEX, _T("?xll_root_fdfsolver_set"), PREFIX _T("ROOT.FDFSOLVER.SET"))
	.Arg(XLL_HANDLEX, _T("Solver"), _T("is a handle returned by ") PREFIX _T("ROOT.FDFSOLVER"))
	.Arg(XLL_HANDLEX, _T("F"), _T("is a handle used by XLL.."))
	.Arg(XLL_HANDLEX, _T("dF"), _T("is a handle used by XLL.."))
	.Arg(XLL_DOUBLEX, _T("X0"), _T("is the initial root guess."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a gsl::root::fsolver object."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_root_fdfsolver_set(HANDLEX h, HANDLEX f, HANDLEX df, double x0)
{
#pragma XLLEXPORT
	try {
		handle<gsl::root::fdfsolver> h_(h);
		handle<function> f_(f);
		handle<function> df_(df);

		h_->set(*f_, *df_, x0);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_root_fdfsolver_iterate(
	FunctionX(XLL_HANDLEX, _T("?xll_root_fdfsolver_iterate"), PREFIX _T("ROOT.FDFSOLVER.ITERATE"))
	.Arg(XLL_HANDLEX, _T("Solver"), _T("is a handle returned by ") PREFIX _T("ROOT.FDFSOLVER"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a gsl::root::fdfsolver object after one iteration step."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_root_fdfsolver_iterate(HANDLEX h)
{
#pragma XLLEXPORT
	try {
		handle<gsl::root::fdfsolver> h_(h);

		ensure (GSL_SUCCESS == h_->iterate());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_root_fdfsolver_root(
	FunctionX(XLL_HANDLEX, _T("?xll_root_fdfsolver_root"), PREFIX _T("ROOT.FDFSOLVER.ROOT"))
	.Arg(XLL_HANDLEX, _T("Solver"), _T("is a handle returned by ") PREFIX _T("ROOT.FDFSOLVER"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the current root estimate of a gsl::root::fdfsolver object."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_root_fdfsolver_root(HANDLEX h)
{
#pragma XLLEXPORT
	handlex root;

	try {
		handle<gsl::root::fdfsolver> h_(h);

		root = h_->root();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return root;
}

#ifdef _DEBUG

XLL_TEST_BEGIN(xll_test_roots)

	test_xll_math();
	test_gsl_root_fsolver();
	test_gsl_root_fdfsolver();

XLL_TEST_END(xll_test_roots)

#endif // _DEBUG