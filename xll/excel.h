// excel.h - Wrapper functions for Excel* functions.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include "oper.h"

namespace xll {

	template<typename... Args>
	inline OPER12 Excel(int xlf, const Args&... args)
	{
		OPER12 o;

        try {
		    int ret = ::Excel12(xlf, &o, sizeof...(args), &args...);
		    ensure (ret == xlretSuccess);
		    if (!o.isScalar())
			    o.xltype |= xlbitXLFree;
        }
        catch (const std::exception& ex) {
            XLL_ERROR(ex.what());
        }
		return o;
	}

	inline OPER12 Excelv(int xlf, const OPER12& args)
	{
		OPER12 o;
		LPXLOPER12 pargs[256]; // just like XLCALL.CPP

        try {
		    ensure (args.size() < 256);
		    for (int i = 0; i < args.size(); ++i) {
			    pargs[i] = (LPXLOPER12)&args[i];
		    }
		    int ret = ::Excel12v(xlf, &o, static_cast<int>(args.size()), pargs);
		    ensure (ret == xlretSuccess);
		    if (!o.isScalar())
			    o.xltype |= xlbitXLFree;
        }
        catch (const std::exception& ex) {
            XLL_ERROR(ex.what());
        }

        return o;
	}

} // namespace xll

// Just like Excel
inline xll::OPER12 operator&(const xll::OPER12& a, const xll::OPER12& b)
{
	return xll::Excel(xlfConcatenate, a, b);
}
