#include "xll.h"

using namespace xll;

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
	static char buf[17];

	encode(h, buf);

	return buf;
}
static AddIn xai_decode(
	Function(XLL_HANDLE, XLL_DECORATE(L"xll_decode", 4), L"DECODE")
	.Arg(XLL_PSTRING4, L"string", L"is an encoded handle.")
	.Category(L"XLL")
	.FunctionHelp(L"Convert string to handle.")
);
extern "C" __declspec(dllexport)
HANDLEX WINAPI xll_decode(const char* str)
{
//#pragma XLLEXPORT
	return decode(str);
}