// xll.t.cpp - Console testing program for xll12
#include "../xll/xll.h"
#include <ctime>
#include <numeric>
#include <random>

//[module(name="xll12")];
using namespace xll;

static std::default_random_engine dre;

void test_sref()
{
	REF12 r1, r2;
	ensure (r1 == r2);
	REF12 r3(1,2,3,4);
	REF12 r4(r3);
	ensure (r3 == r4);
	r2 = r3;
	ensure (r2 == r3);

	OPER12 o1(r2);
	ensure (o1.type() == xltypeSRef);
	ensure (r2 == o1.val.sref.ref);

	OPER12 o2(REF12(1,2,3,4));
	ensure (o1 == o2);
	ensure (o2.val.sref.ref == r2);

	{
		REF12 r(1,2,3,4);
		r.move(1,1);
		ensure (r == REF12(2,3,3,4));
		r.up();
		ensure (r == REF12(1,3,3,4));
		r.right();
		ensure (r == REF12(1,4,3,4));
		r.down();
		ensure (r == REF12(2,4,3,4));
		r.left();
		ensure (r == REF12(2,3,3,4));

		ensure (move(r, 2, 3) == REF12(4, 6, 3, 4));
	}
	{
		// strict-weak ordering
		// !(x < y) && !(y < x) is an equivalence relation
		// x R x, x R y => y R x, !(x R y) and y R z => x R z
		std::uniform_int_distribution<RW> u(1, 10);
		auto R = [](const REF12& x, const REF12& y) {
			return !(x < y) && !(y < x);
		};
		auto ref = [&u]() { return REF12(u(dre),u(dre),u(dre),u(dre)); };
		for (int i = 0; i < 100; ++i) {
			auto x = ref(), y = ref(), z = ref();
			ensure (R(x, x));
			ensure (R(y, y));
			ensure (R(z, z));
			ensure (R(x, y) ? R(y, x) : true);
			ensure ((R(x, y) && R(y, z)) ? R(x, z) : true);
		}
	}
}

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
	ensure (o.size() == 0);
	ensure (o.xltype == xltypeMissing);
	OPER12 o2(o);
	ensure (o2.xltype == xltypeMissing);
	ensure (o == o2);
	o = o2;
	ensure (o.xltype == xltypeMissing);
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
	num = 0.; // calls operator=(double)
	ensure (num.xltype == xltypeNum);
	ensure (num.val.num == 0);
	ensure (num == 0);
	ensure (!num);

	OPER12 num2(num);
	ensure (num == num2);
	num = num2;
	ensure (num == num2);

    OPER12 o3{ 0 };
    ensure(o3.xltype == xltypeNum);
    ensure(o3.val.num == 0);
//    OPER12 o4 = 0; // calls OPER12(const XCHAR*) !!!
//    ensure(o4.xltype == xltypeNum);
//    ensure(o4.val.num == 0);
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
	o = L"foo";
	ensure (o == L"foo");
	ensure (o != L"bar");
	o &= L"bar";
	ensure (o.val.str[0] == 6);
	ensure (o == L"foobar");

	const XCHAR* null = 0;
	OPER12 Null(null);
	ensure (Null.xltype == xltypeMissing);
	OPER12 Empty(L"");
	ensure (Empty.xltype == xltypeStr);
	ensure (Empty.val.str[0] == 0);
	Empty &= L"a";
	ensure (Empty.val.str[0] == 1);
	ensure (Empty.val.str[1] == L'a');

	OPER12 o2;
	o2 = L"foo";
	ensure (o2 == L"foo");
	o2 &= L"bar";
	ensure (o2 == L"foobar")
	{
		auto v = std::uniform_int_distribution<int>{1, 0x7FFE};
        auto u = std::uniform_int_distribution<int>{ 1, -1 + std::numeric_limits<XCHAR>::max() };
        for (int i = 0; i < 100; ++i) {
			size_t len = v(dre);
			std::wstring s3;
			s3.resize(len);
			std::generate_n(begin(s3), len, [&]() { return static_cast<XCHAR>(u(dre)); });
			OPER12 o3(s3);
			o3 &= s3;
			ensure (o3.val.str[0] == 2*len);
		}
	}
}
void test_bool()
{
	OPER12 b(true);
	ensure (b.xltype == xltypeBool);
	ensure (b.val.xbool);
	ensure (b.val.xbool == 1);
	ensure (b.val.xbool == TRUE);
	ensure (b);
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

	int i = 0;
	for (auto& m : multi)
		ensure (m == multi[i++]);

	multi.push_back(multi);
	ensure (multi.xltype == xltypeMulti);
	ensure (multi.rows() == 4);
	ensure (multi.columns() == 3);
/*
	bool thrown = false;
	try {
		multi.push_back(OPER12(1.23));
	}
	catch (...) {
		thrown = true;
	}
	ensure (thrown);
*/
	OPER12 m0;
	m0.push_back(OPER12(L"foo"));
	ensure (m0.xltype == xltypeMulti);
	ensure (m0.rows() == 1);
	ensure (m0.columns() == 1);
	ensure (m0[0] == L"foo");

	std::uniform_int_distribution<RW> u(1, 100);
	for (int j = 0; j < 100; ++j) {
		OPER12 a(u(dre), u(dre)), b(u(dre), u(dre));
		if (a.columns() == b.columns()) {
			if (a.rows() == 1 && b.rows() != 1)
				b.resize(1, b.size());
			a.push_back(b);
		}
	}

	{
		OPER12 o{OPER12(1.23), OPER12(L"foo")};
		ensure (o.xltype == xltypeMulti);
		ensure (o.size() == 2);
		ensure (o[0] == 1.23);
		ensure (o[1] == L"foo");
	}
	{
		OPER12 o{{OPER12(1.23)},{OPER12(L"foo"), OPER12(true)}};
		ensure (o.xltype == xltypeMulti);
		ensure (o.rows() == 2);
		ensure (o.columns() == 2);
		ensure (o(0,0) == 1.23);
		ensure (o(0,1) == OPER12());
		ensure (o(1,0) == L"foo");
		ensure (o(1,1) == true);
	}
}
void test_int()
{
	{
		OPER o(1);
		ensure (o.xltype == xltypeNum);
		ensure (o.val.num == 1);
	}
	{
		OPER o(1l);
		ensure (o.xltype == xltypeNum);
		ensure (o.val.num == 1);
	}
}
void test_handle()
{
	auto pi = new std::vector<int>();
	handle<std::vector<int>> h(pi);
	auto pi2 = h.ptr();
	ensure (pi == pi2);
	delete pi;
}

void test_arity()
{
	Args args(XLL_DOUBLE, L"?proc", L"Proc");
	ensure (args.Arity() == 0);
	args.Num(L"x", L"is an x");
	ensure (args.Arity() == 1);
	args.Arg(XLL_CSTRING, L"s", L"is a string");
	ensure (args.Arity() == 2);
	args.Uncalced();
	ensure (args.Arity() == 2);
	args.Volatile();
	ensure (args.Arity() == 2);
}

void test_fp()
{
	xll::FP12 a;
	ensure (a.is_empty());
	ensure (a[0] != a[0]);
	a.resize(2,3);
	ensure (a.size() == 6);
	ensure (a.rows() == 2);
	ensure (a.columns() == 3);
	a(1,2) = 3;
	ensure (a[5] == 3);

	std::iota(begin(a), end(a), 0);
	ensure (a[0] == 0);
	ensure (a[5] == 5);
	ensure (a(0,0) == 0);
	ensure (a(1,2) == 5);
	ensure (a.index(-1, -1) == 5);
	ensure (a.index(-2, -6) == 0);

	xll::FP12 a_(a);
	ensure (a_ == a);
	xll::FP12 _a;
	_a = a;
	ensure (_a == a);

	xll::FP12 A_(*a.get());
	ensure (A_ == a);
	xll::FP12 _A;
	_A = *a.get();
	ensure (_A == a);

	xll::FP12 a2;
	a2.push_back(1.23);
	ensure (a2.rows() == 1);
	ensure (a2.columns() == 1);
	ensure (a2[0] == 1.23);
	a2.push_back(4.56);
	ensure (a2.rows() == 1);
	ensure (a2.columns() == 2);
	ensure (a2[1] = 4.56);
	ensure (a2(0,1) == 4.56);

	a2.push_back({7, 8});
	ensure (a2.rows() == 1);
	ensure (a2.columns() == 4);
	ensure (a2[2] == 7);
	ensure (a2[3] == 8);

	a2.resize(1, 2);
	ensure (a2.rows() == 1);
	ensure (a2.columns() == 2);
	ensure (a2[0] = 1.23);
	ensure (a2[1] = 4.56);
	ensure (a2(0,1) == 4.56);
	a2.push_down({7,8});
	ensure (a2.rows() == 2);
	ensure (a2.columns() == 2);
	ensure (a2(1,0) == 7);
	ensure (a2(1,1) == 8);

	xll::FP12 a3{1,2};
	ensure (a3.rows() == 1);
	ensure (a3.columns() == 2);

	xll::FP12 a4{{0},{1,2}};
	ensure (a4.rows() == 2);
	ensure (a4.columns() == 2);
	ensure (a4(0,0) == 0);
//	ensure (a4(0,1) == 0); // uninitialized
	ensure (a4(1,0) == 1);
	ensure (a4(1,1) == 2);
}

void test_error()
{
	XLL_ALERT_LEVEL(0);
	ensure (0 == XLL_ALERT_LEVEL(1));
	ensure (1 == XLL_ALERT_LEVEL(2));
	ensure (2 == XLL_ALERT_LEVEL(0));
}

int main()
{
	dre.seed(static_cast<unsigned>(::time(0)));

	test_sref();
	test_swap();
	test_oper();
	test_num();
	test_str();
	test_bool();
	test_multi();
	test_int();
//	test_handle();
	test_arity();
	test_fp();
	test_error();

	return 0;
}