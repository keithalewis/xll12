// bachelier.cpp - The Bachelier model
#include "../xll/xll.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace xll;

// In the Bachelier model of stock price as Brownain motion, the value of a call is
// E max{F_t - k, 0} = (f - k) N(d) + sigma sqrt(t) N'(d), where N is the normal cdf.
// Implement a function for valuing the call option in the Bachelier model

//inline double bachelier_call_value(double f, double sigma, double k, double t)

// Implement an Excel add-in for this function called BACHELIER.CALL.VALUE.
//static AddInX xai_bachelier_call_value(
//	FunctionX(XLL_DOUBLEX, _T("?xll_bachelier_call_value"), _T("BACHELIER.CALL.VALUE"))
// double WINAPI ...

// Implement tests for the bachelier_call_value function.

// Create a spreadsheet that calls this function.

// What is the formula for F_t?
// F_t = f + sigma * B(t)
// B(t) as a Brownian Motion
