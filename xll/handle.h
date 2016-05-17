// handle.h - Handles to C++ objects
#pragma once

using HANDLEX = double;

namespace xll {

	template<class T>
	inline HANDLEX p2h(T* _p)
	{
		union {
			T* p;
			HANDLEX h;
			int32_t i[2];
		};
		p = _p;
#ifdef _M_X64 
		std::swap(i[0],i[1]);
#endif

		return h;
	}
	template<class T>
	inline T* h2p(HANDLEX _h)
	{
		union {
			T* p;
			HANDLEX h;
			int32_t i[2];
		};
		h = _h;
#ifdef _M_X64 
		std::swap(i[0],i[1]);
#endif

		return p;
	}

	// Pointers to objects encoded as doubles
	template<class T>
	class handle {
		const T* p;
	public:
		handle(const T* p)
			: p(p)
		{ }
		handle(HANDLEX h)
		{
			p = h2p<T>(h);
		}
		handle(const handle&) = delete;
		handle& operator=(const handle&) = delete;
		~handle()
		{ }
		HANDLEX get() const
		{
			return p2h<T>(p);
		}
	};

} // xll namespace
