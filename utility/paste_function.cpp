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
		ensure (selection.size() == 1 || !"Selection must be a single cell.");
		OPER regid = Excel(xlCoerce, selection);
		// find addin corresponding to registerId in active cell
		auto key = AddIn::RegIdKeyMap.find(regid);
		ensure (key != AddIn::RegIdKeyMap.end());
		const Args& args = AddIn::KeyArgsMap[key->second];
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

// Add function with default arguments below the function call
static AddIn xai_insert_function(
	Macro(XLL_DECORATE(L"xll_insert_function", 0), L"XLL.INSERT.FUNCTION")
);
extern "C" __declspec(dllexport) int WINAPI
xll_insert_function()
{
	try {
		OPER selection = Excel(xlfSelection);
		ensure(selection.size() == 1 || !"Selection must be a single cell.");
		ensure(selection.xltype == xltypeSRef);
		OPER regid = Excel(xlCoerce, selection);
		// find addin corresponding to registerId in active cell
		auto key = AddIn::RegIdKeyMap.find(regid);
		ensure(key != AddIn::RegIdKeyMap.end());
		const Args& args = AddIn::KeyArgsMap[key->second];
		// function text
		OPER text(L"=");
		text &= args.FunctionText();
		text &= L"(";
		// args
		auto n = args.Arity();
		OPER sep(L"");
		for (decltype(n) i = 1; i <= n; ++i) {
			text &= sep;
			text &= OPER(L"R[") & OPER(i) & OPER(L"]C[0]");
			//Excel(xlfSetValue, Excel(xlfOffset, selection, OPER(i), OPER(0)), args.ArgumentDefault(i));
			Excel(xlcFormula, args.ArgumentDefault(i), Excel(xlfOffset, selection, OPER(i), OPER(0)));
			sep = OPER(L", ");
		}
		text &= L")";
		// paste into original active cell
		Excel(xlcFormula, text, selection);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
}
On<Key> xlo_insert_function(ON_CTRL ON_SHIFT L"C", L"XLL.INSERT.FUNCTION");
