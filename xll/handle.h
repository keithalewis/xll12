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
		static std::set<HANDLEX>& handles()
		{
			static std::set<HANDLEX> handles_;

			return handles_;
		}

        static bool insert(HANDLEX h)
		{
			std::set<HANDLEX>& hs = handles();
			auto coerce = Excel(xlCoerce, Excel(xlfCaller));

            // Delete value in cell if it is an existing pointer.
			if (coerce.xltype == xltypeNum && coerce.val.num != 0)
			{
				auto i = hs.find(coerce.val.num);
				if (i != hs.end()) {
					delete h2p(*i);
					hs.erase(i);
				}
			}

			return hs.insert(h).second;
		}
	public:
        // base to offset from
        static T* base()
        {
            static std::unique_ptr<T> p0(new T{});
        
            return 0;//p0.get();
        }
        static HANDLEX p2h(T* p)
        {
            // ptrdiff_t dp = p - base();
            //return static_cast<HANDLEX>(dp);
            union { double d; T* p; } u;
            u.p = p;

            return u.d;
        }
        // h = p0 - p, p = p0 - h
        static T* h2p(HANDLEX h)
        {
            //ptrdiff_t dp = static_cast<ptrdiff_t>(h);
            //return base() + dp;
            union { double d; T* p; } u;
            u.d = h;

            return u.p;
        }
		T* pt;
		handle(T* pt)
			: pt(pt)
		{
			insert(p2h(pt));
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
        // Call this in Auto<Close>.
		static void gc()
		{
			for (auto& h : handles())
				delete h2p(h);
		}
    };
} // xll namespace
