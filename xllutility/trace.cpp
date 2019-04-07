// trace.cpp - trace Excel calculation
#include "../xll/xll.h"

using namespace xll;

static AddIn xai_trace(
    Function(XLL_LPXLOPER, XLL_DECORATE(L"xll_trace", 4), L"TRACE")
    .Arg(XLL_LPXLOPER, L"cell", L"is the cell to trace.")
    .Category(L"XLL")
    .FunctionHelp(L"Alert when cell is called in a calculation.")
    .Uncalced()
);
extern "C" __declspec(dllexport) LPXLOPER12 WINAPI 
xll_trace(LPXLOPER12 px)
{
    if (px->xltype == xltypeSRef) {
        std::wstring s = L"Reference: ";
        OPER o = Excel(xlfReftext, *px, OPER(true));
        s.append(o.val.str + 1, o.val.str[0]);
        s.append(L"\nFormula: ");
        o = Excel(xlfGetCell, OPER(6), *px);
        s.append(o.val.str + 1, o.val.str[0]);
        s.append(L"\nContents: ");
        o = Excel(xlfText, Excel(xlfGetCell, OPER(5), *px), OPER(L"General"));
        s.append(o.val.str + 1, o.val.str[0]);
        MessageBoxW(0, s.c_str(), L"XLL.TRACE", MB_OK);
    }

    return px;
}

