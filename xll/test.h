// test.h - convenience struct for testing
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include <exception>
#include <functional>
#include "test.h"

namespace xll {
	// Run a function on construction.
    struct test {
        test(const std::function<void(void)>& f)
        {
            try {
                f();
            }
            catch (const std::exception& ex) {
                XLL_ERROR(ex.what());
            }
            catch (...) {
                XLL_ERROR("Unknown exception in xll::test");
            }
        }
    };
}