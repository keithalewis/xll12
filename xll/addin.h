// addin.h - convenience wrapper for Excel add-ins
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include <map>
#include "auto.h"
#include "args.h"

namespace xll {
	/// Manage the lifecycle of an Excel add-in.
	class AddIn {
    public:
        // Map procedure text to args.
        static std::map<OPER12, Args>& map()
        {
            static std::map<OPER12, Args> map_;

            return map_;
        }
		/// Register and Unregister an add-in when Excel calls xlAutoOpen and xlAutoClose.
		AddIn(const Args& args)
		{
            // remove initial '?' character
            OPER proc;
            if (args.isDocumentation()) {
                proc = args.FunctionText();
            }
            else {
                proc = args.Procedure();
                proc = OPER(proc.val.str + 2, proc.val.str[0] - 1);
            }
            map()[proc] = args;

			Auto<Open> ao([args]() { 
				return args.Register().isNum();
			});
			Auto<Close> ac([args]() {
				return args.Unregister();
			});
		}
    };
	using AddIn12 = AddIn;
	using AddInX = AddIn;
} // xll namespace


