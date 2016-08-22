// ref.h - XLREF12 wrapper
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include <Windows.h>
#include "XLCALL.H"

inline bool operator==(const XLREF12& r, const XLREF12& s)
{
	return r.colFirst == s.colFirst
		&& r.colLast == s.colLast
		&& r.rwFirst == s.rwFirst
		&& r.rwLast == s.rwLast;
}
// strict weak ordering
inline bool operator<(const XLREF12& r, const XLREF12& s)
{
	return r.colFirst < s.colFirst
		|| r.colFirst == s.colFirst && r.colLast < s.colLast
		|| r.colFirst == s.colFirst && r.colLast == s.colLast && r.rwFirst > s.rwFirst
		|| r.colFirst == s.colFirst && r.colLast == s.colLast && r.rwFirst == s.rwFirst && r.rwLast < s.rwLast;
}

namespace xll {

	/// <summary>
	/// Wrapper for XLREF12 class
	/// </summary>
	/// The <c>XLREF12</c> class represents a reference to a two dimensional
	/// range of cells.
	/// <remarks>
	/// The constructor uses <c>height</c> and <c>width</c> instead of <c>rw/colLast</c>
	/// </remarks>
	class REF12 : public XLREF12 {
	public:
		/// Construct a reference to a two dimensional range.
		REF12(RW rw = 0, COL col = 0, RW height = 1, COL width = 1)
			: XLREF12{rw, rw + height - 1, col, col + width - 1}
		{ }
		/// Translate a reference by rw, col
		REF12& move(RW rw, COL col = 0)
		{
			rwFirst  += rw;
			rwLast   += rw;
			colFirst += col;
			colLast  += col;

			return *this;
		}
		REF12& up(RW rw = 1) { return move(-rw, 0); }
		REF12& down(RW rw = 1) { return move(rw, 0); }
		REF12& left(COL col = 1) { return move(0, -col); }
		REF12& right(COL col = 1) { return move(0, col); }
	};
	/// Return a reference translated by <c>rw</c> and <c>col</c>.
	inline REF12 move(REF12 r, RW rw = 0, COL col = 0)
	{
		return r.move(rw, col);
	}

	// No support for Excel 2003 and earlier.
	using REF = REF12;

} // xll