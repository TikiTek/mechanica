#pragma once
#ifndef TIKI_SIZEDARRAY_HPP
#define TIKI_SIZEDARRAY_HPP

#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	class SizedArray
	{
		TIKI_NONCOPYABLE_CLASS( SizedArray );

	public:
		
		typedef T			Type;

		typedef T*			Iterator;
		typedef const T*	ConstIterator;

		typedef T&			Reference;
		typedef const T&	ConstReference;

		TIKI_FORCE_INLINE					SizedArray();
		TIKI_FORCE_INLINE					~SizedArray();

		TIKI_FORCE_INLINE bool				create( uint capacity );
		TIKI_FORCE_INLINE void				dispose();

		TIKI_FORCE_INLINE void				clear();

		TIKI_FORCE_INLINE bool				isEmpty() const	{ return m_count == 0u; }
		TIKI_FORCE_INLINE bool				isFull() const	{ return m_count == m_capacity; }

		TIKI_FORCE_INLINE Reference			push();
		TIKI_FORCE_INLINE Reference			push( ConstReference value );
		TIKI_FORCE_INLINE Iterator			pushRange( uint count );
		TIKI_FORCE_INLINE Iterator			pushRange( ConstIterator pData, uint count );

		TIKI_FORCE_INLINE bool				pop( Reference value );

		TIKI_FORCE_INLINE void				removeUnsortedByIndex( uint index );
		TIKI_FORCE_INLINE bool				removeUnsortedByValue( ConstReference value );

		TIKI_FORCE_INLINE uint				getCount() const	{ return m_count; }
		TIKI_FORCE_INLINE uint				getCapacity() const	{ return m_capacity; }

		TIKI_FORCE_INLINE Iterator			getBegin()			{ return m_pData; }
		TIKI_FORCE_INLINE ConstIterator		getBegin() const	{ return m_pData; }

		TIKI_FORCE_INLINE Iterator			getEnd()			{ return m_pData + m_count; }
		TIKI_FORCE_INLINE ConstIterator		getEnd() const		{ return m_pData + m_count; }

		TIKI_FORCE_INLINE Reference			getFirst()			{ return (*this)[ 0u ]; }
		TIKI_FORCE_INLINE ConstReference	getFirst() const	{ return (*this)[ 0u ]; }

		TIKI_FORCE_INLINE Reference			getLast()			{ return (*this)[ m_count - 1u ]; }
		TIKI_FORCE_INLINE ConstReference	getLast() const		{ return (*this)[ m_count - 1u ]; }

		TIKI_FORCE_INLINE uint				getIndexOf( ConstIterator pValue ) const;
		TIKI_FORCE_INLINE uint				getIndexOf( ConstReference value ) const;

		TIKI_FORCE_INLINE Reference			operator[]( uint index );
		TIKI_FORCE_INLINE ConstReference	operator[]( uint index ) const;

		TIKI_FORCE_INLINE Iterator			begin()				{ return getBegin(); }
		TIKI_FORCE_INLINE ConstIterator		begin() const		{ return getBegin(); }

		TIKI_FORCE_INLINE Iterator			end()				{ return getEnd(); }
		TIKI_FORCE_INLINE ConstIterator		end() const			{ return getEnd(); }

	private:

		uint	m_count;
		uint	m_capacity;
		Type*	m_pData;

	};
}

#include "../../../source/sizedarray.inl"

#endif // TIKI_SIZEDARRAY_HPP
