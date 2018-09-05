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

// guids based on strings
inline std::wstring guid(const std::wstring& s)
{
    wchar_t buf[32+4+1];

    std::hash<std::wstring> hash;
    swprintf(buf, 36, L"%8x-0000-0000-0000-000000000000", hash(s));

    return std::wstring(buf);
}

inline void make_function(const Args& args)
{
    OPER12 function = args.FunctionText();

}

static AddIn xai_make_doc(
    Macro(L"?xll_make_doc", L"XLL.MAKE.DOC")
);
int xll_make_doc(void)
{
    try {
	OPER12 dir = Args::XlGetName();
	// get addin name
	// remove from dir
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return FALSE;
    }

    return TRUE;
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