#pragma once
#ifndef __TIKI_ARRAY_HPP_INCLUDED__
#define __TIKI_ARRAY_HPP_INCLUDED__

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/inline.hpp"
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

		TIKI_FORCE_INLINE			Array();
		TIKI_FORCE_INLINE			~Array();

		TIKI_FORCE_INLINE bool		create( uint count, size_t aligment = TIKI_DEFAULT_ALIGNMENT );
		TIKI_FORCE_INLINE bool		create( const T* pInitData, uint count, size_t aligment = TIKI_DEFAULT_ALIGNMENT );
		TIKI_FORCE_INLINE void		dispose();

		TIKI_FORCE_INLINE size_t	getCount() const;

		TIKI_FORCE_INLINE T*		getData();
		TIKI_FORCE_INLINE const T*	getData() const;
		TIKI_FORCE_INLINE T*		getLast();
		TIKI_FORCE_INLINE const T*	getLast() const;
		TIKI_FORCE_INLINE T*		getEnd();
		TIKI_FORCE_INLINE const T*	getEnd() const;

		TIKI_FORCE_INLINE size_t	getIndex( const T& value ) const;
		TIKI_FORCE_INLINE T&		operator[]( uint index );
		TIKI_FORCE_INLINE const T&	operator[]( uint index ) const;

	private:

		T*		m_pData;
		uint	m_dataCount;

	};
}

#include "../../../source/array.inl"

#endif // __TIKI_ARRAY_HPP_INCLUDED__
