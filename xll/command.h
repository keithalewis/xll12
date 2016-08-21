// command.h - 
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include "excel.h"

namespace xll {

enum COMMAND {
	NOTHING   = 3, // no open worksheets
	INFO      = 4,
	CELL      = 7,
	OBJECT    = 8,
	CHART     = 9,
	WORKSHEET = 10,
	VBA       = 12,
};

enum MENU {
	CELL_TOOLBARS = 1,
	CELL_TOOLBAR = 2,
	CELL_WORKBOOK = 3,
	CELL_CELLS = 4,
	CELL_COLUMN = 5,
	CELL_ROW = 6,
	CELL_WORKBOOK_TABS = 7,
	CELL_CELLS = 8,
	CELL_WORKBOOK_TITLEBAR = 9,
	CELL_DESKTOP = 10,
	CELL_MODULE = 11,
	CELL_WATCH = 12,
	CELL_IMMEDIATE = 13,
	CELL_DEBUG = 14,
	OBJECT_DRAWN = 1,
	OBJECT_BUTTONS = 2,
	OBJECT_TEXT = 3,
	OBJECT_DIALOG = 4,
	CHART_SERIES = 1,
	CHART_TITLE = 2,
	CHART_PLOT = 3,
	CHART_CHART = 4,
	CHART_AXES = 5,
	CHART_GRIDLINES = 6,
	CHART_FLOOR = 7,
	CHART_LEGEND = 8,
};

struct Command : public OPER12 {
	using xcstr = const XCHAR*;
	/// command ref item
	struct Item : public OPER12 {
		using OPER12::operator[];
		Item(xcstr name, xcstr macro, xcstr text = 0)
			: OPER12(1,5)
		{
			operator[](0) = name;
			operator[](1) = macro;
			operator[](3) = text;
		}
	};
	Command(xcstr name, std::initializer_list<Item> ref)
		: OPER12(1,5)
	{
		operator[](0) = name;
		for (const auto& item : ref)
			this->push_down(ref);
	}
	Add(COMMAND bar, MENU menu, const OPER12& pos1, const OPER12& pos2)
	{ 
		operator[](0) = bar;
	}
};


} // xll namespace