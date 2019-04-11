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
        union ph {
            T* p;
            int32_t i[2];
            HANDLEX h;
        };
        static HANDLEX p2h(T* p)
        {
            ph h;

            h.p = p;
#ifdef _WIN64
            std::swap(h.i[0], h.i[1]);
#endif
            return h.h;
        }
        static T* h2p(HANDLEX h)
        {
            ph p;

            p.h = h;
#ifdef _WIN64
            std::swap(p.i[0], p.i[1]);
#endif
            return p.p;
        }

        static std::set<T*>& handles()
		{
            struct set_ {
                std::set<T*> h;
                ~set_()
                {
                    for (auto p : h)
                        delete p;
                }
            };
			static set_ h;

			return h.h;;
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
                auto i = hs.find(h2p(n)); // h2p(coerce) ???
				if (i != hs.end()) {
                    delete *i;
					hs.erase(i);
				}
			}

            hs.insert(p);
		}
		T* pt;
    public:
        //!!! use ptr sink
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
