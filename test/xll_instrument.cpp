// xll_instrument.cpp - fixed income instruments
#include "xll_instrument.h"

using namespace xll;

#ifdef _DEBUG
TEST_BEGIN(fp_instument)

zero_coupon_bond<years> zcb(2);
ensure (zcb.size() == 0);
zcb.fix(0,0);
ensure (zcb.size() == 1);
ensure (fabs(zcb.time()[0] - 2) < 1./365);
ensure (zcb.cash()[0] == 1);

//!!! Understand these tests
interest_rate_swap irs(10, 0.01, FREQ_SEMIANNUAL, DCB_US30_360);
ensure (irs.size() == 0);
excel_date val = Excel(xlfToday);
excel_date eff = date_add(val, days{2}); 
irs.fix(val, eff);
ensure (irs.size() == 21);
ensure (irs.time(20) == (date_add(eff, years{10}) - val)/DAYS_PER_YEAR);
ensure (irs.cash(0) == -1);
ensure (irs.cash(1) == 0.01/2);
ensure (irs.cash(19) == 0.01/2);
ensure (irs.cash(20) == 1 + 0.01/2);

//!!! L* Add tests for cash_deposit

//!!! [M-S]* Add tests for forward_rate_agreement

TEST_END
#endif // _DEBUG

static AddIn xai_instrument_fix(
	Function(XLL_HANDLE, L"?xll_instrument_fix", L"INSTRUMENT.FIX")
	.Arg(XLL_HANDLE, L"instrument", L"is a handle to an instrument.")
	.Arg(XLL_DOUBLE, L"valuation", L"is the date on which prices are quoted.")
	.Arg(XLL_DOUBLE, L"effective", L"is the date of the first cash flow or calculation period..")
	.Category(L"XLL")
	.FunctionHelp(L"Return handle to instrument with time and cash flows populated.")
);
HANDLEX WINAPI xll_instrument_fix(HANDLEX h, double val, double eff)
{
#pragma XLLEXPORT
	try {
		handle<fp_instrument> h_(h);

		if (h_)
			h_->fix(val, eff);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddIn xai_instrument_time(
	Function(XLL_FP, L"?xll_instrument_time", L"INSTRUMENT.TIME")
	.Arg(XLL_HANDLE, L"instrument", L"is a handle to an instrument..")
	.Category(L"XLL")
	.FunctionHelp(L"Return cash flow times of an intrument.")
);
const _FP12* WINAPI xll_instrument_time(HANDLEX h)
{
#pragma XLLEXPORT
	const _FP12* pc = 0;

	try {
		handle<fp_instrument> h_(h);

		if (h_) {
			pc = h_->fp_time();
			ensure (0 != pc);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pc;
}

static AddIn xai_instrument_cash(
	Function(XLL_FP, L"?xll_instrument_cash", L"INSTRUMENT.CASH")
	.Arg(XLL_HANDLE, L"instrument", L"is a handle to an instrument..")
	.Category(L"XLL")
	.FunctionHelp(L"Return cash flow amounts of an intrument.")
);
const _FP12* WINAPI xll_instrument_cash(HANDLEX h)
{
#pragma XLLEXPORT
	const _FP12* pc = 0;

	try {
		handle<fp_instrument> h_(h);

		if (h_) {
			pc = h_->fp_cash();
			ensure (0 != pc);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pc;
}

static AddIn xai_instrument_cash_deposit(
	Function(XLL_HANDLE, L"?xll_instrument_cash_deposit", L"INSTRUMENT.CASH.DEPOSIT")
	.Arg(XLL_LONG, L"tenor", L"is the tenor of the CD in months.")
	.Arg(XLL_DOUBLE, L"coupon", L"is the coupon")
	.Arg(XLL_LONG, L"dcb", L"is the fixed leg day count basis from the XLL_DCB_* enumeration..")
	.Uncalced()
	.Category(L"XLL")
	.FunctionHelp(L"Return a handle to a cash deposit.")
);
HANDLEX WINAPI xll_instrument_cash_deposit(LONG tenor, double coupon, day_count_basis dcb)
{
#pragma XLLEXPORT
	handlex h;

	try{
		handle<fp_instrument> h_(new cash_deposit<months>(tenor, coupon, dcb));
		ensure (h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddIn xai_instrument_forward_rate_agreement(
	Function(XLL_HANDLE, L"?xll_instrument_forward_rate_agreement", L"INSTRUMENT.FORWARD.RATE.AGREEMENT")
	.Arg(XLL_LONG, L"tenor", L"is the tenor of the FRA in months.")
	.Arg(XLL_DOUBLE, L"coupon", L"is the coupon")
	.Arg(XLL_LONG, L"dcb", L"is the fixed leg day count basis from the XLL_DCB_* enumeration..")
	.Uncalced()
	.Category(L"XLL")
	.FunctionHelp(L"Return a handle to a forward rate agreement.")
);
HANDLEX WINAPI xll_instrument_forward_rate_agreement(LONG tenor, double coupon, day_count_basis dcb)
{
#pragma XLLEXPORT
	handlex h;

	try{
		handle<fp_instrument> h_(new forward_rate_agreement<months>(tenor, coupon, dcb));
		ensure (h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddIn xai_instrument_interest_rate_swap(
	Function(XLL_HANDLE, L"?xll_instrument_interest_rate_swap", L"INSTRUMENT.INTEREST.RATE.SWAP")
	.Arg(XLL_LONG, L"tenor", L"is the tenor of the swap in years.")
	.Arg(XLL_DOUBLE, L"coupon", L"is the swap fixed coupon")
	.Arg(XLL_LONG, L"frequency", L"is the number of payments per year.")
	.Arg(XLL_LONG, L"dcb", L"is the fixed leg day count basis from the XLL_DCB_* enumeration..")
	.Uncalced()
	.Category(L"XLL")
	.FunctionHelp(L"Return a handle to an interest rate swap.")
);
HANDLEX WINAPI xll_instrument_interest_rate_swap(LONG tenor, double coupon, frequency freq, day_count_basis dcb)
{
#pragma XLLEXPORT
	handlex h;

	try{
		handle<fp_instrument> h_(new interest_rate_swap(tenor, coupon, freq, dcb));
		ensure (h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}