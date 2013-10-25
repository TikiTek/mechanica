#pragma once
#ifndef __TIKI_FIXEDARRAY_HPP_INCLUDED__
#define __TIKI_FIXEDARRAY_HPP_INCLUDED__

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T, uint TCapacity>
	class FixedArray
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( FixedArray );

	public:

		typedef T			Type;

		typedef T*			Iterator;
		typedef const T*	ConstIterator;

		typedef T&			Reference;
		typedef const T&	ConstReference;

		TIKI_FORCE_INLINE void create( ConstReference initValue, uint count = TCapacity )
		{
			TIKI_ASSERT( count <= TCapacity );

			for (uint i = 0u; i < count; ++i)
			{
				m_data[ i ] = initValue;
			}
		}

		TIKI_FORCE_INLINE void create( ConstIterator pInitData, uint count )
		{
			TIKI_ASSERT( count <= TCapacity );
			for (uint i = 0u; i < count; ++i)
			{
				m_data[ i ] = pInitData[ i ];
			}
		}

		TIKI_FORCE_INLINE uint				getCapacity() const	{ return TCapacity; }

		TIKI_FORCE_INLINE Iterator			getBegin()			{ return m_data; }
		TIKI_FORCE_INLINE ConstIterator		getBegin() const	{ return m_data; }

		TIKI_FORCE_INLINE Iterator			getEnd()			{ return m_pData + TCapacity; }
		TIKI_FORCE_INLINE ConstIterator		getEnd() const		{ return m_pData + TCapacity; }

		TIKI_FORCE_INLINE Reference			getFirst()			{ return m_data[ 0u ]; }
		TIKI_FORCE_INLINE ConstReference	getFirst() const	{ return m_data[ 0u ]; }

		TIKI_FORCE_INLINE Reference			getLast()			{ return m_data[ TCapacity - 1u ]; }
		TIKI_FORCE_INLINE ConstReference	getLast() const		{ return m_data[ TCapacity - 1u ]; }

		TIKI_FORCE_INLINE uint getIndex( ConstReference value ) const
		{
			TIKI_ASSERT( &value >= m_data );
			TIKI_ASSERT( &value < getEnd() );
			return value - m_data;
		}

		TIKI_FORCE_INLINE Reference operator[]( uint index )
		{
			TIKI_ASSERT( index < TCapacity );
			return m_data[ index ];
		}

		TIKI_FORCE_INLINE ConstReference operator[]( uint index ) const
		{
			TIKI_ASSERT( index < TCapacity );
			return m_data[ index ];
		}

	private:

		Type m_data[ TCapacity ];

	};
}

#endif // __TIKI_FIXEDARRAY_HPP_INCLUDED__
