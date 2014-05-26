#pragma once
#ifndef __TIKI_FIXEDSIZEDARRAY_HPP_INCLUDED__
#define __TIKI_FIXEDSIZEDARRAY_HPP_INCLUDED__

#include "tiki/base/assert.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T, uint TCapacity>
	class FixedSizedArray
	{
		TIKI_NONCOPYABLE_CLASS( FixedSizedArray );

	public:

		typedef T			Type;

		typedef T*			Iterator;
		typedef const T*	ConstIterator;

		typedef T&			Reference;
		typedef const T&	ConstReference;

		TIKI_FORCE_INLINE					FixedSizedArray();
		TIKI_FORCE_INLINE					~FixedSizedArray();

		TIKI_FORCE_INLINE void				create( ConstReference initValue, uint count = TCapacity );
		TIKI_FORCE_INLINE void				create( ConstIterator pInitData, uint count );

		TIKI_FORCE_INLINE void				clear();

		TIKI_FORCE_INLINE bool				isEmpty() const	{ return m_count == 0u; }
		TIKI_FORCE_INLINE bool				isFull() const	{ return m_count == TCapacity; }

		TIKI_FORCE_INLINE Reference			push();
		TIKI_FORCE_INLINE Reference			push( ConstReference value );
		TIKI_FORCE_INLINE Iterator			pushRange( uint count );
		TIKI_FORCE_INLINE Iterator			pushRange( ConstIterator pData, uint count );

		TIKI_FORCE_INLINE bool				pop( Reference value );

		TIKI_FORCE_INLINE void				removeUnsortedByIndex( uint index );
		TIKI_FORCE_INLINE bool				removeUnsortedByValue( ConstReference value );

		TIKI_FORCE_INLINE uint				getCount() const	{ return m_count; }
		TIKI_FORCE_INLINE uint				getCapacity() const	{ return TCapacity; }

		TIKI_FORCE_INLINE Iterator			getBegin()			{ return m_aData; }
		TIKI_FORCE_INLINE ConstIterator		getBegin() const	{ return m_aData; }

		TIKI_FORCE_INLINE Iterator			getEnd()			{ return m_aData + m_count; }
		TIKI_FORCE_INLINE ConstIterator		getEnd() const		{ return m_aData + m_count; }

		TIKI_FORCE_INLINE Reference			getFirst()			{ return m_aData[ 0u ]; }
		TIKI_FORCE_INLINE ConstReference	getFirst() const	{ return m_aData[ 0u ]; }

		TIKI_FORCE_INLINE Reference			getLast()			{ return m_aData[ m_count - 1u ]; }
		TIKI_FORCE_INLINE ConstReference	getLast() const		{ return m_aData[ m_count - 1u ]; }
		
		TIKI_FORCE_INLINE uint				getIndexOf( ConstIterator pValue ) const;
		TIKI_FORCE_INLINE uint				getIndexOf( ConstReference value ) const;

		TIKI_FORCE_INLINE Reference			operator[]( uint index );
		TIKI_FORCE_INLINE ConstReference	operator[]( uint index ) const;

	private:

		uint	m_count;
		Type	m_aData[ TCapacity ];

	};
}

#include "../../../source/fixedsizedarray.inl"

#endif // __TIKI_FIXEDSIZEDARRAY_HPP_INCLUDED__
