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
		List( const T* data, uint count, bool readOnly );
		~List();

		TIKI_FORCE_INLINE void		clear();
		TIKI_FORCE_INLINE void		dispose();

		TIKI_FORCE_INLINE size_t	getCount() const;

		TIKI_FORCE_INLINE sint		indexOf( const T& item ) const;
		TIKI_FORCE_INLINE bool		contains( const T& item ) const;

		TIKI_FORCE_INLINE T&		add();
		TIKI_FORCE_INLINE void		add( const T& item );
		TIKI_FORCE_INLINE void		addRange( const List<T>& list );
		TIKI_FORCE_INLINE void		addRange( const T* src, size_t length );
		TIKI_FORCE_INLINE void		insert( size_t index, const T& item );

		TIKI_FORCE_INLINE const T*	getData() const;
		TIKI_FORCE_INLINE const T*	getEnd() const;

		TIKI_FORCE_INLINE bool		remove( const T& item );
		TIKI_FORCE_INLINE void		removeAt( size_t index );

		TIKI_FORCE_INLINE void		reserve( const size_t count );

		TIKI_FORCE_INLINE const T&	operator[]( size_t index ) const;
		TIKI_FORCE_INLINE T&		operator[]( size_t index );

		TIKI_FORCE_INLINE void		operator=( const List<T>& copy );

	protected:

		T*							m_pData;
		size_t						m_count;
		size_t						m_capacity;

		bool						m_isReadOnly;

	private:

		TIKI_FORCE_INLINE size_t	getNextSize( size_t targetSize );
		TIKI_FORCE_INLINE void		checkArraySize( size_t neddedSize );
	};
}

#include "list.inl"

#endif // TIKI_LIST_HPP