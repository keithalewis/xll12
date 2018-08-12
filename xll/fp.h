// fp.h - Two dimensional array of doubles.
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <numeric>
#include <iterator>
#include <Windows.h>
#include "XLCALL.H"
#include "ensure.h"

namespace xll {

	// 1x1 NaN indicates empty array
	inline bool 
	is_empty(const _FP12& fp)
	{
		return fp.rows == 1
			&& fp.columns == 1
			&& _isnan(fp.array[0]);
	}

	inline RW
	rows(const _FP12& fp)
	{
		return is_empty(fp) ? 0 : fp.rows;
	}
	inline COL
	columns(const _FP12& fp)
	{
		return is_empty(fp) ? 0 : fp.columns;
	}
	inline INT32
	size(const _FP12& fp)
	{
		return is_empty(fp) ? 0 : fp.rows * fp.columns;
	}
	// cyclic index
	inline INT32 cyclic(INT32 i, INT32 n)
	{
		i = i % n;

		return i >= 0 ? i : i + n;
	}
	inline double&
	index(_FP12& fp, INT32 i)
	{
		return fp.array[cyclic(i, size(fp))];
	}
	inline const double&
	index(const _FP12& fp, INT32 i)
	{
		return fp.array[cyclic(i, size(fp))];
	}
	inline double&
	index(_FP12& fp, RW i, COL j)
	{
		return fp.array[cyclic(i, fp.rows)*fp.columns + cyclic(j, fp.columns)];
	}
	inline const double&
	index(const _FP12& fp, RW i, COL j)
	{
		return fp.array[cyclic(i, fp.rows)*fp.columns + cyclic(j, fp.columns)];
	}
	inline double*
	begin(_FP12& fp)
	{
		return &fp.array[0];
	}
	inline double*
	end(_FP12& fp)
	{
		return &fp.array[0] + size(fp);
	}

	inline const double*
	begin(const _FP12& fp)
	{
		return &fp.array[0];
	}
	inline const double*
	end(const _FP12& fp)
	{
		return &fp.array[0] + size(fp);
	}

	class FP12 {
	public:
		FP12()
			: buf(0)
		{
			realloc(0,0);
		}
		FP12(RW r, COL c) 
			: buf(0) 
		{
			realloc(r, c);
		}
		FP12(const FP12& x) 
			: buf(0)
		{
			realloc(x.rows(), x.columns());
			copy(x.get());
		}
		FP12(const _FP12& x)
			: buf(0)
		{
			realloc(x.rows, x.columns);
			copy(&x);
		}
		FP12(std::initializer_list<double> a)
			: buf(0)
		{
			realloc(1, static_cast<COL>(a.size()));
			copy(a.begin());
		}
		FP12(std::initializer_list<std::initializer_list<double>> a)
			: FP12()
		{
			for (const auto& r : a) {
				if (!is_empty() && columns() < static_cast<INT32>(r.size()))
					resize(rows(), static_cast<COL>(r.size()));
				push_down(r.begin(), r.end());
			}
		}
		FP12& operator=(const FP12& x)
		{
			if (&x != this) {
				realloc(x.rows(), x.columns());
			    copy(x.get());
			}

			return *this;
		}
		FP12& operator=(const _FP12& x)
		{
			realloc(x.rows, x.columns);
			copy(x.array);

			return *this;
		}
		~FP12()
		{
			free(buf);
		}

		bool operator==(const _FP12& x) const
		{
			auto size = x.rows * x.columns;
			return rows() == x.rows && columns() == x.columns
				&& std::equal(begin(), end(), stdext::checked_array_iterator<const double*>(&x.array[0], size));
		}
		bool operator==(const FP12& x) const
		{
            return operator==(*x.get());
        }
		bool operator!=(const _FP12& x) const
		{
			return !operator==(x);
		}
		bool operator!=(const FP12& x) const
		{
			return !operator==(x);
		}
		_FP12* get(void)
		{
            return reinterpret_cast<_FP12*>(buf);
        }
		// use when returning to Excel
		const _FP12* get(void) const
		{
            return reinterpret_cast<_FP12*>(buf);
        }
		double* array()
		{
			return get()->array;
		}
		const double* array() const
		{
			return get()->array;
		}
		RW rows() const
		{
			return get()->rows;
		}
		COL columns() const
		{
			return get()->columns;
		}
		INT32 size() const
		{
			return buf ? (is_empty() ? 0 : get()->rows * get()->columns) : 0;
		}

		void resize(RW r, COL c = 1)
		{
			realloc(r, c);
		}
		const double& operator[](INT32 i) const
		{
			return get()->array[i];
		}
		double& operator[](INT32 i)
		{
			return get()->array[i];
		}
		const double& operator()(RW i, COL j) const
		{
			return xll::index(*get(), i, j);
		}
		double& operator()(RW i, COL j)
		{
			return xll::index(*get(), i, j);
		}
		// cyclic index
		const double& index(INT32 i) const
		{
			return xll::index(*get(), i);
		}
		double& index(INT32 i)
		{
			return xll::index(*get(), i);
		}
		const double& index(RW i, COL j) const
		{
			return xll::index(*get(), i, j);
		}
		double& index(RW i, COL j)
		{
			return xll::index(*get(), i, j);
		}

		double* begin()
		{
			return get()->array;
		}
		const double* begin() const
		{
			return get()->array;
		}
		double* end()
		{
			return get()->array + size();
		}
		const double* end() const
		{
			return get()->array + size();
		}
		template<class T>
		FP12& push_back(const T& t)
		{
			return push_back(&t, &t + 1);
		}
		// favor columns
		template<class I>
		FP12& push_back(I b, I e)
		{
			INT32 n = static_cast<INT32>(std::distance(b, e));

			if (is_empty()) {
				resize(1, n);
			}
			else if (rows() == 1) {
				resize(1, columns() + n);
			}
			else if (columns() == 1) {
				resize(rows() + n, 1);
			}
			else  {
				ensure (columns() == n);

				resize(rows() + 1, n);
			}

			std::copy(b, e, stdext::checked_array_iterator<double*>(end() - n, n));

			return *this;
		}
		FP12& push_back(std::initializer_list<double> a)
		{
			return push_back(a.begin(), a.end());
		}
		template<class I>
		FP12& push_down(I b, I e)
		{
			INT32 n = static_cast<INT32>(std::distance(b, e));

			if (is_empty()) {
				resize(n, 1);
			}
			else if (columns() == 1) {
				resize(rows() + n, 1);
			}
			else {
				ensure (columns() == n);
				resize(rows() + 1, n);
			}

			std::copy(b, e, stdext::checked_array_iterator<double*>(end() - n, n));

			return *this;
		}
		FP12& push_down(std::initializer_list<double> a)
		{
			return push_down(a.begin(), a.end());
		}

		// buffer n rows
		template<class I>
		FP12& buffer(I b, I e, int n)
		{
			INT32 c = static_cast<INT32>(std::distance(b, e));

			if (is_empty() || n == -1) {
				push_back(b, e, true);
			}
			// push_front
			else if (n == 0) {
				INT32 sz = size();
				ensure (c == columns());
				resize(rows() + 1, columns());
				memmove(begin() + c, get()->array, sz*sizeof(double));
				memcpy(begin(), b, c*sizeof(double));
			}
			else if (n < -1) {
				if (rows() == static_cast<INT32>(-n)) {
					memmove(begin(), begin() + c, (size() - c)*sizeof(double));
					memcpy(end() - c, b, c*sizeof(double));
				}
				else {
					push_back(b, e, true);
				}
			}
			else {
				ensure (n > 0);
				if (rows() == static_cast<INT32>(n)) {
					memmove(begin() + c, begin(), (size() - c)*sizeof(double));
					memcpy(begin(), b, c*sizeof(double));
				}
				else {
					INT32 sz = size();
					resize(rows() + 1, columns());
					memmove(begin() + c, begin(), sz*sizeof(double));
					memcpy(begin(), b, c*sizeof(double));
				}
			}

			return *this;
		}

		bool is_empty(void) const
		{
			return xll::is_empty(*get());
		}
	private:
		void copy(const double* p)
		{
			get()->array[0] = p[0]; // could be empty array
			for (auto i = 1; i < size(); ++i)
				get()->array[i] = p[i];
		}
		void copy(const _FP12* p)
		{
			ensure (get()->rows == p->rows);
			ensure (get()->columns == p->columns);

			copy(p->array);
		}
		void realloc(RW r, COL c)
		{
			if (!buf || size() != r*c) {
				auto size = r * c;
				buf = static_cast<char*>(::realloc(buf, sizeof(_FP12) + size*sizeof(double)));
				ensure (buf != 0);
			}
//			memset(buf, 0, sizeof(_FP12) + r*c*sizeof(double));
			// check size
			get()->rows = r;
			get()->columns = c;

			// empty array
			if (r*c == 0) {
				get()->rows = 1;
				get()->columns = 1;
				get()->array[0] = std::numeric_limits<double>::quiet_NaN();
			}
			else if (is_empty()) {
				get()->array[0] = 0; // so resize(1,1) on empty array is not empty
			}
		}
		char* buf;
	};

} // namespace xll

inline auto begin(const xll::FP12& a) { return a.begin(); }
inline auto begin(xll::FP12& a) { return a.begin(); }
inline auto end(const xll::FP12& a) { return a.end(); }
inline auto end(xll::FP12& a) { return a.end(); }
inline auto index(const xll::FP12& a, INT32 i) { return a.index(i); }
inline auto index(xll::FP12& a, INT32 i) { return a.index(i); }
inline auto index(const xll::FP12& a, INT32 i, INT32 j) { return a.index(i, j); }
inline auto index(xll::FP12& a, INT32 i, INT32 j) { return a.index(i, j); }
