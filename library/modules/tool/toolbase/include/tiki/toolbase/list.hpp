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

		List();
		List( const List<T>& copy );
		List( const T* data, uint count, bool readOnly = false );
		~List();

		TIKI_FORCE_INLINE void		clear();
		TIKI_FORCE_INLINE void		dispose();

		TIKI_FORCE_INLINE uint		getCount() const;
		TIKI_FORCE_INLINE bool		isEmpty() const;

		TIKI_FORCE_INLINE sint		indexOf( const T& item ) const;
		TIKI_FORCE_INLINE bool		contains( const T& item ) const;

		TIKI_FORCE_INLINE T&		add();
		TIKI_FORCE_INLINE void		add( const T& item );
		TIKI_FORCE_INLINE void		addRange( const List<T>& list );
		TIKI_FORCE_INLINE void		addRange( const T* src, uint length );
		TIKI_FORCE_INLINE void		insert( uint index, const T& item );

		TIKI_FORCE_INLINE T*		getData();
		TIKI_FORCE_INLINE T*		getLast();
		TIKI_FORCE_INLINE T*		getEnd();

		TIKI_FORCE_INLINE const T*	getData() const;
		TIKI_FORCE_INLINE const T*	getLast() const;
		TIKI_FORCE_INLINE const T*	getEnd() const;

		TIKI_FORCE_INLINE bool		remove( const T& item );
		TIKI_FORCE_INLINE void		removeAt( uint index );

		TIKI_FORCE_INLINE void		reserve( const uint count );

		TIKI_FORCE_INLINE const T&	operator[]( uint index ) const;
		TIKI_FORCE_INLINE T&		operator[]( uint index );

		TIKI_FORCE_INLINE void		operator=( const List<T>& copy );

	protected:

		T*							m_pData;
		uint						m_count;
		uint						m_capacity;

		bool						m_isReadOnly;

	private:

		TIKI_FORCE_INLINE uint		getNextSize( uint targetSize );
		TIKI_FORCE_INLINE void		checkArraySize( uint neddedSize );
	};
}

#include "../../../source/list.inl"

#endif // TIKI_LIST_HPP