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
        friend void swap(OPER12& a, OPER12& b) noexcept
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
        OPER12(OPER12&& o) noexcept
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

            xltype = xltypeNil;
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
            case xltypeStr: // case insensitive
                return val.str[0] == o.val.str[0] && 0 == _wcsnicmp(val.str + 1, o.val.str + 1, val.str[0]);
            case xltypeBool:
                return val.xbool == o.val.xbool;
            case xltypeRef:
                if (val.mref.idSheet != o.val.mref.idSheet)
                    return false;
                if (val.mref.lpmref->count != o.val.mref.lpmref->count)
                    return false;
                for (WORD i = 0; i < val.mref.lpmref->count; ++i)
                    if (val.mref.lpmref->reftbl[i] != o.val.mref.lpmref->reftbl[i])
                        return false;

                return true; 
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
                if (val.array.rows < o.val.array.rows)
                    return true;
                if (val.array.rows == o.val.array.rows && val.array.columns < o.val.array.columns)
                    return true;
                if (val.array.rows == o.val.array.rows && val.array.columns == o.val.array.columns) {
                    for (int i = 0; i < size(); ++i) {
                        if (operator[](i) < o[i])
                            return true;
                    }
                }
                return false;
            case xltypeMissing:
            case xltypeNil:
                return false;
            case xltypeSRef:
                return val.sref.ref < o.val.sref.ref;
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
        explicit OPER12(double num)
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
                return std::numeric_limits<double>::quiet_NaN();
//            if (type() == xltypeStr)
//                return val.str[0] != 0 && val.str[1] != 0;

            throw std::runtime_error("OPER12::operator double() only used for num, int, bool, and err");
        }
        int isNum() const
        {
            return type() == xltypeNum ? TRUE : FALSE;
        }

        // Str
        explicit OPER12(const wchar_t* str)
            : OPER12(str, str ? wcslen(str) : 0)
        {
        }
        template<int n>
        OPER12(const wchar_t(&str)[n])
            : OPER12(str, n - 1) // char arrays have termniating null
        { }
        OPER12(const wchar_t* str, size_t len)
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
        std::wstring toStr() const
        {
            ensure(isStr());

            return std::wstring(val.str + 1, val.str[0]);
        }
		/*
        const std::wstring_view to_string_view() const
        {
            return std::wstring_view(val.str + 1, val.str[0]);
        }
        std::wstring to_string() const
        {
            const std::wstring_view& sv = to_string_view();

            return std::wstring(sv.begin(), sv.end());
        }
		*/
        /*
        OPER12& operator=(const wchar_t* str)
        {
            return operator=(OPER12(str));
        }
        */
        ///  Append a string
        OPER12& append(const wchar_t* str, size_t len)
        {
            if (xltype == xltypeStr) {
                auto end = 1 + val.str[0];
                reallocate_str(val.str[0] + len);
                wmemcpy(val.str + end, str, len);
            }
            else if (xltype == xltypeNil || xltype == xltypeMissing) { /// xltype == OPER12().xltype
                xltype = xltypeStr;
                allocate_str(len);
                copy_str(str);
            }
            else {
                throw std::runtime_error("OPER12::operator&=: must be a string or default");
            }

            return *this;
        }
        OPER12& append(const std::wstring& str)
        {
            return append(str.c_str(), str.length());
        }
        OPER12& operator&=(const wchar_t* str)
        {
            return append(str, wcslen(str));
        }
        OPER12& operator&=(const std::wstring& str)
        {
            return append(str.data(), str.length());
        }
        OPER12& operator&=(const OPER12& o)
        {
            if (o.xltype & (xltypeMissing | xltypeNil)) {
                return *this;
            }

            ensure(o.xltype & xltypeStr);
            return append(o.val.str + 1, o.val.str[0]);
        }
        /*
        bool operator==(const wchar_t* str) const
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
        OPER12(int rw, int col)
        {
            allocate_multi(rw, col);
            uninitialized_fill_multi(OPER12{});
        }
        OPER12(std::initializer_list<OPER12> o)
            : OPER12(1, static_cast<int>(o.size()))
        {
            std::copy(o.begin(), o.end(), stdext::checked_array_iterator<OPER12*>(begin(), o.size()));
        }
        int isMulti() const
        {
            return type() == xltypeMulti ? TRUE : FALSE;
        }

        int rows() const
        {
            return type() == xltypeMulti ? val.array.rows : 1;
        }
        int columns() const
        {
            return type() == xltypeMulti ? val.array.columns : 1;
        }
        int size() const
        {
            switch (type()) {
                case xltypeMissing:
                case xltypeNil:
                    return 0;
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
        OPER12& operator[](int i)
        {
            if (type() != xltypeMulti) {
                ensure(i == 0);
                return *this;
            }

            return static_cast<OPER12&>(val.array.lparray[i]);
        }
        const OPER12& operator[](int i) const
        {
            if (type() != xltypeMulti) {
                ensure(i == 0);
                return *this;
            }

            return static_cast<const OPER12&>(val.array.lparray[i]);
        }
        OPER12& operator()(int i, int j)
        {
            auto n = i * columns() + j;
            return operator[](n);
        }
        const OPER12& operator()(int i, int j) const
        {
            auto n = i * columns() + j;
            return operator[](n);
        }

        OPER12& resize(int rw, int col)
        {
            if (rw == 0 || col == 0) {
                this->~OPER12();
                xltype = xltypeNil; // empty range
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
            if (size() == 0) {
                operator=(o);
            }
            else if (type() != xltypeMulti) {
                resize(1, 1);
                return push_back(o);
            }
            else {
                ensure(columns() == o.columns());
                int n = size();
                resize(rows() + o.rows(), columns());
                int n_ = size();
                for (int i = n; i < n_; ++i) {
                    int i_ = i - n;
                    operator[](i) = o[i_];
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
            xltype = xltypeInt; // just like Excel
            val.w = w;
        }
        /*
        explicit OPER12(int w)
        {
            xltype = xltypeNum; // just like Excel
            val.num = w;
        }
        */
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
            ensure(1 + len < std::numeric_limits<wchar_t>::max());
            val.str = static_cast<wchar_t*>(::malloc((1 + len) * sizeof(wchar_t)));
            ensure(val.str != nullptr);
            if (val.str)
                val.str[0] = static_cast<wchar_t>(len);
            xltype = xltypeStr;
        }
        void reallocate_str(size_t len)
        {
            ensure(xltype == xltypeStr);
            ensure(1 + len < std::numeric_limits<wchar_t>::max());
            val.str = static_cast<wchar_t*>(::realloc(val.str, (1 + len) * sizeof(wchar_t)));
            ensure(val.str);
            if (val.str)
                val.str[0] = static_cast<wchar_t>(len);
        }
        void copy_str(const wchar_t* str)
        {
            wmemcpy(val.str + 1, str, val.str[0]);
        }
        void deallocate_str()
        {
            ensure(xltype == xltypeStr);

            ::free(val.str);
        }
        void allocate_multi(int rw, int col)
        {
            // check rw, col size?
            auto size = rw * col;
            val.array.lparray = static_cast<XLOPER12*>(::calloc(size, sizeof(XLOPER12)));
            ensure(val.array.lparray);
            val.array.rows = rw;
            val.array.columns = col;
            xltype = xltypeMulti;
        }
        void reallocate_multi(int rw, int col)
        {
            ensure(xltype == xltypeMulti);
            auto size = rw * col;
            auto dsize = size - this->size();
            if (dsize > 0) {
                val.array.lparray = static_cast<XLOPER12*>(::realloc(val.array.lparray, size * sizeof(XLOPER12)));
                ensure(val.array.lparray);
                std::uninitialized_fill(val.array.lparray + this->size(), val.array.lparray + size, OPER12{});
            }
            if (dsize < 0) {
                for (auto pi = begin() + size; pi != end(); ++pi) {
                    pi->~OPER12();
                }
            }
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

            std::uninitialized_fill(begin(), end(), i);
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
