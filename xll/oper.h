// oper.h - C++ wrapper for XLOPER's
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include "ensure.h"
#include "XLCALL.H"
#include <malloc.h>
#include <algorithm>
#include <initializer_list>
#include <limits>
#include <memory>

enum class xlerr {
	Null  = xlerrNull,
	Div0  = xlerrDiv0,
	Value = xlerrValue,
	Ref   = xlerrRef,
	Name  = xlerrName,
	Num   = xlerrNum,
	NA    = xlerrNA,
	GettingData = xlerrGettingData
};

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
	/// <remarks>
	/// The constructor uses <c>height</c> and <c>width</c> instead of <c>rw/colFirst/Last</c>
	/// </remarks>
	class REF12 : public XLREF12 {
	public:
		// default is A1
		REF12(RW rw = 0, COL col = 0, RW height = 1, COL width = 1)
			: XLREF12{rw, rw + height - 1, col, col + width - 1}
		{ }
		// translate by rw, col
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
	inline REF12 move(REF12 r, RW rw = 0, COL col = 0)
	{
		return r.move(rw, col);
	}
		
	/// 
	struct OPER12 : public XLOPER12 
	{
		friend void swap(OPER12& a, OPER12& b)
		{
			std::swap(a.xltype, b.xltype);
			std::swap(a.val, b.val);
		}

		// Strip out if xlFree or xlAutoFree is involved.
		int type() const
		{
			return xltype&~(xlbitXLFree|xlbitDLLFree);
		}

		OPER12()
		{
			xltype = xltypeMissing;
		}
		OPER12(const OPER12& o)
		{
			xltype = o.type();

			if (xltype == xltypeStr) {
				allocate_str(o.val.str[0]);
				copy_str(o.val.str + 1);
			}
			else if (xltype == xltypeMulti) {
				allocate_multi(o.val.array.rows, o.val.array.columns);
				std::copy(o.begin(), o.end(), begin());
			}
			else if (xltype == xltypeBigData) {
				ensure(!"not implemented");
			}
			else {
				val = o.val;
			}
		}
		OPER12(OPER12&& o)
		{
			xltype = o.xltype;
			val = o.val;

			o.xltype = xltypeMissing;
		}
		OPER12& operator=(OPER12 o)
		{
			swap(*this, o);

			return *this;
		}
		~OPER12()
		{
			if (xltype & xlbitXLFree) {
				Excel12(xlFree, nullptr, 1, this);
			}
			else if (xltype & xlbitDLLFree) {
				// deleted by xlAutoFree112()
			}
			else if (xltype == xltypeStr) {
				deallocate_str();
			}
			else if (xltype == xltypeMulti) {
				destroy_multi();
				deallocate_multi();
			}
		}

		bool operator==(const OPER12& o) const
		{
			if (type() != o.type()) {
				if (type() == xltypeNum && o.type() == xltypeInt)
					return val.num == o.val.w;
				if (type() == xltypeInt && o.type() == xltypeNum)
					return val.w == o.val.num;

				return false;
			}

			switch (type()) {
			case xltypeNum:
				return val.num == o.val.num;
			case xltypeStr:
				return val.str[0] == o.val.str[0] && 0 == _wcsnicmp(val.str + 1, o.val.str + 1, val.str[0]);
			case xltypeBool:
				return val.xbool == o.val.xbool;
			case xltypeRef:
				return false; //! compare sub references
			case xltypeErr:
				return val.err == o.val.err; //? false like NaN
			case xltypeFlow:
				return false; // ancient macro programming artifact
			case xltypeMulti:
				return rows() == o.rows()
					&& columns() == o.columns()
					&& std::equal(begin(), end(), o.begin(), o.end());
			case xltypeMissing:
			case xltypeNil:
				return true;
			case xltypeSRef:
				return val.sref.ref == o.val.sref.ref;
			case xltypeInt:
				return val.w == o.val.w;
			case xltypeBigData:
				return false; //? check val.bigdata.lpbData
			}

			return false;
		}
		bool operator!=(const OPER12& o) const
		{
			return !operator==(o);
		}
		// bool operator<(...)

		template<class T>
		OPER12& operator=(const T& t)
		{
			operator=(OPER12(t));

			return *this;
		}
		template<class T>
		bool operator==(const T& t) const
		{
			return operator==(OPER12(t));
		}
		template<class T>
		bool operator!=(const T& t) const
		{
			return !operator==(OPER12(t));
		}

		// Num
		explicit OPER12(const double& num)
		{
			xltype = xltypeNum;
			val.num = num;
		}
		/*
		OPER12& operator=(const double& num)
		{
			return *this = OPER12(num);
		}
		
		bool operator==(const double& num) const
		{
			return type() == xltypeNum && val.num == num
				|| type() == xltypeInt && val.w == num;
		}
		*/

		operator double() const
		{
			ensure (type() == xltypeNum || type() == xltypeInt);

			return type() == xltypeNum ? val.num : val.w;
		}

		// Str
		explicit OPER12(const XCHAR* str)
			: OPER12(str, str ? wcslen(str) : 0)
		{ }
		template<int n>
		OPER12(const XCHAR (&str)[n])
			: OPER12(str, n - 1) // char arrays have termniating null
		{ }
		OPER12(const XCHAR* str, size_t len)
		{
			if (!str) {
				xltype = xltypeMissing;
			}
			else {
				xltype = xltypeStr;
				allocate_str(len);
				copy_str(str);
			}
		}
		/*
		OPER12& operator=(const XCHAR* str)
		{
			return operator=(OPER12(str));
		}
		*/
		// append
		OPER12& operator&=(const XCHAR* str)
		{
			size_t origLen = val.str[0];
			size_t len = wcslen(str);			
			reallocate_str(len);
			XCHAR* end = val.str + 1 + origLen;
			wmemcpy(end, str, len);

			return *this;
		}
		/*
		bool operator==(const XCHAR* str) const
		{
			return 0 == wcsncmp(str, val.str + 1, val.str[0]);
		}
		*/
		// Bool
		explicit OPER12(const bool& xbool)
		{
			xltype = xltypeBool;
			val.xbool = xbool;
		}
		/*
		OPER12& operator=(const bool& xbool)
		{
			return operator=(OPER12(xbool));
		}
		*/

		// Ref

		// Err
		explicit OPER12(const xlerr& err)
		{
			xltype = xltypeErr;
			val.err = static_cast<int>(err);
		}

		// Multi
		OPER12(RW rw, COL col)
		{
			allocate_multi(rw, col);
			uninitialized_fill_multi(OPER12());
		}
		OPER12(std::initializer_list<OPER12> o)
			: OPER12(1, static_cast<COL>(o.size()))
		{
			std::copy(o.begin(), o.end(), begin());
		}
		OPER12(std::initializer_list<std::initializer_list<OPER12>> o)
			: OPER12(static_cast<RW>(o.size()), static_cast<COL>(o.begin()->size()))
		{
			size_t cols = columns();
			for (const auto& r : o)
				if (r.size() > cols)
					cols = r.size();
			if (cols > static_cast<size_t>(columns()))
				resize(rows(), static_cast<COL>(cols));

			size_t i = 0;
			for (const auto& r : o) {
				std::copy(r.begin(), r.end(), begin() + i*cols);
				++i;
			}
		}
		RW rows() const
		{
			return type() == xltypeMulti ? val.array.rows : 1;
		}
		COL columns() const
		{
			return type() == xltypeMulti ? val.array.columns : 1;
		}
		auto size() const
		{
			return rows() * columns();
		}
		OPER12* begin()
		{
			return type() == xltypeMulti ? static_cast<OPER12*>(val.array.lparray) : this;
		}
		const OPER12* begin() const
		{
			return type() == xltypeMulti ? static_cast<const OPER12*>(val.array.lparray) : this;
		}
		OPER12* end()
		{
			return type() == xltypeMulti ? static_cast<OPER12*>(val.array.lparray + size()) : this + 1;
		}
		const OPER12* end() const
		{
			return type() == xltypeMulti ? static_cast<const OPER12*>(val.array.lparray + size()) : this + 1;
		}
		OPER12& operator[](size_t i)
		{
			if (type() != xltypeMulti) {
				ensure (i == 0);
				return *this;
			}

			return static_cast<OPER12&>(val.array.lparray[i]);
		}
		const OPER12& operator[](size_t i) const
		{
			if (type() != xltypeMulti) {
				ensure (i == 0);
				return *this;
			}

			return static_cast<const OPER12&>(val.array.lparray[i]);
		}
		OPER12& operator()(RW i, COL j)
		{
			return operator[](i*columns() + j);
		}
		const OPER12& operator()(RW i, COL j) const
		{
			return operator[](i*columns() + j);
		}

		OPER12& resize(RW rw, COL col)
		{
			if (rw == 0 || col == 0) {
				this->~OPER12();
				operator=(OPER12{});
			}
			else if (type() == xltypeMulti) {
				reallocate_multi(rw, col);
			}
			else {
				OPER12 this_(*this);
				operator=(OPER12(rw, col));
				operator[](0) = this_;
			}

			return *this;
		}

		OPER12& push_back(const OPER12& o)
		{
			if (type() != xltypeMulti) {
				resize(1, 1);
				return push_back(o);
			}
			else {
				auto size = this->size();

				if (rows() == 1) {
					ensure (o.rows() == 1);
					resize(1, columns() + o.size());
				}
				else if (columns() == 1) {
					ensure (o.columns() == 1);
					resize(rows() + o.rows(), 1);
				}
				else {
					ensure (columns() == o.columns());
					resize(rows() + o.rows(), columns());
				}
				for (auto i = size; i < this->size(); ++i)
					operator[](i) = o[i - size];
			}

			return *this;
		}

		// Missing

		// SRef
		explicit OPER12(const XLREF12& ref)
		{
			xltype = xltypeSRef;
			val.sref.ref = ref;
		}

		// Int
		explicit OPER12(const int& w)
		{
			xltype = xltypeInt;
			val.w = w;
		}
		/*
		OPER12& operator=(const int& w)
		{
			xltype = xltypeInt;
			val.w = w;

			return *this;
		}
		
		bool operator==(const int& w) const
		{
			return type() == xltypeInt && val.w == w
				|| type() == xltypeNum && val.num == w;
		}
		*/
	private:
		void allocate_str(size_t len)
		{
			ensure (len < std::numeric_limits<XCHAR>::max());
			val.str = static_cast<XCHAR*>(::malloc((1 + len)*sizeof(XCHAR)));
			ensure (val.str);
			if (val.str) {
				val.str[0] = static_cast<XCHAR>(len);
				xltype = xltypeStr;
			}
		}
		void reallocate_str(size_t len)
		{
			ensure (xltype == xltypeStr);
			len += val.str[0];
			ensure (len < std::numeric_limits<XCHAR>::max());
			val.str = static_cast<XCHAR*>(::realloc(val.str, (1 + len)*sizeof(XCHAR)));
			ensure (val.str);
			if (val.str) {
				val.str[0] = static_cast<XCHAR>(len);
			}
		}
		void copy_str(const XCHAR* str)
		{
			wcsncpy(val.str + 1, str, val.str[0]);
		}
		void deallocate_str()
		{
			ensure (xltype == xltypeStr);

			::free(val.str);
		}
		void allocate_multi(RW rw, COL col)
		{
			// check rw, col size?
			val.array.lparray = static_cast<XLOPER12*>(::malloc(rw*col*sizeof(XLOPER12)));
			ensure (val.array.lparray);
			val.array.rows = rw;
			val.array.columns = col;
			xltype = xltypeMulti;
		}
		void reallocate_multi(RW rw, COL col)
		{
			ensure (xltype == xltypeMulti);
			auto size = rw*col;
			if (this->size() < size) {
				val.array.lparray = static_cast<XLOPER12*>(::realloc(val.array.lparray, size*sizeof(XLOPER12)));
				ensure (val.array.lparray);
			}
			for (auto i = this->size(); i < size; ++i)
				new (static_cast<void*>(val.array.lparray + i)) OPER12{};
			val.array.rows = rw;
			val.array.columns = col;
		}
		void uninitialized_copy_multi(const XLOPER12* i)
		{
			ensure (xltype == xltypeMulti);

			for (auto& o : *this) {
				new (static_cast<void*>(&o)) XLOPER12(*i++);
			}
		}
		void uninitialized_fill_multi(const OPER12& i)
		{
			ensure (xltype == xltypeMulti);

			for (auto& o : *this) {
				new (static_cast<void*>(&o)) OPER12(i);
			}
		}
		void destroy_multi()
		{
			ensure (xltype == xltypeMulti);
		
			for (auto& o : *this)
				o.~OPER12();
		}
		void deallocate_multi()
		{
			ensure (xltype == xltypeMulti);

			::free(val.array.lparray);
		}
	};

} // xll

/*
inline bool operator==(const xll::OPER12& a, const xll::OPER12& b)
{
	return a.operator==(b);
}
inline bool operator<(const xll::OPER12& a, const xll::OPER12& b)
{
	return a.operator<(b);
}
*/