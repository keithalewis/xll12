// addin.h - convenience wrapper for Excel add-ins
#pragma once
#include "auto.h"
#include "args.h"

namespace xll {
	/// Manage the lifecycle of an Excel add-in.
	class AddIn {
	public:
		AddIn(const Args& args)
		{
			Auto<Open> ao([&]() { return xltypeNum == Excelv(xlfRegister, args).type(); });
			Auto<Close> ac([&]() {
				return xltypeNum == Excel(xlfUnregister,	
					// The RegisterId of the function.
					Excelv(xlfEvaluate, 
						Excel(xlfConcatenate, OPER12(L"="), args.FunctionText()))); 
			});
		}
	};

} // xll namespace


