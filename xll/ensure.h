// ensure.h - assert replacement that throws instead of calling abort()
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// #define NENSURE before including to turn ensure checking off
#pragma once
#ifdef _WIN32
#define NOMINMAX
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <limits>
#include <stdexcept>

#ifdef NENSURE
#define ensure(x)
#endif

#ifndef ensure

#define ENSURE_HASH_(x) #x
#define ENSURE_STRZ_(x) ENSURE_HASH_(x)
#define ENSURE_FILE "file: " __FILE__
#ifdef __FUNCTION__
#define ENSURE_FUNC "\nfunction: " __FUNCTION__
#else
#define ENSURE_FUNC ""
#endif
#define ENSURE_LINE "\nline: " ENSURE_STRZ_(__LINE__)
#define ENSURE_SPOT ENSURE_FILE ENSURE_LINE ENSURE_FUNC

#ifdef _DEBUG
#ifdef _WIN32 // defined for 64 bit Windows also
#define ensure(e) if (!(e)) { DebugBreak(); }
#else // gcc
#define ensure(e) if (!(e)) { __builtin_trap(); }
#endif

#define DEBUG_(e) e
#else // Release
#define ensure(e) if (!(e)) \
		throw std::runtime_error(ENSURE_SPOT "\nensure: \"" #e "\" failed"); \
		else (void)0;

#define DEBUG_(e)
#endif

#endif // ensure

