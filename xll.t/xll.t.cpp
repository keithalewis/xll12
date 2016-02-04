// xll.t.cpp - Console testing program for xll12
#include "../xll/xll.h"

//[module(name="xll12")];
using namespace xll;

void test_swap()
{
	OPER12 str(L"foo");
	OPER12 multi(2,3);
	swap(str, multi);
	ensure (str.xltype == xltypeMulti);
	ensure (str.rows() == 2);
	ensure (str.columns() == 3);
	ensure (multi.xltype == xltypeStr);
	ensure (multi == L"foo");
}

void test_oper()
{
	OPER12 o;
	ensure (o.xltype == xltypeNil);
	OPER12 o2(o);
	ensure (o2.xltype == xltypeNil);
	ensure (o == o2);
	o = o2;
	ensure (o.xltype == xltypeNil);
	ensure (o == o2);
	ensure (!(o != o2));
}

void test_num()
{
	OPER12 num(1.23);
	ensure (num.xltype == xltypeNum);
	ensure (num.val.num == 1.23);
	ensure (num == 1.23); // operator double
	ensure (1.23 == num);
	ensure (num);
	num = 0; // calls operator=(double)
	ensure (num.xltype == xltypeNum);
	ensure (num.val.num == 0);
	ensure (num == 0);
	ensure (!num);

	OPER12 num2(num);
	ensure (num == num2);
	num = num2;
	ensure (num == num2);
}

void test_str()
{
//	[cpp_quote(__FUNCDNAME__)];
//	[idl_quote(__FUNCTION__)];
	OPER12 str(L"str");
	ensure (str.xltype == xltypeStr);
	ensure (str.val.str[0] == 3);
	ensure (0 == wcsncmp(str.val.str + 1, L"str", 3));
	ensure (str == L"str");
	OPER12 str2(str);
	ensure (str2.xltype == xltypeStr);
	ensure (str2.val.str[0] == 3);
	ensure (0 == wcsncmp(str2.val.str + 1, L"str", 3));
	ensure (str == str2);
	str = str2;
	ensure (str == str2);

	str = L"foobar";
	ensure (str.xltype == xltypeStr);
	ensure (str == L"foobar");

	OPER12 o;
	o = L"foobaz";
	ensure (o == L"foobaz");
}
void test_bool()
{
	OPER12 b(true);
	ensure (b.xltype == xltypeBool);
	ensure (b.val.xbool);
	ensure (b.val.xbool == 1);
	ensure (b.val.xbool == TRUE);
}
void test_multi()
{
	OPER12 multi(2,3);
	ensure (multi.xltype == xltypeMulti);
	ensure (multi.rows() == multi.val.array.rows);
	ensure (multi.columns() == multi.val.array.columns);
	ensure (multi[0] == OPER12());

	multi[0] = 1.23;
	multi[1] = L"str";
}

int main()
{
	test_swap();
	test_oper();
	test_num();
	test_str();
	test_bool();
	test_multi();

	return 0;
}