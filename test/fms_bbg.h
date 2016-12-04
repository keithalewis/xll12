// fms_bbg.h - Bloomberg specific functions
#pragma once
#include <map>
#include <memory>
#include <regex>
#include <string>
#include "xll_instrument.h"

namespace fms {

	// current year
	inline int Year()
	{
		static int year = static_cast<int>(xll::Excel(xlfYear, xll::Excel(xlfToday)));

		return year;
	}

	namespace cmegroup {

		constexpr int call_month_code_table[] = { 0,
			'A', 'B', 'C',
			'D', 'E', 'F',
			'G', 'H', 'I',
			'J', 'K', 'L'
		};
		constexpr int put_month_code_table[] = { 0,
			'M', 'N', 'O',
			'P', 'Q', 'R',
			'S', 'T', 'U',
			'V', 'W', 'X'
		};
		constexpr int futures_month_code_table[] = { 0,
			'F', 'G', 'H',
			'J', 'K', 'M',
			'N', 'Q', 'U',
			'V', 'X', 'Z'
		};
		template<class T, size_t N>
		inline int find(const T(&table)[N], const T& t)
		{
			return static_cast<int>(std::find(table, table + N, t) - table);
		}
	} // namespace cmegroup
namespace bbg {

	// return instrument based on Bloomberg ticker
	std::unique_ptr<xll::fp_instrument> parse_ticker(const wchar_t* ticker, double quote, xll::excel_date valuation)
	{
		using instrument = xll::fp_instrument;
		using cd = xll::cash_deposit<xll::months>;
		using fra = xll::forward_rate_agreement<xll::months>;
		using swap = xll::interest_rate_swap;
		
		static std::basic_regex<wchar_t> us0(L"US0+([1-9]+)M");
		if (std::regex_search<wchar_t>(ticker, us0)) {
			std::wcmatch match;
			std::regex_match(ticker, match, us0);
			auto m = stoi(match[1].str());
			
			std::unique_ptr<xll::fp_instrument> i(new xll::cash_deposit<xll::months>(m, quote/100, xll::DCB_ACTUAL_360));
			i->fix(valuation, valuation);

			return i;
		}
		
		std::basic_regex<wchar_t> ed(L"ED([A-Z])([0-9])");
		if (std::regex_search<wchar_t>(ticker, ed)) {
			std::wcmatch match;
			std::regex_match(ticker, match, ed);
			// month from month code
			auto month = cmegroup::find(cmegroup::futures_month_code_table, (int)match[1].str()[0]);
			// year from current year and last year digit
			int year = Year();
			int y = year%10;
			int d = match[2].str()[0] - '0';
			year = (year - y) + d + (d < y);

			std::unique_ptr<xll::fp_instrument> i(new xll::forward_rate_agreement<xll::months>(3, 1 - quote/100, xll::DCB_ACTUAL_360));
			xll::excel_date fom = xll::date(year, month, 1);
			// third wednesday of expiration month
			xll::excel_date effective = xll::date_nth_day_of_week(fom, 2, xll::DOW_WED);
			i->fix(valuation, effective);

			return i;
		}

		std::basic_regex<wchar_t> ussw(L"USSW(AP)?([0-9]+)");
		if (std::regex_search<wchar_t>(ticker, ussw)) {
			std::wcmatch match;
			std::regex_match(ticker, match, us0);
			auto y = stoi(match[1].str());

			std::unique_ptr<xll::fp_instrument> i(new xll::interest_rate_swap(y, quote/100, xll::FREQ_SEMIANNUAL, xll::DCB_ACTUAL_360));
			i->fix(valuation, valuation);

			return i;
		}

		return nullptr;
	}

} // namespace bbg
} // namespace fms
