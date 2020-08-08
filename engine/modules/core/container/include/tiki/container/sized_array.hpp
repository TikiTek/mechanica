#pragma once

#include "tiki/base/types.hpp"
#include "tiki/container/array_view.hpp"
#include "tiki/container/slice.hpp"

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

		TIKI_FORCE_INLINE bool				create( uintreg capacity, uintreg aligment = TIKI_DEFAULT_ALIGNMENT, bool constructElements = true );
		TIKI_FORCE_INLINE void				dispose();

		TIKI_FORCE_INLINE void				clear();

		TIKI_FORCE_INLINE bool				isEmpty() const	{ return m_count == 0u; }
		TIKI_FORCE_INLINE bool				isFull() const	{ return m_count == m_capacity; }

		TIKI_FORCE_INLINE Reference			push();
		TIKI_FORCE_INLINE Reference			push( ConstReference value );
		TIKI_FORCE_INLINE Iterator			pushRange( uintreg count );
		TIKI_FORCE_INLINE Iterator			pushRange( ConstIterator pData, uintreg count );

		TIKI_FORCE_INLINE bool				pop( Reference value );

		TIKI_FORCE_INLINE void				removeUnsortedByIndex( uintreg index );
		TIKI_FORCE_INLINE bool				removeUnsortedByValue( ConstReference value );

		TIKI_FORCE_INLINE uintreg				getCount() const	{ return m_count; }
		TIKI_FORCE_INLINE uintreg				getCapacity() const	{ return m_capacity; }

		TIKI_FORCE_INLINE Iterator			getBegin()			{ return m_pData; }
		TIKI_FORCE_INLINE ConstIterator		getBegin() const	{ return m_pData; }

		TIKI_FORCE_INLINE Iterator			getEnd()			{ return m_pData + m_count; }
		TIKI_FORCE_INLINE ConstIterator		getEnd() const		{ return m_pData + m_count; }

		TIKI_FORCE_INLINE Reference			getFirst()			{ return (*this)[ 0u ]; }
		TIKI_FORCE_INLINE ConstReference	getFirst() const	{ return (*this)[ 0u ]; }

		TIKI_FORCE_INLINE Reference			getLast()			{ return (*this)[ m_count - 1u ]; }
		TIKI_FORCE_INLINE ConstReference	getLast() const		{ return (*this)[ m_count - 1u ]; }

		TIKI_FORCE_INLINE uintreg				getIndexOfIterator( ConstIterator pValue ) const;
		TIKI_FORCE_INLINE uintreg				getIndexOfValue( ConstReference value ) const;

		TIKI_FORCE_INLINE Reference			operator[]( uintreg index );
		TIKI_FORCE_INLINE ConstReference	operator[]( uintreg index ) const;

		TIKI_FORCE_INLINE Slice< T >		toSlice();
		TIKI_FORCE_INLINE ArrayView< T >	toArrayView() const;

		TIKI_FORCE_INLINE Iterator			begin()				{ return getBegin(); }
		TIKI_FORCE_INLINE ConstIterator		begin() const		{ return getBegin(); }

		TIKI_FORCE_INLINE Iterator			end()				{ return getEnd(); }
		TIKI_FORCE_INLINE ConstIterator		end() const			{ return getEnd(); }

	private:

		Type*	m_pData;
		uintreg	m_count;
		uintreg	m_capacity;
	};
}

#include "../../../source/sized_array.inl"
