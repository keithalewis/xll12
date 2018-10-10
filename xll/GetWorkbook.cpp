// GetWorkbook.cpp - Get Workbook items
#include "../xll/xll.h"

using namespace xll;

static AddIn xai_get_workbook(
    Function(XLL_LPXLOPER, XLL_DECORATE(L"xll_get_workbook", 4), L"_GET.WORKBOOK")
    .Arg(XLL_WORD, L"item", L"is the workbook item to get.")
    .Uncalced()
    .Category(L"XLL")
    .FunctionHelp(L"Get workbook item.")
);
extern "C" __declspec(dllexport) LPXLOPER12 WINAPI
xll_get_workbook(WORD item)
{
    static OPER result;

    result = Excel(xlfGetWorkbook, OPER(item));

    return &result;
}

