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
		
		typedef T			Type;

		typedef T*			Iterator;
		typedef const T*	ConstIterator;

		typedef T&			Reference;
		typedef const T&	ConstReference;

		SizedArray();
		~SizedArray();

		TIKI_FORCE_INLINE bool		create( uint capacity );
		TIKI_FORCE_INLINE void		dispose();

		TIKI_FORCE_INLINE void		clear();

		TIKI_FORCE_INLINE bool		isEmpty() const	{ return m_count == 0u; }
		TIKI_FORCE_INLINE bool		isFull() const	{ return m_count == m_capacity; }

		TIKI_FORCE_INLINE uint		getCount() const;
		TIKI_FORCE_INLINE uint		getCapacity() const;

		TIKI_FORCE_INLINE T*		getData();
		TIKI_FORCE_INLINE const T*	getData() const;

		TIKI_FORCE_INLINE T*		getEnd();
		TIKI_FORCE_INLINE const T*	getEnd() const;

		TIKI_FORCE_INLINE T&		getTop();
		TIKI_FORCE_INLINE const T&	getTop() const;

		TIKI_FORCE_INLINE uint		getIndexOf( const T& value ) const;

		//TIKI_FORCE_INLINE bool		remove( const T& value );
		TIKI_FORCE_INLINE void		removeUnsortedByIndex( uint index );
		TIKI_FORCE_INLINE bool		removeUnsortedByValue( const T& value );

		TIKI_FORCE_INLINE T&		pop();
		TIKI_FORCE_INLINE T&		push();
		TIKI_FORCE_INLINE T&		push( const T& value );
		TIKI_FORCE_INLINE T*		pushRange( uint count );
		TIKI_FORCE_INLINE void		pushRange( const T* pData, uint count );

		TIKI_FORCE_INLINE T&		operator[]( uint index );
		TIKI_FORCE_INLINE const T&	operator[]( uint index ) const;

	private:

		T*		m_pData;

		uint	m_count;
		uint	m_capacity;

	};
}

#include "../../../source/sizedarray.inl"

#endif // TIKI_SIZEDARRAY_HPP
