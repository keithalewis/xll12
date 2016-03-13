// test.cpp
#include "../xll/xll.h"

using namespace xll;
#if 0
// minimal function registration example
int xll_foo0(void)
{ 
	Excelv(xlfRegister, OPER12{Excel(xlGetName),
		OPER12(L"?foo"), OPER12(XLL_DOUBLE XLL_DOUBLE),
		OPER12(L"FOO0"), OPER12(L"Num"), OPER12(1)});

	return 1; 
};
double WINAPI foo(double x)
{
#pragma XLLEXPORT
	return x;
}
Auto<Open> xao_foo0(xll_foo0);

int xll_test()
{
	try {
		{
			// register by hand
			OPER12 o{Excel(xlGetName),
				OPER12(L"?foo"), OPER12(XLL_DOUBLE XLL_DOUBLE),
				OPER12(L"FOO"), OPER12(L"Num"), OPER12(1)};
			Excelv(xlfRegister, o);
		}
		{
			// register by hand
			Excelv(xlfRegister, OPER12{Excel(xlGetName),
				OPER12(L"?foo"), OPER12(XLL_DOUBLE XLL_DOUBLE),
				OPER12(L"FOO"), OPER12(L"Num"), OPER12(1)});
		}
		{
			// register using Args class
			Args o;
			o.Procedure(L"?foo");
			o.TypeText(L"BB");
			o.FunctionText(L"FOO");
			o.MacroType(1);
			XLF(Register, o);
		}
		{
			// Arguments vs Args
			OPER12 o1 = Arguments(L"?foo", XLL_DOUBLE XLL_DOUBLE, L"FOO", L"Num");
			OPER12 o2 = Args(XLL_DOUBLE, L"?foo", L"FOO").Arg(XLL_DOUBLE, L"Num");
			ensure (o1 == o2);
			Excelv(xlfRegister, o1);
			o2[ARG::FunctionText].val.str[3] = L'P';
			Excelv(xlfRegister, o2);
		}
		{
			// test Excel() and xlFree
			OPER12 foo(L"foo");
			OPER12 foo2 = Excel(xlfLeft, Excel(xlfConcatenate, foo, foo), OPER12(4));
			ensure (foo2 == L"foof");
			foo2 = XLF(Right, XLF(Concatenate, foo, foo), OPER12(4));
			ensure (foo2 == L"ofoo");
		}
		{
			// operator& work like Excel concatenation
			OPER12 o1(L"foo"), o2(1.23);
			OPER12 o3 = o1 & o2;
			ensure (o3 == L"foo1.23");
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return FALSE;
	}
	catch (...) {
		XLL_ERROR("unknown exception");
		return FALSE;
	}

	return TRUE;
}
Auto<Open> xao_test(xll_test);

// lambdas work too
Auto<Open> xao_foo2_([]{
	Excelv(xlfRegister, Args(XLL_DOUBLE, L"?foo2", L"FOO2_").Arg(XLL_DOUBLE, L"Num"));
// #VALUE! since FOO2 is the name of a cell
//	Excelv(xlfRegister, Args(XLL_DOUBLE, L"?foo2", L"FOO2").Arg(XLL_DOUBLE, L"Num"));
	return 1;
});
double WINAPI foo2(double x)
{
#pragma XLLEXPORT

	return 2*x;
}

#endif // 0