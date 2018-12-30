// addin.h - convenience wrapper for Excel add-ins
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include <cwctype>
#include <map>
#include "auto.h"
#include "args.h"

namespace xll {

	inline std::map<OPER, Args> AddInMap;
	inline std::map<double, OPER> RegIdMap; // register id to Key

	/// Manage the lifecycle of an Excel add-in.
	class AddIn {
    public:
		/// Register and Unregister an add-in when Excel calls xlAutoOpen and xlAutoClose.
		AddIn(const Args& args)
		{
            AddInMap[args.Key()] = args;

			Auto<Open> ao([args]() { 
				try {
					RegIdMap[args.Register().val.num] = args.Key();
				}
				catch (const std::exception& ex) {
					XLL_ERROR(ex.what());

					return FALSE;
				}

				return TRUE;
			});
			Auto<Close> ac([args]() {
				try {
					args.Unregister();
				}
				catch (const std::exception& ex) {
					XLL_ERROR(ex.what());

					return FALSE;
				}

				return TRUE;
			});
		}
    };
	using AddIn12 = AddIn;
	using AddInX = AddIn;
} // xll namespace


