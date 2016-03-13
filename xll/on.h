// on.h - xlcOnXXX functions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include "auto.h"
#include "excel.h"

// used with On<Key>
#define ON_SHIFT   _T("+")
#define ON_CTRL    _T("^")
#define ON_ALT     _T("%")
#define ON_COMMAND _T("*")

namespace xll {

	struct Data {
		static const int On = xlcOnData;
	};
	struct Doubleclick {
		static const int On = xlcOnDoubleclick;
	};
	struct Entry {
		static const int On = xlcOnEntry;
	};
	struct Key {
		static const int On = xlcOnKey;
	};
	struct Recalc {
		static const int On = xlcOnRecalc;
	};
	struct Sheet {
		static const int On = xlcOnSheet;
	};
	struct Time {
		static const int On = xlcOnTime;
	};
	struct Window {
		static const int On = xlcOnWindow;
	};

	// static On<Key> ok("shortcut", "MACRO");
	template<class Key>
	class On {
		using xcstr = const XCHAR*;
	public:
		On(xcstr text, xcstr macro)
		{
			Auto<Open> xao([text, macro]() {
				return Excel(Key::On, OPER12(text), OPER12(macro))
					.type() == xltypeNum;
			});
			Auto<Close> xac([text]() {
				return Excel(Key::On, OPER12(text))
					.type() == xltypeNum;
			});
		}
		On(xcstr text, xcstr macro, bool activate)
		{
			ensure (Key::On == xlcOnSheet);

			Auto<Open> xao([text, macro, activate]() {
				return Excel(Key::On, OPER12(text), OPER12(macro), OPER12(activate))
					.type() == xltypeNum;
			});
			Auto<Close> xac([text]() {
				return Excel(Key::On, OPER12(text))
					.type() == xltypeNum;
			});
		}
		On(const OPER12& time, xcstr macro, 
			const OPER12& tolerance = OPER12{}, bool insert = true)
		{
			OPER12 x(5,1);

			ensure (Key::On == xlcOnTime);

			Auto<Open> xao([time, macro, tolerance, insert]() {
				return Excel(Key::On, OPER12(time), OPER12(macro), OPER12(tolerance), OPER12(insert))
					.type() == xltypeNum;
			});
			Auto<Close> xac([]() {
				return Excel(Key::On)
					.type() == xltypeNum;
			});
		}
	};
} // namespace xll
