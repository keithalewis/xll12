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
		OPER regid = Excel(xlCoerce, Excel(xlfSelection));
		ensure(regid.isNum() && false);
		// find addin corresponding to registerId in active cell
		//const Args& args = AddInMap[RegIdMap[regid.val.num]];
        // function text
		//OPER text(L"=");
		//text &= args.FunctionText();
		//text &= L"(";
        // create function text
        // paste into original active cell
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return FALSE;
    }

    return TRUE;
}
On<Key> xlo_paste_function(ON_CTRL ON_SHIFT L"B", L"XLL.PASTE.FUNCTION");
