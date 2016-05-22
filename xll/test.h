// test.h - convenience macros for testing
#pragma once

#define BEGIN_TEST(name) xll::Auto<Open> xao_test_ ## name([]() { try {

// add void(*)(void) functions that throw here...

#define END_TEST } catch (const std::exception& ex) { \
MessageBoxA(0, ex.what(), "Error", MB_OK); return FALSE; } return TRUE; });
