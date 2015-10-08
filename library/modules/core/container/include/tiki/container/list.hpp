#pragma once
#ifndef TIKI_LIST_HPP
#define TIKI_LIST_HPP

#include "tiki/base/assert.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	class List
	{
	public:

		typedef T			Type;
		typedef T&			Reference;
		typedef const T&	ConstReference;
		typedef T*			Iterator;
		typedef const T*	ConstIterator;

		TIKI_FORCE_INLINE					List();
		TIKI_FORCE_INLINE					List( const List<T>& copy );
		TIKI_FORCE_INLINE					List( const T* data, uint count, bool readOnly = false );
		TIKI_FORCE_INLINE					~List();

		TIKI_FORCE_INLINE void				clear();
		TIKI_FORCE_INLINE void				dispose();
		
		TIKI_FORCE_INLINE void				reserve( uint count );
		TIKI_FORCE_INLINE void				resize( uint count );

		TIKI_FORCE_INLINE uint				getCount() const;
		TIKI_FORCE_INLINE bool				isEmpty() const;

		TIKI_FORCE_INLINE sint				indexOf( const T& item ) const;
		TIKI_FORCE_INLINE bool				contains( const T& item ) const;

		TIKI_FORCE_INLINE T&				add();
		TIKI_FORCE_INLINE void				add( const T& item );
		TIKI_FORCE_INLINE void				addRange( const List<T>& list );
		TIKI_FORCE_INLINE void				addRange( const T* src, uint length );
		TIKI_FORCE_INLINE void				insert( uint index, const T& item );

		TIKI_FORCE_INLINE Iterator			getBegin();
		TIKI_FORCE_INLINE ConstIterator		getBegin() const;

		TIKI_FORCE_INLINE Iterator			getEnd();
		TIKI_FORCE_INLINE ConstIterator		getEnd() const;

		TIKI_FORCE_INLINE Reference			getFirst();
		TIKI_FORCE_INLINE ConstReference	getFirst() const;

		TIKI_FORCE_INLINE Reference			getLast();
		TIKI_FORCE_INLINE ConstReference	getLast() const;

		TIKI_FORCE_INLINE bool				removeSortedByValue( const T& item );
		TIKI_FORCE_INLINE void				removeSortedAtIndex( uint index );

		TIKI_FORCE_INLINE Reference			operator[]( uint index );
		TIKI_FORCE_INLINE ConstReference	operator[]( uint index ) const;

		TIKI_FORCE_INLINE void				operator=( const List<T>& copy );

		TIKI_FORCE_INLINE Iterator			begin()				{ return getBegin(); }
		TIKI_FORCE_INLINE ConstIterator		begin() const		{ return getBegin(); }

		TIKI_FORCE_INLINE Iterator			end()				{ return getEnd(); }
		TIKI_FORCE_INLINE ConstIterator		end() const			{ return getEnd(); }

	private:

		T*									m_pData;
		uint								m_count;
		uint								m_capacity;

		bool								m_isReadOnly;

		TIKI_FORCE_INLINE uint				getNextSize( uint targetSize );
		TIKI_FORCE_INLINE void				checkArraySize( uint neddedSize );

	};
}

#include "../../../source/list.inl"

#endif // TIKI_LIST_HPP