// addin.h - convenience wrapper for Excel add-ins
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include <cwctype>
#include <map>
#include "auto.h"
#include "args.h"

namespace xll {

	/// Manage the lifecycle of an Excel add-in.
	class AddIn {
    public:
        static inline std::map<OPER, Args> KeyArgsMap;
        static inline std::map<double, OPER> RegIdKeyMap;

        /// Register and Unregister an add-in when Excel calls xlAutoOpen and xlAutoClose.
		AddIn(const Args& args)
		{
            auto kv = std::make_pair(args.Key(), args);
            if (!KeyArgsMap.insert(kv).second) {
                //OPER key = args.Key();
                //MessageBoxW(GetForegroundWindow(), ex.what(), L"AddIn Auto<Open> failed", MB_OKCANCEL| MB_ICONWARNING);
            }

			Auto<Open> ao([args]() { 
				try {
                    if (!RegIdKeyMap.insert(std::make_pair(args.Register().val.num, args.Key())).second) {
                        //RegIdMap[args.Register().val.num] = args.Key();
                    }
				}
				catch (const std::exception& ex) {
                    MessageBoxA(GetForegroundWindow(), ex.what(), "AddIn Auto<Open> failed", MB_OKCANCEL| MB_ICONERROR);

					return FALSE;
				}

				return TRUE;
			});
			Auto<Close> ac([args]() {
				try {
					args.Unregister();
				}
				catch (const std::exception& ex) {
                    MessageBoxA(GetForegroundWindow(), ex.what(), "AddIn Auto<Close> failed", MB_OKCANCEL| MB_ICONERROR);

					return FALSE;
				}

				return TRUE;
			});
		}
    };
	using AddIn12 = AddIn;
	using AddInX = AddIn;
} // xll namespace


