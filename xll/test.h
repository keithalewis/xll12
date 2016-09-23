// test.h - convenience macros for testing
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once

#define TEST_BEGIN(name) \
xll::Auto<Open> xao_test_ ## name([]() { try {

#define TEST_END \
} catch (const std::exception& ex) { \
	MessageBoxA(0, ex.what(), "Error", MB_OK); return FALSE; } \
return TRUE; });
