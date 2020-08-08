#pragma once
#ifndef __TIKI_ARRAY_HPP_INCLUDED__
#define __TIKI_ARRAY_HPP_INCLUDED__

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	class Array
	{
		TIKI_NONCOPYABLE_CLASS( Array );

	public:

		typedef T			Type;
		typedef T&			Reference;
		typedef const T&	ConstReference;
		typedef T*			Iterator;
		typedef const T*	ConstIterator;

		TIKI_FORCE_INLINE					Array();
		TIKI_FORCE_INLINE					~Array();

		TIKI_FORCE_INLINE bool				create( uintreg capacity, uintreg aligment = TIKI_DEFAULT_ALIGNMENT, bool constructElements = true );
		TIKI_FORCE_INLINE bool				create( ConstIterator pInitData, uintreg capacity, uintreg aligment = TIKI_DEFAULT_ALIGNMENT, bool constructElements = true );
		TIKI_FORCE_INLINE void				dispose();

		TIKI_FORCE_INLINE void				swap( Array< T >& other );

		TIKI_FORCE_INLINE uintreg			getCount() const	{ return m_capacity; }
		TIKI_FORCE_INLINE uintreg			getCapacity() const	{ return m_capacity; }

		TIKI_FORCE_INLINE Iterator			getBegin()			{ return m_pData; }
		TIKI_FORCE_INLINE ConstIterator		getBegin() const	{ return m_pData; }

		TIKI_FORCE_INLINE Iterator			getEnd()			{ return m_pData + m_capacity; }
		TIKI_FORCE_INLINE ConstIterator		getEnd() const		{ return m_pData + m_capacity; }

		TIKI_FORCE_INLINE Reference			getFirst()			{ return m_pData[ 0u ]; }
		TIKI_FORCE_INLINE ConstReference	getFirst() const	{ return m_pData[ 0u ]; }

		TIKI_FORCE_INLINE Reference			getLast()			{ return m_pData[ m_capacity - 1u ]; }
		TIKI_FORCE_INLINE ConstReference	getLast() const		{ return m_pData[ m_capacity - 1u ]; }

		TIKI_FORCE_INLINE uintreg				getIndexOfIterator( ConstIterator pValue ) const;
		TIKI_FORCE_INLINE uintreg		getIndexOfValue( ConstReference value ) const;

		TIKI_FORCE_INLINE Reference			operator[]( uintreg index );
		TIKI_FORCE_INLINE ConstReference	operator[]( uintreg index ) const;

		TIKI_FORCE_INLINE Iterator			begin()				{ return getBegin(); }
		TIKI_FORCE_INLINE ConstIterator		begin() const		{ return getBegin(); }

		TIKI_FORCE_INLINE Iterator			end()				{ return getEnd(); }
		TIKI_FORCE_INLINE ConstIterator		end() const			{ return getEnd(); }

	private:

		T*									m_pData;
		uintreg								m_capacity;
	};

	template< class T >
	class AutoArray : public Array< T >
	{
		TIKI_NONCOPYABLE_CLASS( AutoArray );

	public:

		TIKI_FORCE_INLINE					AutoArray() { }
		TIKI_FORCE_INLINE					~AutoArray() { dispose(); }
	};
}

#include "../../../source/array.inl"

#endif // __TIKI_ARRAY_HPP_INCLUDED__
