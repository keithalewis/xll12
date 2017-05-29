#include "Header.h"

using namespace xll;

Auto<Open> xao_open([]() -> int {
	XLC(Alert, OPER(L"Hi"));
	
	return TRUE;
});