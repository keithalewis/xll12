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
	// Assumes high 32 bits are constant.
	template<class T>
	class handle {
		static std::set<HANDLEX>& handles()
		{
			static std::set<HANDLEX> handles_;

			return handles_;
		}
		static void gc()
		{
			for (auto& h : handles())
				delete h.ptr();
		}

        static bool insert(HANDLEX h)
		{
			auto hs{handles()};
			auto coerce = Excel(xlCoerce, Excel(xlfCaller));

            // delete value in cell
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
        // offset from first call
        static ptrdiff_t offset(T* p = 0)
        {
            static T* p0 = nullptr;

            if (p0 == nullptr)
                p0 = p;

            return p0 - p;
        }
        static HANDLEX p2h(T* p)
        {
            return static_cast<HANDLEX>(offset(p));
        }
        // h = p0 - p, p = p0 - h
        static T* h2p(HANDLEX h)
        {
            return (T*)0 + offset() - static_cast<ptrdiff_t>(h);
        }
		T* pt;
		handle(T* pt)
			: pt(pt)
		{
			insert(p2h(pt));
		}
		handle(HANDLEX h)
		{
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
    };
} // xll namespace
