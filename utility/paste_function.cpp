// paste_function.cpp - Paste a function with default arguments
#include "../xll/xll.h"

using namespace xll;

static AddIn xai_paste_function(
    Macro(XLL_DECORATE(L"xll_paste_function", 0), L"XLL.PASTE.FUNCTION")
);
extern "C" __declspec(dllexport) int WINAPI
xll_paste_function()
{
    try {
        // find addin corresponding to registerId in active cell
        // paste default args in rows below active cell
        // create function text
        // paste into original active cell
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return FALSE;
    }

    return TRUE;
}

// connect to Ctrl-Shift-B 