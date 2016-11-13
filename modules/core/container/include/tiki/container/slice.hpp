#pragma once
#ifndef TIKI_SLICE_HPP_INCLUDED
#define TIKI_SLICE_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	template< typename T >
	class Slice
	{
	public:

		typedef T							Type;
		typedef T&							Reference;
		typedef const T&					ConstReference;
		typedef T*							Iterator;
		typedef const T*					ConstIterator;

		explicit							Slice();
		explicit							Slice( T* pData, uint* pCount, uint capacity );

		TIKI_FORCE_INLINE void				clear();

		TIKI_FORCE_INLINE bool				isEmpty() const { return getCount() == 0u; }
		TIKI_FORCE_INLINE bool				isFull() const { return getCount() == m_capacity; }

		TIKI_FORCE_INLINE Reference			push();
		TIKI_FORCE_INLINE Reference			push( ConstReference value );
		TIKI_FORCE_INLINE Iterator			pushRange( uint count );
		TIKI_FORCE_INLINE Iterator			pushRange( ConstIterator pData, uint count );

		TIKI_FORCE_INLINE bool				pop( Reference value );

		TIKI_FORCE_INLINE void				removeUnsortedByIndex( uint index );
		TIKI_FORCE_INLINE bool				removeUnsortedByValue( ConstReference value );

		TIKI_FORCE_INLINE uint				getCount() const { return *m_pCount; }
		TIKI_FORCE_INLINE uint				getCapacity() const { return m_capacity; }

		TIKI_FORCE_INLINE Iterator			getBegin() { return m_pData; }
		TIKI_FORCE_INLINE ConstIterator		getBegin() const { return m_pData; }

		TIKI_FORCE_INLINE Iterator			getEnd() { return m_pData + getCount(); }
		TIKI_FORCE_INLINE ConstIterator		getEnd() const { return m_pData + getCount(); }

		TIKI_FORCE_INLINE Reference			getFirst() { return (*this)[ 0u ]; }
		TIKI_FORCE_INLINE ConstReference	getFirst() const { return (*this)[ 0u ]; }

		TIKI_FORCE_INLINE Reference			getLast() { return (*this)[ getCount() - 1u ]; }
		TIKI_FORCE_INLINE ConstReference	getLast() const { return (*this)[ getCount() - 1u ]; }

		TIKI_FORCE_INLINE uint				getIndexOfIterator( ConstIterator pValue ) const;
		TIKI_FORCE_INLINE uint				getIndexOfValue( ConstReference value ) const;

		TIKI_FORCE_INLINE Reference			operator[]( uint index );
		TIKI_FORCE_INLINE ConstReference	operator[]( uint index ) const;

		TIKI_FORCE_INLINE Iterator			begin() { return getBegin(); }
		TIKI_FORCE_INLINE ConstIterator		begin() const { return getBegin(); }

		TIKI_FORCE_INLINE Iterator			end() { return getEnd(); }
		TIKI_FORCE_INLINE ConstIterator		end() const { return getEnd(); }

	private:

		T*									m_pData;
		uint*								m_pCount;
		uint								m_capacity;
	};
}

#include "../../../source/slice.inl"

#endif // TIKI_SLICE_HPP_INCLUDED
