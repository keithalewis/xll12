// excel.h - Wrapper functions for Excel* functions.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include "oper.h"

namespace xll {

	template<class X, typename... Args>
	inline OPER12 Excel(int xlf, const Args&... args)
	{
		OPER12 o;

		int ret = Excel12(xlf, &o, sizeof...(args), &OPER12(args)...);
		ensure (ret == xlretSuccess);
		o.xltype |= xlbitXLFree;

		return o;
	}

}
