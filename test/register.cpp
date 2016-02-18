// register.cpp - funcdname mangling
#include "../xll/xll.h"

using namespace xll;

XCHAR* funcdname[64];

#define L_(x) L ## x

//#define FOO Auto<Open> xao_foo ## __COUNTER__ ([](){Excelv(xlfRegister,Arguments(L"?foo"
