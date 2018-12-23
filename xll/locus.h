// locus.h - location in a spreadsheet
#pragma once
#include <Windows.h>
#include "oper.h"
#include "excel.h"

namespace xll {
	namespace {

		inline void text2name(XCHAR* t)
		{

			for (int i = 1; i <= t[0]; ++i)
				if (!isalnum(t[i]))
					t[i] = XCHAR('_');
		}

		inline void replace(XCHAR* t, XCHAR r, XCHAR c)
		{
			for (int i = 1; i < t[0]; ++i) {
				if (t[i] == r)
					t[i] = c;
				else if (t[i] == c)
					t[i] = r;
			}
		}

	}
	class LOCUS {
		OPER xDefText;
	public:
		LOCUS()
		{
			OPER xCaller = Excel(xlfCaller);
			OPER xReftext = Excel(xlfReftext, xCaller);
			OPER xGetDef = Excel(xlfGetDef, xDefText, OPER(), OPER(2));
			// returns #NAME? if not found
			if (xGetDef.xltype != xltypeStr) {

			}
		}
		void set(const OPER& value)
		{
			Excel(xlfSetName, xDefText, value);
		}
		OPER get() const
		{
			return Excel(xlfEvaluate, Excel(xlfGetName, xDefText));
		}
	};
}
