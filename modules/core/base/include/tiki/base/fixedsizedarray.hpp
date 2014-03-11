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

		FixedSizedArray();
		~FixedSizedArray();

		TIKI_FORCE_INLINE void				create( ConstReference initValue, uint count = TCapacity );
		TIKI_FORCE_INLINE void				create( ConstIterator pInitData, uint count );

		TIKI_FORCE_INLINE void				clear();

		TIKI_FORCE_INLINE Reference			push();
		TIKI_FORCE_INLINE Reference			push( ConstReference value );

		TIKI_FORCE_INLINE void				removeUnsortedByIndex( uint index );
		TIKI_FORCE_INLINE bool				removeUnsortedByValue( ConstReference value );

		TIKI_FORCE_INLINE uint				getCount() const	{ return m_count; }
		TIKI_FORCE_INLINE uint				getCapacity() const	{ return TCapacity; }

		TIKI_FORCE_INLINE Iterator			getBegin()			{ return m_data; }
		TIKI_FORCE_INLINE ConstIterator		getBegin() const	{ return m_data; }

		TIKI_FORCE_INLINE Iterator			getEnd()			{ return m_pData + m_count; }
		TIKI_FORCE_INLINE ConstIterator		getEnd() const		{ return m_pData + m_count; }

		TIKI_FORCE_INLINE Reference			getFirst()			{ return m_data[ 0u ]; }
		TIKI_FORCE_INLINE ConstReference	getFirst() const	{ return m_data[ 0u ]; }

		TIKI_FORCE_INLINE Reference			getLast()			{ return m_data[ m_count - 1u ]; }
		TIKI_FORCE_INLINE ConstReference	getLast() const		{ return m_data[ m_count - 1u ]; }
		
		TIKI_FORCE_INLINE uint				getIndex( ConstIterator pValue ) const;
		TIKI_FORCE_INLINE uint				getIndex( ConstReference value ) const;

		TIKI_FORCE_INLINE Reference			operator[]( uint index );
		TIKI_FORCE_INLINE ConstReference	operator[]( uint index ) const;

	private:

		uint	m_count;
		Type	m_data[ TCapacity ];

	};
}

#include "../../../source/fixedsizedarray.inl"

#endif // __TIKI_FIXEDSIZEDARRAY_HPP_INCLUDED__
