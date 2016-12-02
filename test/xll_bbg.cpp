// xll_bbg.cpp - Bloomberg specific tickers.
#include "../xll/xll.h"
#include "fms_bbg.h"

using namespace xll;
using namespace fms;

static AddIn xai_parse_ticker(
	Function(XLL_HANDLE, L"?xll_parse_ticker", L"PARSE.TICKER")
	.Arg(XLL_CSTRING, L"ticker", L"is the Bloomberg ticker.")
	.Arg(XLL_DOUBLE, L"quote", L"is the Bloomberg quote associated with the ticker.")
	.Arg(XLL_DOUBLE, L"valuation", L"is the valuation date of the quote.")
	.Uncalced()
	.Category(L"BBG")
	.FunctionHelp(L"Return handle to a instrument corresponding to a Bloomberg ticker.")
);
HANDLEX WINAPI xll_parse_ticker(const XCHAR* ticker, double quote, double valuation)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<fp_instrument> h_(bbg::parse_ticker(ticker, quote, valuation).get());
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

TEST_BEGIN(bbg)

excel_date today = Excel(xlfToday);
auto cd = bbg::parse_ticker(L"US0003M", 0.01, today);
ensure (cd->size() == 1);
//ensure (fabs(cd->time(0) - 0.25) < 1./365);
//ensure (cd->cash(0) - (1 + 0.01/4) < 0.001);

TEST_END