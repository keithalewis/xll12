// excel.h - Wrapper functions for Excel* functions.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include "oper.h"

namespace xll {

	template<typename... Args>
	inline OPER12 Excel(int xlf, const Args&... args)
	{
		OPER12 o;

		int ret = Excel12(xlf, &o, sizeof...(args), &args...);
		ensure (ret == xlretSuccess);
		if (o.xltype == xltypeStr || o.xltype == xltypeMulti)
			o.xltype |= xlbitXLFree;

		return o;
	}

	inline OPER12 Excelv(int xlf, const OPER12& args)
	{
		OPER12 o;
		LPXLOPER12 pargs[255]; // just like XLCALL.CPP
		for (int i = 0; i < args.size(); ++i) {
			pargs[i] = (LPXLOPER12)&args[i];
		}
		int ret = Excel12v(xlf, &o, static_cast<int>(args.size()), pargs);
		ensure (ret == xlretSuccess);
		ensure (o.type() != xltypeErr);
		if (o.xltype == xltypeStr || o.xltype == xltypeMulti)
			o.xltype |= xlbitXLFree;

		return o;
	}

}
