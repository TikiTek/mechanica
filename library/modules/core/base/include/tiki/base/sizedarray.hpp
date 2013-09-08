#pragma once
#ifndef TIKI_SIZEDARRAY_HPP
#define TIKI_SIZEDARRAY_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/inline.hpp"

namespace tiki
{
	template<typename T>
	class SizedArray
	{
	public:

		typedef T* Iterator;
		typedef const T* ConstIterator;

		SizedArray();
		~SizedArray();

		TIKI_FORCE_INLINE void		create( size_t capacity );
		TIKI_FORCE_INLINE void		dispose();

		TIKI_FORCE_INLINE void		clear();

		TIKI_FORCE_INLINE size_t	getCount() const;
		TIKI_FORCE_INLINE size_t	getCapacity() const;

		TIKI_FORCE_INLINE T*		getData();
		TIKI_FORCE_INLINE const T*	getData() const;

		TIKI_FORCE_INLINE T*		getEnd();
		TIKI_FORCE_INLINE const T*	getEnd() const;

		TIKI_FORCE_INLINE T&		getTop();
		TIKI_FORCE_INLINE const T&	getTop() const;

		TIKI_FORCE_INLINE bool		remove( const T& value );
		TIKI_FORCE_INLINE void		removeAt( size_t index );

		TIKI_FORCE_INLINE T&		pop();
		TIKI_FORCE_INLINE T&		push();
		TIKI_FORCE_INLINE T&		push( const T& value );
		TIKI_FORCE_INLINE T*		pushRange( uint count );
		TIKI_FORCE_INLINE void		pushRange( const T* pData, uint count );

		TIKI_FORCE_INLINE T&		operator[]( size_t index );
		TIKI_FORCE_INLINE const T&	operator[]( size_t index ) const;

	private:

		T*		m_pData;

		size_t	m_count;
		size_t	m_capacity;

	};
}

#include "sizedarray.inl"

#endif // TIKI_SIZEDARRAY_HPP
