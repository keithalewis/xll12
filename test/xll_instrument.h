// xll_instrument.h - fixed income instruments
#pragma once
#include "xll_date.h"

namespace xll {

	// Forward rate over [t, u] given discounts and day count fraction.
	template<class X = double>
	inline X forward(X Dt, X Du, X dcf)
	{
		return (Dt/Du - 1)/dcf;
	}

	// (Non-Virtal) Interface to all (fixed income) instruments
	// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-Virtual_Interface
	template<class T = double, class C = double>
	class instrument {
	public:
		virtual ~instrument()
		{ }
		// public interface
		size_t size() const 
		{
			return size_();
		}
		const T* time() const
		{
			return time_();
		}
		const C* cash() const
		{
			return cash_();
		}
		// Convenience functions.
		const T& time(size_t i) const
		{
			ensure (i < size());

			return time_()[i];
		}
		const C& cash(size_t i) const
		{
			ensure (i < size());

			return cash_()[i];
		}
		// Time of last cash flow.
		T maturity() const
		{
			auto n = size();

			return n ? time(n-1) : 0;
		}
	private:
		// subclass must override
		virtual size_t size_() const = 0;
		virtual const T* time_() const = 0;
		virtual const C* cash_() const = 0;
	};

	// Implement using FP data type
	class fp_instrument : public instrument<double,double> {
	protected:
		FP12 t, c; // time and cash flows
	public:
		fp_instrument()
		{ }
		fp_instrument(int n)
			: t(1,n), c(1, n)
		{ }
		fp_instrument(const fp_instrument&) = default;
		fp_instrument& operator=(const fp_instrument&) = default;
		virtual ~fp_instrument()
		{ }
		const _FP12* fp_time() const
		{
			return t.get();
		}
		const _FP12* fp_cash() const
		{
			return c.get();
		}
		void resize(int m)
		{
			t.resize(1, m);
			c.resize(1, m);
		}
		// Fix times and cash flows based on valuation and effective date, where
		// valuation is date price is quoted and effective is the first cash flow.
		void fix(excel_date valuation, excel_date effective)
		{
			fix_(valuation, effective);
		}
	private:
		virtual void fix_(excel_date, excel_date) = 0;
		size_t size_() const
		{
			return t.is_empty() ? 0 : t.columns();
		}
		const double* time_() const
		{
			return t.is_empty() ? 0 : t.array();
		}
		const double* cash_() const
		{
			return c.is_empty() ? 0 : c.array();
		}
	};

	template<class Tenor>
	class zero_coupon_bond : public fp_instrument {
		Tenor tenor;
	public:
		zero_coupon_bond(Tenor tenor)
			: tenor(tenor)
		{ }
		zero_coupon_bond(const zero_coupon_bond&) = default;
		zero_coupon_bond& operator=(const zero_coupon_bond&) = default;
		~zero_coupon_bond()
		{ }
	private:
		void fix_(excel_date valuation, excel_date effective)
		{
			effective = effective; // not used
			resize(1);

			excel_date maturity = date_add(valuation, tenor);
			t[0] = (maturity - valuation)/DAYS_PER_YEAR;
			c[0] = 1;
		}
	};

	template<class Tenor>
	class cash_deposit : public fp_instrument {
		Tenor tenor;
		double rate;
		day_count_basis dcb;
	public:
		cash_deposit(Tenor tenor, double rate, day_count_basis dcb = DCB_ACTUAL_360)
			: tenor(tenor), rate(rate), dcb(dcb)
		{ }
		cash_deposit(const cash_deposit&) = default;
		cash_deposit& operator=(const cash_deposit&) = default;
		~cash_deposit()
		{ }
	private:
		void fix_(excel_date valuation, excel_date effective)
		{
			effective = effective; // not used
			resize(1);

			excel_date maturity = date_add(valuation, tenor);
			t[0] = (maturity - valuation)/DAYS_PER_YEAR;			
			c[0] = 1 + rate*day_count_fraction(valuation, maturity, dcb);
		}
	};

	template<class Tenor>
	class forward_rate_agreement : public fp_instrument {
		Tenor tenor;
		double forward;
		day_count_basis dcb;
	public:
		forward_rate_agreement(Tenor tenor, double forward, day_count_basis dcb = DCB_ACTUAL_360)
			: tenor(tenor), forward(forward), dcb(dcb)
		{ }
		forward_rate_agreement(const forward_rate_agreement&) = default;
		forward_rate_agreement& operator=(const forward_rate_agreement&) = default;
		~forward_rate_agreement()
		{ }
	private:
		void fix_(excel_date valuation, excel_date effective) override
		{
			resize(2);

			t[0] = (effective - valuation)/DAYS_PER_YEAR;
			excel_date termination = date_add(effective, tenor); 
			t[1] = (termination - valuation)/DAYS_PER_YEAR;

			c[0] = -1;
			c[1] = 1 + forward*day_count_fraction(effective, termination,  dcb);
		}
	};

	class interest_rate_swap : public fp_instrument {
		years tenor;
		double coupon;
		frequency freq;
		day_count_basis dcb; // of fixed leg
	public:
		interest_rate_swap(years tenor, double coupon, frequency freq, day_count_basis dcb)
			: tenor(tenor), coupon(coupon), freq(freq), dcb(dcb)
		{ }
		interest_rate_swap(const interest_rate_swap&) = default;
		interest_rate_swap& operator=(const interest_rate_swap&) = default;
		~interest_rate_swap()
		{}
	private:
		void fix_(excel_date valuation, excel_date effective) override
		{

			excel_date termination = date_add(effective, tenor);
			
			int n = 1;
			while (date_add(effective, months{n*12/freq}) < termination) {
				++n;
			}
			resize(n + 1);
			
			t[0] = (effective - valuation)/DAYS_PER_YEAR;
			c[0] = -1;
			excel_date prev = effective;
			for (int i = 1; prev < termination; ++i) {
				excel_date next = date_add(effective, months{i*12/freq});
				t[i] = (next - valuation)/DAYS_PER_YEAR;
				c[i] = coupon*day_count_fraction(prev, next, dcb);
				prev = next;
			}
			ensure (prev == termination);
			c[c.columns()-1] += 1;
		}
	};

} // namespace xll

