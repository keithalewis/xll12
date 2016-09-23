// xll.h - Excel add-in library header file
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Module handle from DllMain
extern HMODULE xll_hModule;

#include "fp.h"
#include "error.h"
#include "on.h"
#include "addin.h"
#include "handle.h"
#include "test.h"

extern HMODULE xll_Instance;