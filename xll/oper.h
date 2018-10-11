// oper.h - C++ wrapper for XLOPER's
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include "ensure.h"
#define NOMINMAX
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "XLCALL.H"
#include "ref.h"
#include <malloc.h>
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <map>
#include <memory>

enum class xlerr {
    Null = xlerrNull,
    Div0 = xlerrDiv0,
    Value = xlerrValue,
    Ref = xlerrRef,
    Name = xlerrName,
    Num = xlerrNum,
    NA = xlerrNA,
    GettingData = xlerrGettingData
};

/*!!! not yet supported
inline const std::map<int, const wchar_t*> xlerr_message = {
  {xlretSuccess,       L"success"},
  {xlretAbort,         L"macro halted"},
  {xlretInvXlfn,       L"invalid function number"},
  {xlretInvCount,      L"invalid number of arguments"},
  {xlretInvXloper,     L"invalid OPER structure"},
  {xlretStackOvfl,     L"stack overflow"},
  {xlretFailed,        L"command failed"},
  {xlretUncalced,      L"uncalced cell"},
  {xlretNotThreadSafe, L"not allowed during multi-threaded calc"},
  {xlretInvAsynchronousContext, L"invalid asynchronous function handle"},
  {xlretNotClusterSafe,L"not supported on cluster"},
};
*/

enum class xltype {
    Num = xltypeNum,        // IEEE 64-bit floating point
    Str = xltypeStr,        // Counted string
    Bool = xltypeBool,      // Boolean value
    Ref = xltypeRef,        // Reference to multiple ranges
    Err = xltypeErr,        // Excel error type
    Flow = xltypeFlow,      // not used
    Multi = xltypeMulti,    // A range of OPERs
    Missing = xltypeMissing,// Argument not specified
    Nil = xltypeNil,        // Empty data type
    SRef = xltypeSRef,      // Single reference to a range
    Int = xltypeInt,        // Integer type that never occurs
    BigData = xltypeBigData,// Binary data
};

namespace xll {

    /// <summary>
    /// Value type that represents an Excel cell or range.
    /// </summary>
    struct OPER12 : public XLOPER12
    {
        friend void swap(OPER12& a, OPER12& b)
        {
            using std::swap;

            swap(a.xltype, b.xltype);
            swap(a.val, b.val);
        }

        // Strip out if xlFree or xlAutoFree is involved.
        int type() const
        {
            return xltype & ~(xlbitXLFree | xlbitDLLFree);
        }

        int isScalar() const
        {
            return xltype & (xltypeNum | xltypeBool | xltypeErr | xltypeSRef | xltypeInt | xltypeNil | xltypeMissing);
        }

        int isMissing() const
        {
            return type() == xltypeMissing ? TRUE : FALSE;
        }
        int isNil() const
        {
            return type() == xltypeNil ? TRUE : FALSE;
        }

        OPER12(::xltype type = xltype::Missing)
        {
            xltype = static_cast<DWORD>(type);
        }
        OPER12(const XLOPER12& o)
        {
            xltype = o.xltype;

            if (xltype == xltypeStr) {
                allocate_str(o.val.str[0]);
                copy_str(o.val.str + 1);
            }
            else if (xltype == xltypeMulti) {
                allocate_multi(o.val.array.rows, o.val.array.columns);
                std::copy(o.val.array.lparray, o.val.array.lparray + size(), stdext::checked_array_iterator<OPER12*>(begin(), size()));
            }
            else if (xltype == xltypeBigData) {
                ensure(!"not implemented");
            }
            else {
                val = o.val;
            }
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
                std::copy(o.begin(), o.end(), stdext::checked_array_iterator<OPER12*>(begin(), o.size()));
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

            o.xltype = xltypeNil;
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
                // permit int/double comparison
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
        bool operator<(const OPER12&o) const
        {
            if (type() != o.type())
                return type() < o.type();
            switch (type()) {
            case xltypeNum:
                return val.num < o.val.num;
            case xltypeStr:
                return std::lexicographical_compare(val.str + 1, val.str + 1 + val.str[0], o.val.str + 1, o.val.str + 1 + o.val.str[0]);
            case xltypeBool:
                return val.xbool < o.val.xbool;
            case xltypeRef:
                return false; // !!! for now
            case xltypeErr:
                return val.err < o.val.err;
            case xltypeMulti:
                return false; // !!! for now
            case xltypeMissing:
            case xltypeNil:
                return false;
            case xltypeSRef:
                if (val.sref.ref.rwFirst < o.val.sref.ref.rwFirst)
                    return true;
                if (val.sref.ref.rwFirst == o.val.sref.ref.rwFirst
                    && val.sref.ref.rwLast < o.val.sref.ref.rwLast)
                    return true;
                if (val.sref.ref.rwFirst == o.val.sref.ref.rwFirst
                    && val.sref.ref.rwLast == o.val.sref.ref.rwLast
                    && val.sref.ref.colFirst < o.val.sref.ref.colFirst)
                    return true;
                if (val.sref.ref.rwFirst == o.val.sref.ref.rwFirst
                    && val.sref.ref.rwLast == o.val.sref.ref.rwLast
                    && val.sref.ref.colFirst == o.val.sref.ref.colFirst
                    && val.sref.ref.colLast < o.val.sref.ref.colLast)
                    return true;

                return false;
            case xltypeInt:
                return val.w < o.val.w;
            }

            return false; // throw???
        }

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
            if (type() == xltypeNum)
                return val.num;
            if (type() == xltypeInt)
                return val.w;
            if (type() == xltypeBool)
                return val.xbool;
            if (type() == xltypeErr)
                return false;
            if (type() == xltypeStr)
                return val.str[0] != 0 && val.str[1] != 0;

            throw std::runtime_error("OPER12::operator double() only used for num, int, bool, and err");
        }
        int isNum() const
        {
            return type() == xltypeNum ? TRUE : FALSE;
        }

        // Str
        explicit OPER12(const XCHAR* str)
            : OPER12(str, str ? wcslen(str) : 0)
        {
        }
        template<int n>
        OPER12(const XCHAR(&str)[n])
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
        OPER12(const std::wstring& str)
            : OPER12(str.data(), str.length())
        { }
        int isStr() const
        {
            return type() == xltypeStr ? TRUE : FALSE;
        }
        const std::wstring_view to_string_view() const
        {
            return std::wstring_view(val.str + 1, val.str[0]);
        }
        std::wstring to_string() const
        {
            const std::wstring_view& sv = to_string_view();

            return std::wstring(sv.begin(), sv.end());
        }
        /*
        OPER12& operator=(const XCHAR* str)
        {
            return operator=(OPER12(str));
        }
        */
        ///  Append a string
        OPER12& append(const XCHAR* str, size_t len = 0)
        {
            if (xltype == xltypeStr) {
                if (!len)
                    len = wcslen(str);
                auto end = 1 + val.str[0];
                reallocate_str(val.str[0] + len);
                wmemcpy(val.str + end, str, len);
            }
            else if (xltype == xltypeNil || xltype == xltypeMissing) { /// xltype == OPER12().xltype
                operator=(str);
            }
            else {
                throw std::runtime_error("OPER12::operator&=: must be a string or default");
            }

            return *this;
        }
        OPER12& operator&=(const XCHAR* str)
        {
            return append(str);
        }
        OPER12& operator&=(const std::wstring& str)
        {
            return append(str.data(), str.length());
        }
        OPER12& operator&=(const OPER12& o)
        {
            ensure(o.xltype & xltypeStr);
            return append(o.val.str + 1, o.val.str[0]);
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
        int isBool() const
        {
            return type() == xltypeBool ? TRUE : FALSE;
        }

        // Ref
        /*
        explicit OPER12(const XLMREF12& ref)
        { }
        */
        // Err
        explicit OPER12(const xlerr& err)
        {
            xltype = xltypeErr;
            val.err = static_cast<int>(err);
        }
        int isErr() const
        {
            return type() == xltypeErr ? TRUE : FALSE;
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
            std::copy(o.begin(), o.end(), stdext::checked_array_iterator<OPER12*>(begin(), o.size()));
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
                std::copy(r.begin(), r.end(), stdext::checked_array_iterator<OPER12*>(begin() + i * cols, r.size()));
                ++i;
            }
        }
        int isMulti() const
        {
            return type() == xltypeMulti ? TRUE : FALSE;
        }

        RW rows() const
        {
            return type() == xltypeMulti ? val.array.rows : 1;
        }
        COL columns() const
        {
            return type() == xltypeMulti ? val.array.columns : 1;
        }
        int size() const
        {
            switch (type()) {
                case xltypeMissing:
                case xltypeNil:
                    return 0;
                case xltypeStr:
                    return val.str[0];
                default:
                    return rows() * columns();
            }
        }
        OPER12* begin() // xltypeStr???
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
                ensure(i == 0);
                return *this;
            }

            return static_cast<OPER12&>(val.array.lparray[i]);
        }
        const OPER12& operator[](size_t i) const
        {
            if (type() != xltypeMulti) {
                ensure(i == 0);
                return *this;
            }

            return static_cast<const OPER12&>(val.array.lparray[i]);
        }
        OPER12& operator()(RW i, COL j)
        {
            auto n = i * columns() + j;
            return operator[](n);
        }
        const OPER12& operator()(RW i, COL j) const
        {
            auto n = i * columns() + j;
            return operator[](n);
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
                operator[](0) = o;
                return *this;
            }
            else {
                auto size = rows() * columns();

                if (rows() == 1) {
                    ensure(o.rows() == 1);
                    resize(1, columns() + o.rows() * o.columns());
                }
                else if (columns() == 1) {
                    ensure(o.columns() == 1);
                    resize(rows() + o.rows(), 1);
                }
                else {
                    ensure(columns() == o.columns());
                    resize(rows() + o.rows(), columns());
                }
                for (auto i = size; i < this->size(); ++i) {
                    auto i_size = i - size;
                    operator[](i) = o[i_size];
                }
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
        int isSRef() const
        {
            return type() == xltypeSRef ? TRUE : FALSE;
        }

        // Int
        explicit OPER12(int w)
        {
            xltype = xltypeNum; // just like Excel
            val.num = w;
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
        int isInt() const
        {
            return type() == xltypeInt ? TRUE : FALSE;
        }

    private:
        void allocate_str(size_t len)
        {
            ensure(1 + len < std::numeric_limits<XCHAR>::max());
            val.str = static_cast<XCHAR*>(::malloc((1 + len) * sizeof(XCHAR)));
            ensure(val.str != nullptr);
            if (val.str)
                val.str[0] = static_cast<XCHAR>(len);
            xltype = xltypeStr;
        }
        void reallocate_str(size_t len)
        {
            ensure(xltype == xltypeStr);
            ensure(1 + len < std::numeric_limits<XCHAR>::max());
            val.str = static_cast<XCHAR*>(::realloc(val.str, (1 + len) * sizeof(XCHAR)));
            ensure(val.str);
            if (val.str)
                val.str[0] = static_cast<XCHAR>(len);
        }
        void copy_str(const XCHAR* str)
        {
            wmemcpy(val.str + 1, str, val.str[0]);
        }
        void deallocate_str()
        {
            ensure(xltype == xltypeStr);

            ::free(val.str);
        }
        void allocate_multi(RW rw, COL col)
        {
            // check rw, col size?
            auto size = rw * col;
            val.array.lparray = static_cast<XLOPER12*>(::calloc(size, sizeof(XLOPER12)));
            ensure(val.array.lparray);
            val.array.rows = rw;
            val.array.columns = col;
            xltype = xltypeMulti;
        }
        void reallocate_multi(RW rw, COL col)
        {
            ensure(xltype == xltypeMulti);
            auto size = rw * col;
            if (this->size() < size) {
                val.array.lparray = static_cast<XLOPER12*>(::realloc(val.array.lparray, size * sizeof(XLOPER12)));
                ensure(val.array.lparray);
            }
            for (auto i = this->size(); i < size; ++i)
                new (static_cast<void*>(val.array.lparray + i)) OPER12{};
            val.array.rows = rw;
            val.array.columns = col;
        }
        void uninitialized_copy_multi(const XLOPER12* i)
        {
            ensure(xltype == xltypeMulti);

            for (auto& o : *this) {
                new (static_cast<void*>(&o)) XLOPER12(*i++);
            }
        }
        void uninitialized_fill_multi(const OPER12& i)
        {
            ensure(xltype == xltypeMulti);

            for (auto& o : *this) {
                new (static_cast<void*>(&o)) OPER12(i);
            }
        }
        void destroy_multi()
        {
            ensure(xltype == xltypeMulti);

            for (auto& o : *this)
                o.~OPER12();
        }
        void deallocate_multi()
        {
            ensure(xltype == xltypeMulti);

            ::free(val.array.lparray);
        }
    };

    // No support for Excel 2003 and earlier.
    using OPER = OPER12;
    using LPOPER12 = OPER12 * ;
    using LPOPER = LPOPER12;

} // xll
