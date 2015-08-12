#pragma once
#ifndef TIKI_NUMBERLIMITS_HPP_INCLUDED__
#define TIKI_NUMBERLIMITS_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#undef min
#undef max

namespace tiki
{
	template <class T>
	class NumberLimits
	{
	public:

		TIKI_FORCE_INLINE static T min()
		{
			return (T)0;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)0;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return (T)0;
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)0;
		}
	};

	template< class T >
	class NumberLimits< const T > : public NumberLimits< T >
	{
	};

	template< class T >
	class NumberLimits< volatile T > : public NumberLimits< T >
	{
	};

	template< class T >
	class NumberLimits< const volatile T > : public NumberLimits< T >
	{
	};

	template <>
	class NumberLimits< sint8 >
	{
	public:

		typedef sint8 T;

		TIKI_FORCE_INLINE static T min()
		{
			return (T)0x80;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)0x7f;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return min();
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)0;
		}
	};

	template <>
	class NumberLimits< sint16 >
	{
	public:

		typedef sint16 T;

		TIKI_FORCE_INLINE static T min()
		{
			return (T)0x8000;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)0x7fff;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return min();
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)0;
		}
	};

	template <>
	class NumberLimits< sint32 >
	{
	public:

		typedef sint32 T;

		TIKI_FORCE_INLINE static T min()
		{
			return (T)0x80000000;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)0x7fffffff;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return min();
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)0;
		}
	};

	template <>
	class NumberLimits< sint64 >
	{
	public:

		typedef sint64 T;

		TIKI_FORCE_INLINE static T min()
		{
			return (T)0x8000000000000000;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)0x7fffffffffffffff;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return min();
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)0;
		}
	};

	template <>
	class NumberLimits< uint8 >
	{
	public:

		typedef uint8 T;

		TIKI_FORCE_INLINE static T min()
		{
			return (T)0;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)0xff;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return min();
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)0;
		}
	};

	template <>
	class NumberLimits< uint16 >
	{
	public:

		typedef uint16 T;

		TIKI_FORCE_INLINE static T min()
		{
			return (T)0;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)0xffff;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return min();
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)0;
		}
	};

	template <>
	class NumberLimits< uint32 >
	{
	public:

		typedef uint32 T;

		TIKI_FORCE_INLINE static T min()
		{
			return (T)0;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)0xffffffff;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return min();
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)0;
		}
	};

	template <>
	class NumberLimits< uint64 >
	{
	public:

		typedef uint64 T;

		TIKI_FORCE_INLINE static T min()
		{
			return (T)0;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)0xffffffffffffffff;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return min();
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)0;
		}
	};

	template <>
	class NumberLimits< float32 >
	{
	public:

		typedef float32 T;

		TIKI_FORCE_INLINE static T min()
		{
			return (T)1.175494351e-38f;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)3.402823466e+38f;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return -max();
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)1.192092896e-07f;
		}
	};

	template <>
	class NumberLimits< float64 >
	{
	public:

		typedef float64 T;

		TIKI_FORCE_INLINE static T min()
		{
			return (T)2.2250738585072014e-308;
		}

		TIKI_FORCE_INLINE static T max()
		{
			return (T)1.7976931348623158e+308;
		}

		TIKI_FORCE_INLINE static T lowest()
		{
			return -max();
		}

		TIKI_FORCE_INLINE static T epsilon()
		{
			return (T)2.2204460492503131e-016;
		}
	};
}

#endif // TIKI_NUMBERLIMITS_HPP_INCLUDED__
