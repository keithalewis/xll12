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
            XLOPER12 o_;
		    int ret = ::Excel12(xlf, &o_, sizeof...(args), &args...);
		    ensure (ret == xlretSuccess);
            o = o_;
		    if (!o.isScalar()) {
                ::Excel12(xlFree, 0, 1, &o_);
            }
        }
        catch (const std::exception& ex) {
            MessageBoxA(GetForegroundWindow(), ex.what(), "Excel failed", MB_OKCANCEL| MB_ICONERROR);
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
            MessageBoxA(GetForegroundWindow(), ex.what(), "Excelv failed", MB_OKCANCEL| MB_ICONERROR);
        }

        return o;
	}

} // namespace xll

// Just like Excel
inline xll::OPER12 operator&(const xll::OPER12& a, const xll::OPER12& b)
{
	return xll::Excel(xlfConcatenate, a, b);
}
