// asm.cpp - call inline assembly
#include "../xll/xll.h"

using namespace xll;

// !!!Use https://github.com/herumi/xbyak
// must use compiler intrinsics for x64
#ifndef _M_X64 
AddIn xai_asm(
    Function(XLL_LONG, L"?xll_asm", L"XLL.ASM")
    .Arg(XLL_LONG, L"num", L"is a number")
    .Arg(XLL_LONG, L"pow", L"is a power")
    .Category(L"XLL")
    .FunctionHelp(L"Return num*2^pow")
);
LONG WINAPI xll_asm(LONG num, LONG pow)
{
#pragma XLLEXPORT
    __asm
    {
        mov eax, num; Get first argument
        mov ecx, pow; Get second argument
        shl eax, cl; EAX = EAX * (2 to the power of CL)
    }
    // returns value in EAX
}
#endif