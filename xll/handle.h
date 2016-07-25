// handle.h - Handles to C++ objects
#pragma once
//#include <memory>
#include <set>
#include <windows.h>
#include "XLCALL.H"

using HANDLEX = double;

namespace xll {

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
		static std::set<T*>& handles()
		{
			static std::set<T*> handles_;

			return handles_;
		}
		struct XLOPER12 : public ::XLOPER12
		{
			XLOPER12()
			{
				this->xltype = xltypeNil;
			}
			XLOPER12(const XLOPER12&) = delete;
			XLOPER12& operator=(const XLOPER12&) = delete;
			~XLOPER12()
			{
				Excel12(xlFree, 0, 1, this);
			}
		};
		static bool insert(T* p)
		{
			XLOPER12 caller, coerce;
			Excel12(xlfCaller, &caller, 0);
			Excel12(xlCoerce, &coerce, 1, &caller);
			if (coerce.xltype == xltypeNum && coerce.val.num != 0)
			{
				handle h(coerce.val.num);
				auto i = handles().find(h.ptr());
				if (i != handles().end())
					delete *i;
			}
			return handles().insert(p).second;
		}
		static int32_t& i1()
		{
			static int32_t i1_;
			
			return i1_; // high order bits
		}
		T* pt;
	public:
		handle(T* pt)
			: pt(pt)
		{
			union {
				T* p;
				int32_t i[2];
			};

			p = pt;
			i1() = i[1];
			insert(pt);
		}
		handle(HANDLEX h)
		{
			union {
				T* p;
				int32_t i[2];
			};

			i[0] = static_cast<int32_t>(h);
			i[1] = i1();
			pt = p;
		}
		handle(const handle&) = delete;
		handle& operator=(const handle&) = delete;
		~handle()
		{ }
		HANDLEX get() const
		{
			union {
				T* p;
				int32_t i[2];
			};

			p = pt;
			
			return i[0];
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
		/*		operator T*()
		{
			return ptr();
		}
		const operator T*() const
		{
			return ptr();
		}
*/	};

} // xll namespace
