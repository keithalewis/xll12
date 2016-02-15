// register.h - register an Excel add-in
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include "excel.h"
#include "defines.h"
#include <stdlib.h>
#include <cuchar>
#include <algorithm>
#include <iterator>
#include <map>

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
		Args()
			: arity(0)
		{
			args[ARG::ModuleText] = Excel(xlGetName);
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
		Args& Procedure(const OPER12& procedure)
		{
			args[ARG::Procedure] = procedure;

			return *this;
		}
		Args& TypeText(const OPER12& typeText)
		{
			args[ARG::TypeText] = typeText;

			return *this;
		}
		Args& FunctionText(const OPER12& functionText)
		{
			args[ARG::FunctionText] = functionText;

			return *this;
		}
		Args& MacroType(int macroType)
		{
			args[ARG::MacroType] = macroType;

			return *this;
		}
		Args& Category(const OPER12& category)
		{
			args[ARG::Category] = category;

			return *this;
		}
		Args& ShortcutText(XCHAR shortcutText)
		{
			args[ARG::ShortcutText] = OPER12(&shortcutText, 1);

			return *this;
		}
		Args& HelpTopic(const OPER12& helpTopic)
		{
			args[ARG::HelpTopic] = helpTopic;

			return *this;
		}
		Args& FunctionHelp(const OPER12& functionHelp)
		{
			args[ARG::FunctionHelp] = functionHelp;

			return *this;
		}
		Args& ArgumentHelp(int i, const OPER12& argumentHelp)
		{
			ensure (i != 0);

			if (args.size() < ARG::ArgumentHelp + i)
				args.resize(ARG::ArgumentHelp + i, 1);

			args[ARG::ArgumentHelp + i - 1] = argumentHelp;

			return *this;
		}

		Args& Arg(const OPER12& type, const OPER12& text, const OPER12& helpText = OPER12{})
		{
			OPER12& Type = args[ARG::TypeText];
			Type = Excel(xlfConcatenate, Type, type);
			
			OPER12& Text = args[ARG::ArgumentText];
			if (Excel(xlfLen, Text) != 0)
				Text = Excel(xlfConcatenate, Text, OPER12(L", "));
			Text = Excel(xlfConcatenate, text);
			
			++arity;
			if (helpText.type() != xltypeMissing)
				ArgumentHelp(Arity(), helpText);

			return *this;
		}
		Args& Num(const OPER12& text, const OPER12& helpText = OPER12{})
		{
			return Arg(OPER12(XLL_DOUBLE), text, helpText);
		}
		// Str ...
	};

	inline OPER12 Arguments(
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
	/*
	// Convert __FUNCDNAME__ to arguments for xlfRegister
	inline Args Demangle(const XCHAR* F)
	{
		static std::map<XCHAR,const OPER12> arg_map = {
			{ L'F', OPER12(XLL_SHORT) },
			{ L'G', OPER12(XLL_WORD) }, // also USHORT
			{ L'H', OPER12(XLL_BOOL) },
			{ L'J', OPER12(XLL_LONG) },
			{ L'N', OPER12(XLL_DOUBLE) },
		};

		// C to Excel naming convention
		auto function_text = [](OPER12 o) {
			ensure (o.type() == xltypeStr);
			for (int i = 1; i <= o.val.str[0]; ++i) {
				if (o.val.str[i] == L'_')
					o.val.str[i] = L'.';
				else
					o.val.str[i] = ::towupper(o.val.str[i]);
			}

			return o;
		};

		Args args;
		args.MacroType(1); // function
								  
		// "?foo@@YGNN@Z"

		ensure (F && *F == '?');
		auto E = wcschr(F, L'@');
		ensure (E);
		args.Procedure(OPER12(F, E - F));
		args.FunctionText(function_text(OPER12(F + 1, E - F - 1)));

		F = E;
		ensure (*++F == L'@');
		ensure (*++F == L'Y');
		ensure (*++F == L'G');
		ensure (arg_map.find(*++F) != arg_map.end());
		args.TypeText(arg_map[*F]);
		while (*++F != L'@') {
			// if (*F == L'P') { pointer...
			ensure (arg_map.find(*F) != arg_map.end());
//			type = Excel(xlfConcatenate, type, OPER12(arg_map[*F]));
		}

		return args;
	}
	*/
} // xll