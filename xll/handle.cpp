#include "xll.h"

using namespace xll;
#if 0
static AddIn xai_encode(
	Function(XLL_PSTRING, XLL_DECORATE(L"xll_encode", 8), L"ENCODE")
	.Arg(XLL_HANDLE, L"handle", L"is a handle.")
	.Category(L"XLL")
	.FunctionHelp(L"Convert handle to string.")
);
extern "C" __declspec(dllexport)
const wchar_t* WINAPI xll_encode(HANDLEX h)
{
//#pragma XLLEXPORT
	static wchar_t buf[17];

	encode((HANDLEX)h, buf);

	return buf;
}
static AddIn xai_decode(
	Function(XLL_HANDLE, XLL_DECORATE(L"xll_decode", 4), L"DECODE")
	.Arg(XLL_PSTRING, L"string", L"is an encoded handle.")
	.Category(L"XLL")
	.FunctionHelp(L"Convert string to handle.")
);
extern "C" __declspec(dllexport)
HANDLEX WINAPI xll_decode(const wchar_t* str)
{
//#pragma XLLEXPORT
	return decode(str);
}
#endif // 0