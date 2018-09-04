// doc.cpp - Create MAML documentation files
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#include "xll.h"

using namespace xll;

namespace {
	class scoped_handle {
		HANDLE h;
	protected:
		scoped_handle(HANDLE h)
			: h(h)
		{ }
	public:
		scoped_handle()
			: h(INVALID_HANDLE_VALUE)
		{ }
		scoped_handle(const scoped_handle&) = delete;
		scoped_handle& operator=(const scoped_handle&) = delete;
		~scoped_handle()
		{
            if (h != INVALID_HANDLE_VALUE) {
			    CloseHandle(h);
            }
		}
	};
}

void make_doc(void)
{
	OPER12 dir = Args::XlGetName();
	// get addin name
	// remove from dir
}

/*

FUNCTION function

Description
   ...

Syntax

FUNCTION(arg,...)

The DATEVALUE function syntax has the following arguments:

- *Arg* Argument help...

Remarks

...

Examples

...

*/