// GetWorkspace.cpp - Get Workspace items.
#include "../xll/xll.h"

using namespace xll;

// 23 Full path of the default startup directory or folder.
// 44 A three-column array of all currently registered procedures in dynamic link libraries (DLLs).

static AddIn xai_get_workspace(
    Function(XLL_LPXLOPER, XLL_DECORATE(L"xll_get_workspace", 4), L"_GET.WORKSPACE")
    .Arg(XLL_WORD, L"item", L"is the workspace item to get.")
    .Uncalced()
    .Category(L"XLL")
    .FunctionHelp(L"Get workspace item.")
);
extern "C" __declspec(dllexport) LPXLOPER12 WINAPI
xll_get_workspace(WORD item)
{
    static OPER result;

    result = Excel(xlfGetWorkspace, OPER(item));

    return &result;
}
