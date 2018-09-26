// handle.h - Handles to C++ objects
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
//#include <memory>
#include <set>
#include <windows.h>
#include "XLCALL.H"

using HANDLEX = double;

namespace xll {

	// HANDLEX that defaults to NaN
	class handlex {
		HANDLEX h_;
	public:
		handlex()
			: h_(std::numeric_limits<double>::quiet_NaN())
		{ }
		operator HANDLEX()
		{
			return h_;
		}
		HANDLEX operator=(HANDLEX h)
		{
			return h_ = h;
		}
	};

	// Pointers to objects encoded as doubles.
	// Use first pointer allocated as a base offset
    // because 64-bit pointers are not always valid doubles.
	template<class T>
	class handle {
    public:
        using uptr = std::unique_ptr<T>;

        static T* base()
        {
#ifdef _WIN64
            static uptr p0(new T{});
#else
            static uptr p0(nullptr);
#endif

            return p0.get();
        }
        static HANDLEX p2h(T* p)
        {
            return static_cast<HANDLEX>(p - base());
        }
        // h = p0 - p, p = p0 - h
        static T* h2p(HANDLEX h)
        {
            static double intmax = ldexp(1, 53);
            ensure(h < intmax && h > -intmax);

            return base() + static_cast<ptrdiff_t>(h);
        }

        static std::set<uptr>& handles()
		{
			static std::set<uptr> handles_;

			return handles_;
		}

        static void insert(T* p)
		{
			auto& hs = handles();
            const auto& caller = Excel(xlfCaller);
			const auto& coerce = Excel(xlCoerce, caller);

            // Delete value in cell if it is an existing pointer.
			if (coerce.xltype == xltypeNum && coerce.val.num != 0)
			{
                double n = coerce.val.num;
                auto i = std::find_if(hs.begin(), hs.end(), [n](const uptr& h) { return n == p2h(h.get()); });
				if (i != hs.end()) {
					hs.erase(i);
				}
			}

            hs.insert(std::move(uptr(p)));
		}
		T* pt;
    public:
        handle(T* p)
            : pt(p)
        {
            insert(p);
        }
		handle(HANDLEX h)
		{
            //!!! check if h in handles
			pt = h2p(h);
		}
		handle(const handle&) = delete;
		handle& operator=(const handle&) = delete;
		~handle()
		{ }
        template<class U>
        bool operator==(const handle<U>& h) const
        {
            return pt == h.pt;
        }
		HANDLEX get() const
		{
			return p2h(pt);
		}
		operator HANDLEX()
		{
			return get();
		}
		T& operator*()
		{
			return *pt;
		}
		T* operator->()
		{
			return pt;
		}
		T* ptr()
		{
			return pt;
		}
    };
} // xll namespace
