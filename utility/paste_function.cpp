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
		OPER selection = Excel(xlfSelection);
		ensure(selection.size() == 1);
		OPER regid = Excel(xlCoerce, selection);
		// find addin corresponding to registerId in active cell
		auto key = RegIdMap.find(regid);
		ensure(key != RegIdMap.end());
		const Args& args = AddInMap[key->second];
        // function text
		OPER text(L"=");
		text &= args.FunctionText();
		text &= L"(";
        // args
		auto n =args.Arity();
		OPER sep(L"");
		for (decltype(n) i = 1; i <= n; ++i) {
			text &= sep;
			text &= args.ArgumentDefault(i);
			sep = OPER(L", ");
		}
		text &= L")";
		// double quote text
		text = Excel(xlfSubstitute, text, OPER(L"\""), OPER(L"\"\""));
		// paste into original active cell
		Excel(xlcFormula, text, selection);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return FALSE;
    }

    return TRUE;
}
On<Key> xlo_paste_function(ON_CTRL ON_SHIFT L"B", L"XLL.PASTE.FUNCTION");
