// defines.h - Excel definitions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#pragma once

/// Use #pragma XLLEXPORT in function body instead of DEF file.
/// !!! use constexpr and static_assert to detect if decorated name is stdcall !!!
// constexpr bool is_stdcall(const char* f) { return strchr(f, "@"); } ???
#define XLLEXPORT comment(linker, "/export:" __FUNCDNAME__ "=" __FUNCTION__)

#ifdef XLL_EXPORTS
#define XLL_DLLIMPEXP __declspec(dllexport)
#define XLL_EXTERN
#else
#define XLL_DLLIMPEXP __declspec(dllimport)
#define XLL_EXTERN extern
#endif

// 64-bit uses different name decoration
#ifdef _M_X64 
#define XLL_DECORATE(s,n) s
#define XLL_X64(x) x
#define XLL_X32(x)
#else
#define XLL_DECORATE(s,n) "_" s "@" #n
#define XLL_X64(x)	
#define XLL_X32(x) x
#endif 

#define XLL_BOOL     L"A"  // short int used as logical
#define XLL_DOUBLE   L"B"  // double
#define XLL_CSTRING  L"C%" // XCHAR* to C style NULL terminated unicode string
#define XLL_PSTRING  L"D%" // XCHAR* to Pascal style byte counted unicode string
#define XLL_DOUBLE_  L"E"  // pointer to double
#define XLL_CSTRING_ L"F%" // reference to a null terminated unicode string
#define XLL_PSTRING_ L"G%" // reference to a byte counted unicode string
#define XLL_USHORT   L"H"  // unsigned 2 byte int
#define XLL_WORD     L"J"  // signed 4 byte int
#define XLL_SHORT    L"I"  // signed 2 byte int
#define XLL_LONG     L"J"  // signed 4 byte int
#define XLL_FP       L"K%" // pointer to struct FP
#define XLL_BOOL_    L"L"  // reference to a boolean
#define XLL_SHORT_   L"M"  // reference to signed 2 byte int
#define XLL_LONG_    L"N"  // reference to signed 4 byte int
#define XLL_LPOPER   L"Q"  // pointer to OPER struct (never a reference type).
#define XLL_LPXLOPER L"U"  // pointer to XLOPER struct
// Modify add-in function behavior.
#define XLL_VOLATILE L"!"  // called every time sheet is recalced
#define XLL_UNCALCED L"#"  // dereferencing uncalced cells returns old value
#define XLL_THREAD_SAFE L"$"    // declares function to be thread safe
#define XLL_CLUSTER_SAFE L"&"	// declares function to be cluster safe
#define XLL_ASYNCHRONOUS L"X"	// declares function to be asynchronous
#define XLL_VOID     L">"	    // return type to use for asynchronous functions

#define XLL_HANDLE XLL_DOUBLE

#ifdef XLL_EXPORTS
#define DECLSPEC_IMPEXP //__declspec(dllexport)
#else
#define DECLSPEC_IMPEXP __declspec(dllimport)
#endif

// Convenience wrappers for Excel calls
#define XLF(fn, ...) xll::Excel(xlf##fn, __VA_ARGS__)
#define XLC(fn, ...) xll::Excel(xlc##fn, __VA_ARGS__)
#define XL_(fn, ...) xll::Excel(xl ##fn, __VA_ARGS__)
