// excel.h - Wrapper functions for Excel* functions.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include "oper.h"
#include <vector>

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

		std::vector<const XLOPER12*> pargs;
		for (auto& arg : args) {
			pargs.push_back(&arg);
		}
		int ret = Excel12v(xlf, &o, static_cast<int>(pargs.size()), const_cast<LPXLOPER12*>(pargs.data()));
		ensure (ret == xlretSuccess);
		if (o.xltype == xltypeStr || o.xltype == xltypeMulti)
			o.xltype |= xlbitXLFree;

		return o;
	}

}
