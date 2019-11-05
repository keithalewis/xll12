#include "xll.h"

using namespace xll;

// character indicating handle string
static char ptr_c = '*';

static AddIn xai_encode(
	Function(XLL_PSTRING4, XLL_DECORATE(L"xll_encode", 8), L"ENCODE")
	.Arg(XLL_HANDLE, L"handle", L"is a handle.")
	.Category(L"XLL")
	.FunctionHelp(L"Convert handle to string.")
);
extern "C" __declspec(dllexport)
const char* WINAPI xll_encode(HANDLEX h)
{
//#pragma XLLEXPORT
	static char buf[19];

	int64_t u = static_cast<int64_t>(h);
	encode(buf + 1, u);

	buf[0] = buf[1] + 1;
	buf[1] = ptr_c;

	return buf;
}
static AddIn xai_decode(
	Function(XLL_HANDLE, XLL_DECORATE(L"xll_decode", 4), L"DECODE")
	.Arg(XLL_PSTRING4, L"string", L"is an encoded handle.")
	.Category(L"XLL")
	.FunctionHelp(L"Convert string to handle.")
);
extern "C" __declspec(dllexport)
HANDLEX WINAPI xll_decode(char* str)
{
//#pragma XLLEXPORT
	handlex h;

	try {
		ensure(str[1] == ptr_c);
		str[1] = str[0] - 1;

		int64_t u;
		decode(str + 1, u);
		h = static_cast<double>(u);

		// put things back?
		str[0] = str[1] + 1;
		str[1] = ptr_c;
	}
	catch (const std::exception & ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}
