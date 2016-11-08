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
	template<class T, class C>
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
		const T& time(size_t i) const
		{
			ensure (i < size());

			return time_()[i];
		}
		const C* cash() const
		{
			return cash_();
		}
		const C& cash(size_t i) const
		{
			ensure (i < size());

			return cash_()[i];
		}
	private:
		// subclass must override
		virtual size_t size_() const = 0;
		virtual const T* time_() const = 0;
		virtual const C* cash_() const = 0;
	};

	// implement using FP data type
	class fp_instrument : public instrument<double,double> {
	protected:
		FP12 tc; // time and cash flows
	public:
		fp_instrument()
		{ }
		fp_instrument(int n)
			: tc(2, n)
		{ }
		fp_instrument(const fp_instrument&) = default;
		fp_instrument& operator=(const fp_instrument&) = default;
		virtual ~fp_instrument()
		{ }
		void resize(int n)
		{
			tc.resize(2, n);
		}
		const _FP12* fp() const
		{
			return tc.is_empty() ? 0 : tc.get();
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
			return tc.is_empty() ? 0 : tc.columns();
		}
		const double* time_() const
		{
			return tc.is_empty() ? 0 : tc.array();
		}
		const double* cash_() const
		{
			return tc.is_empty() ? 0 : tc.array() + size_();
		}
	};

	template<class Tenor>
	class zero_coupon_bond : public fp_instrument {
		Tenor t;
	public:
		zero_coupon_bond(Tenor maturity)
			: t(maturity)
		{ }
		zero_coupon_bond(const zero_coupon_bond&) = default;
		zero_coupon_bond& operator=(const zero_coupon_bond&) = default;
		~zero_coupon_bond()
		{ }
	private:
		void fix_(excel_date valuation, excel_date effective)
		{
			effective = effective; // not used
			tc.resize(2, 1);

			excel_date maturity = date_add(valuation, t);
			tc(0,0) = (maturity - valuation)/DAYS_PER_YEAR;
			tc(1,0) = 1;
		}
	};

	template<class Tenor>
	class cash_deposit : public fp_instrument {
		Tenor t;
		double r;
		day_count_basis dcb;
	public:
		cash_deposit(Tenor tenor, double rate, day_count_basis dcb = ACTUAL_360)
			: t(tenor), r(rate), dcb(dcb)
		{ }
		cash_deposit(const cash_deposit&) = default;
		cash_deposit& operator=(const cash_deposit&) = default;
		~cash_deposit()
		{ }
	private:
		void fix_(excel_date valuation, excel_date effective)
		{
			effective = effective; // not used
			tc.resize(2,1);

			excel_date maturity = date_add(valuation, t);
			tc(0,0) = (maturity - valuation)/DAYS_PER_YEAR;			
			tc(1,0) = 1 + r*day_count_fraction(valuation, maturity, dcb);
		}
	};

	template<class Tenor>
	class forward_rate_agreement : public fp_instrument {
		Tenor tenor;
		double f;
		day_count_basis dcb;
	public:
		forward_rate_agreement(Tenor tenor, double forward, day_count_basis dcb = ACTUAL_360)
			: tenor(tenor), f(forward), dcb(dcb)
		{ }
		forward_rate_agreement(const forward_rate_agreement&) = default;
		forward_rate_agreement& operator=(const forward_rate_agreement&) = default;
		~forward_rate_agreement()
		{ }
	private:
		void set_(excel_date valuation, excel_date effective)
		{
			tc.resize(2,2);

			fc(0,0) = (effective - valuation)/DAYS_PER_YEAR;
			excel_date termination = date_add(effective, tenor); 
			fc(0,1) = (termination - valuation)/DAYS_PER_YEAR;

			fc(1,0) = -1;
			fc(1,1) = 1 + f*day_count_fraction(effective, termination,  dcb);
		}
	};

	class interest_rate_swap : public fp_instrument {
		years tenor;
		double c;
		frequency q;
		day_count_basis dcb; // of fixed leg
	public:
		interest_rate_swap(years tenor, double coupon, frequency freq, day_count_basis dcb)
			: tenor(tenor), c(coupon), q(freq), dcb(dcb)
		{ }
		interest_rate_swap(const interest_rate_swap&) = default;
		interest_rate_swap& operator=(const interest_rate_swap&) = default;
		~interest_rate_swap()
		{}
	private:
		void fix_(excel_date valuation, excel_date effective)
		{

			excel_date termination = date_add(effective, tenor);
			
			int n = 1;
			while (date_add(effective, months(n*12/q)) < termination) {
				++n;
			}
			tc.resize(2, n + 1);
			
			tc(0,0) = (effective - valuation)/DAYS_PER_YEAR;
			tc(1,0) = -1;
			excel_date prev = effective;
			for (int i = 1; prev < termination; ++i) {
				excel_date next = date_add(effective, months(i*12/q));
				tc(0,i) = (next - valuation)/DAYS_PER_YEAR;
				tc(1,i) = c*day_count_fraction(prev, next, dcb);
				prev = next;
			}
			ensure (prev == termination);
			tc(1,tc.columns()-1) += 1;
		}
	};

} // namespace xll

