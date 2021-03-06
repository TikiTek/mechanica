#pragma once

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template< typename T >
	class LinkedList;

	template< typename T >
	class LinkedIterator;

	template< typename T >
	class LinkedItem
	{
		friend class LinkedList< T >;
		friend class LinkedIterator< T >;
		friend class LinkedIterator< const T >;

	public:

		LinkedItem()
		{
			pPrevItem	= nullptr;
			pNextItem	= nullptr;
#if TIKI_ENABLED( TIKI_USE_ASSERT )
			pList		= nullptr;
#endif
		}

		~LinkedItem()
		{
			TIKI_ASSERT( pList == nullptr );
		}

	private:

		T*					pPrevItem;
		T*					pNextItem;
#if TIKI_ENABLED( TIKI_USE_ASSERT )
		LinkedList< T >*	pList;
#endif
	};

	template< typename T >
	class LinkedIterator
	{
	public:

		typedef T				Type;
		typedef LinkedItem< T >	Item;

		typedef T*				Pointer;
		typedef T&				Reference;


		LinkedIterator( Pointer pItem )
			: m_pItem( pItem )
		{
		}

		LinkedIterator( const LinkedIterator& copy )
		{
			*this = copy;
		}

		void operator=( const LinkedIterator& copy )
		{
			m_pItem = copy.m_pItem;
		}

		LinkedIterator& operator++()
		{
			TIKI_ASSERT( m_pItem != nullptr );
			m_pItem = m_pItem->pNextItem;

			return *this;
		}

		bool operator==( const LinkedIterator& rhs )
		{
			return m_pItem == rhs.m_pItem;
		}

		bool operator!=( const LinkedIterator& rhs )
		{
			return m_pItem != rhs.m_pItem;
		}

		Reference operator*() const
		{
			return get();
		}

		Pointer operator->() const
		{
			TIKI_ASSERT( m_pItem != nullptr );
			return m_pItem;
		}

		Reference get() const
		{
			TIKI_ASSERT( m_pItem != nullptr );
			return *m_pItem;
		}

	private:

		Pointer	m_pItem;

	};

	template< typename T >
	class LinkedList
	{
		TIKI_NONCOPYABLE_CLASS( LinkedList );

	public:

		typedef T							Type;
		typedef LinkedItem< T >				Item;

		typedef LinkedIterator< T >			Iterator;
		typedef LinkedIterator< const T >	ConstIterator;

		typedef T*							Pointer;
		typedef const T*					ConstPointer;

		typedef T&							Reference;
		typedef const T&					ConstReference;

		TIKI_FORCE_INLINE					LinkedList();
		TIKI_FORCE_INLINE					~LinkedList();

		TIKI_FORCE_INLINE void				clear();

		TIKI_FORCE_INLINE bool				isEmpty() const	{ return m_count == 0u; }

		TIKI_FORCE_INLINE Reference			push( Reference value );
		TIKI_FORCE_INLINE Reference			push( Pointer pValue );

		TIKI_FORCE_INLINE Pointer			popFirst();
		TIKI_FORCE_INLINE Pointer			popLast();

		TIKI_FORCE_INLINE void				removeSortedByValue( Reference value );
		TIKI_FORCE_INLINE void				removeSortedByValue( Pointer pValue );

		TIKI_FORCE_INLINE uint				getCount() const						{ return m_count; }

		TIKI_FORCE_INLINE Iterator			getBegin()								{ return Iterator( m_pFirst ); }
		TIKI_FORCE_INLINE ConstIterator		getBegin() const						{ return ConstIterator( m_pFirst ); }

		TIKI_FORCE_INLINE Iterator			getEnd()								{ return Iterator( nullptr ); }
		TIKI_FORCE_INLINE ConstIterator		getEnd() const							{ return ConstIterator( nullptr ); }

		TIKI_FORCE_INLINE Pointer			getFirst()								{ return m_pFirst; }
		TIKI_FORCE_INLINE ConstPointer		getFirst() const						{ return m_pFirst; }
		TIKI_FORCE_INLINE Pointer			getLast()								{ return m_pLast; }
		TIKI_FORCE_INLINE ConstPointer		getLast() const							{ return m_pLast; }

		TIKI_FORCE_INLINE Pointer			getNext( Pointer pValue )				{ return pValue->pNextItem; }
		TIKI_FORCE_INLINE ConstPointer		getNext( ConstPointer pValue ) const	{ return pValue->pNextItem; }
		TIKI_FORCE_INLINE Pointer			getPrev( Pointer pValue )				{ return pValue->pPrevItem; }
		TIKI_FORCE_INLINE ConstPointer		getPrev( ConstPointer pValue ) const	{ return pValue->pPrevItem; }

		TIKI_FORCE_INLINE Iterator			begin()									{ return getBegin(); }
		TIKI_FORCE_INLINE ConstIterator		begin() const							{ return getBegin(); }

		TIKI_FORCE_INLINE Iterator			end()									{ return getEnd(); }
		TIKI_FORCE_INLINE ConstIterator		end() const								{ return getEnd(); }

	private:

		Type*		m_pFirst;
		Type*		m_pLast;

		uint		m_count;
	};
}

#include "../../../source/linked_list.inl"
