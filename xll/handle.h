// handle.h - Handles to C++ objects
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include <memory>
#include <set>
#include <windows.h>
#include "XLCALL.H"

using HANDLEX = double;

namespace xll {

	// encode and decode handles to strings
	namespace {
		// uint4 -> char
		inline char enc(uint8_t i)
		{
			return i <= 9 ? '0' + i : 'A' + i - 10;
		}
		template<class C>
		inline uint8_t dec(C c)
		{
			return static_cast<uint8_t>(c <= '9' ? c - '0' : 10 + c - 'A');
		}
		// encode/decode pointers
		inline void encode(uint32_t p, char* h)
		{
			union {
				uint32_t u;
				uint8_t u4[4];
			} p_;
			p_.u = p;

			h[0] = enc(p_.u4[0] & 0xF);
			h[1] = enc((p_.u4[0] & 0xF0) >> 4);
			h[2] = enc(p_.u4[1] & 0xF);
			h[3] = enc((p_.u4[1] & 0xF0) >> 4);
			h[4] = enc(p_.u4[2] & 0xF);
			h[5] = enc((p_.u4[2] & 0xF0) >> 4);
			h[6] = enc(p_.u4[3] & 0xF);
			h[7] = enc((p_.u4[3] & 0xF0) >> 4);
		}
		template<class C>
		inline void decode(const C* h, uint32_t* pu)
		{
			union {
				uint32_t u;
				uint8_t u4[4];
			} p_;

			p_.u4[0] = (dec(h[1]) << 4) | dec(h[0]);
			p_.u4[1] = (dec(h[3]) << 4) | dec(h[2]);
			p_.u4[2] = (dec(h[5]) << 4) | dec(h[4]);
			p_.u4[3] = (dec(h[7]) << 4) | dec(h[6]);

			*pu = p_.u;
		}

		// encode/decode pointers
		inline void encode(uint64_t p, char* h)
		{
			union {
				uint64_t u;
				uint32_t u2[2];
			} p_;
			p_.u = p;

			encode(p_.u2[0], h);
			encode(p_.u2[1], h + 8);
		}
		template<class C>
		inline void decode(const C* h, uint64_t* pu)
		{
			union {
				uint64_t u;
				uint32_t u2[2];
			} p_;

			decode(h, &p_.u2[0]);
			decode(h + 8, &p_.u2[1]);

			*pu = p_.u;
		}

	}

	template<class C>
	inline HANDLEX decode(const C* h)
	{
		if (h[1] == '^') {
			if (h[0] == 9) {
				uint32_t u;
				decode(h + 2, &u);

				return static_cast<HANDLEX>(u);
			}
			else if (h[0] == 17) {
				uint64_t u;
				decode(h + 2, &u);

				return static_cast<HANDLEX>(u);
			}
		}

		return 0;
	}
#ifdef _M_X64
	inline void encode(HANDLEX h, char* buf)
	{
		uint64_t u = static_cast<uint64_t>(h);
		buf[0] = 17;
		buf[1] = '^';
		encode(u, buf + 2);
	}
#else
	inline void encode(HANDLEX h, char* buf)
	{
		uint32_t u = static_cast<uint32_t>(h);
		buf[0] = 9;
		buf[1] = '^';
		encode(u, buf + 2);
	}
#endif

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
		class set {
			static inline std::map<HANDLEX, std::unique_ptr<T>> handle_map;
		public:
			// Convert pointer to (small) number based on first allocation.
			static HANDLEX lookup(T* p)
			{
				static T* base = nullptr;
				if (base == nullptr) {
					base = -8 + p;
				}
				return static_cast<HANDLEX>(p - base);
			}
			static void insert(T* p)
			{
				HANDLEX h = lookup(p);

				// delete if old handle in cell points at something
				OPER oldh = Excel(xlCoerce, Excel(xlfCaller));
				if (oldh.isStr()) {
					if (oldh.val.str[0] > 1 && oldh.val.str[1] == '^') {
						oldh = decode(oldh.val.str);
					}
				}
				if (oldh.isNum() && oldh.val.num != 0) {
					auto i = handle_map.find(h);
					if (i != handle_map.end()) {
						i->second.release();
						handle_map.erase(i);
					}
				}

				handle_map.insert(std::make_pair(h, std::unique_ptr<T>(p)));
			}
			static T* find(HANDLEX h)
			{
				auto i = handle_map.find(h);

				return i == handle_map.end() ? nullptr : i->second.get();
			}
		};
		T* pt;
	public:
		//!!! use ptr sink
		handle(T* p)
			: pt(p)
		{
			set::insert(p);
		}
		handle(HANDLEX h)
		{
			//!!! check if h in handles
			pt = set::find(h);
			ensure(pt != nullptr || !"invalid handle");
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
			return set::lookup(pt);
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
