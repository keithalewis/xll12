// addin.h - convenience wrapper for Excel add-ins
#pragma once
#include "auto.h"
#include "args.h"

namespace xll {
	/// Manage the lifecycle of an Excel add-in.
	class AddIn {
	public:
		/// Returns the RegisterId (Num) given the function text
		static OPER12 RegisterId(const OPER12& functionText)
		{
			return Excel(xlfEvaluate, Excel(xlfConcatenate, OPER12(L"="), functionText)); 
		}
		/// Register and Unregister an add-in when Excel calls xlAutoOpen and xlAutoClose.
		AddIn(const Args& args)
		{
			Auto<Open> ao([&]() { 
				return Register(args).isNum();
			});
			Auto<Close> ac([&]() {
				return Excel(xlfUnregister, RegisterId(args.FunctionText())).isNum();
			});
		}
	};

} // xll namespace


