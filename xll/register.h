// register.h - register and Excel add-in
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include "excel.h"

namespace xll {

	// See https://msdn.microsoft.com/en-us/library/office/bb687900.aspx
	enum ARG {
		ModuleText,   // from xlGetName
		Procedure,    // C function
		TypeText,     // return type and arg codes 
		FunctionText, // Excel function
		ArgumentText, // Ctrl-Shift-A text
		MacroType,    // function, 2 for macro, 0 for hidden
		Category,     // for function wizard
		ShortcutText, // single character for Ctrl-Shift-char shortcut
		HelpTopic,    // filepath!HelpContextID or http://host/path!0
		FunctionHelp, // for function wizard
		ArgumentHelp, // 1-base index
	};

	using xcstr = const XCHAR*;

	class Args {
		OPER12 args;
		int arity;
	public:
		int Arity() const
		{
			//! parse TypeText
			return arity;
		}
		operator const OPER12&() const
		{
			return args;
		}
		Args(xcstr ReturnType, xcstr Procedure, xcstr FunctionText, int MacroType = 1)
			: arity(0)
		{
			args.resize(1, ARG::ArgumentHelp + 9);

			args[ARG::ModuleText] = Excel(xlGetName);
			args[ARG::Procedure] = Procedure;
			args[ARG::TypeText] = ReturnType;
			args[ARG::FunctionText] = FunctionText;
			args[ARG::MacroType] = MacroType;
		}
		Args& Procedure(xcstr procedure)
		{
			args[ARG::Procedure] = procedure;

			return *this;
		}
		Args& TypeText(xcstr typeText)
		{
			args[ARG::TypeText] = typeText;

			return *this;
		}
		Args& FunctionText(xcstr functionText)
		{
			args[ARG::FunctionText] = functionText;

			return *this;
		}
		Args& MacroType(xcstr macroType)
		{
			args[ARG::MacroType] = macroType;

			return *this;
		}
		Args& Category(xcstr category)
		{
			args[ARG::Category] = category;

			return *this;
		}
		Args& ShortcutText(XCHAR shortcutText)
		{
			args[ARG::ShortcutText] = OPER12(&shortcutText, 1);

			return *this;
		}
		Args& HelpTopic(xcstr helpTopic)
		{
			args[ARG::HelpTopic] = helpTopic;

			return *this;
		}
		Args& FunctionHelp(xcstr functionHelp)
		{
			args[ARG::FunctionHelp] = functionHelp;

			return *this;
		}
		Args& ArgumentHelp(int i, xcstr argumentHelp)
		{
			ensure (i != 0);

			if (args.size() < ARG::ArgumentHelp + i)
				args.resize(ARG::ArgumentHelp + i, 1);

			args[ARG::ArgumentHelp + i - 1] = argumentHelp;

			return *this;
		}

		Args& Arg(xcstr type, xcstr text, xcstr helpText = 0)
		{
			OPER12& Type = args[ARG::TypeText];
			Type = Excel(xlfConcatenate, Type, OPER12(type));
			
			OPER12& Text = args[ARG::ArgumentText];
			if (Excel(xlfLen, Text) != 0)
				Text = Excel(xlfConcatenate, Text, OPER12(L", "));
			Text = Excel(xlfConcatenate, OPER12(text));
			
			++arity;
			if (helpText)
				ArgumentHelp(Arity(), helpText);

			return *this;
		}
		Args& Num(xcstr text, xcstr helpText = 0)
		{
			return Arg(XLL_DOUBLE, text, helpText);
		}
		// Str ...
	};

	OPER12 Arguments(
		xcstr Procedure,        // C function
		xcstr TypeText,         // return type and arg codes 
		xcstr FunctionText,     // Excel function
		xcstr ArgumentText = 0, // Ctrl-Shift-A text
		int   MacroType = 1,    // function, 2 for macro, 0 for hidden
		xcstr Category = 0,     // for function wizard
		xcstr ShortcutText = 0, // single character for Ctrl-Shift-char shortcut
		xcstr HelpTopic = 0,    // filepath!HelpContextID or http://host/path!0
		xcstr FunctionHelp = 0, // for function wizard
		xcstr ArgumentHelp1 = 0,
		xcstr ArgumentHelp2 = 0,
		xcstr ArgumentHelp3 = 0,
		xcstr ArgumentHelp4 = 0,
		xcstr ArgumentHelp5 = 0,
		xcstr ArgumentHelp6 = 0,
		xcstr ArgumentHelp7 = 0,
		xcstr ArgumentHelp8 = 0,
		xcstr ArgumentHelp9 = 0
	)
	{
		OPER12 args(Excel(xlGetName));
		args.push_back(OPER12(Procedure));
		args.push_back(OPER12(TypeText));
		args.push_back(OPER12(FunctionText));
		args.push_back(OPER12(ArgumentText));
		args.push_back(OPER12(MacroType));
		args.push_back(OPER12(Category));
		args.push_back(OPER12(ShortcutText));
		args.push_back(OPER12(HelpTopic));
		args.push_back(OPER12(FunctionHelp));
		args.push_back(OPER12(ArgumentHelp1));
		args.push_back(OPER12(ArgumentHelp2));
		args.push_back(OPER12(ArgumentHelp3));
		args.push_back(OPER12(ArgumentHelp4));
		args.push_back(OPER12(ArgumentHelp5));
		args.push_back(OPER12(ArgumentHelp6));
		args.push_back(OPER12(ArgumentHelp7));
		args.push_back(OPER12(ArgumentHelp8));
		args.push_back(OPER12(ArgumentHelp9));

		return args;
	}


} // xll