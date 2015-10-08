#pragma once
#ifndef TIKI_LIST_HPP
#define TIKI_LIST_HPP

#include "tiki/base/assert.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	class Pool
	{
		TIKI_NONCOPYABLE_CLASS( Pool );

	public:

		typedef T			Type;
		typedef T&			Reference;
		typedef const T&	ConstReference;

		TIKI_FORCE_INLINE					Pool();
		TIKI_FORCE_INLINE					~Pool();

		TIKI_FORCE_INLINE void				create(uint capacity);
		TIKI_FORCE_INLINE void				dispose();

		TIKI_FORCE_INLINE void				clear();

		TIKI_FORCE_INLINE uint				getCount() const;
		TIKI_FORCE_INLINE bool				isEmpty() const;

		TIKI_FORCE_INLINE bool				contains( const T& item ) const;

		TIKI_FORCE_INLINE T&				push();
		TIKI_FORCE_INLINE T&				push( const T& item );

		TIKI_FORCE_INLINE bool				removeUnsortedByValue( const T& item );

	private:

		struct FreeElement
		{
			T*		pNextFree;
			void*	pMagicId;
		};

		T*			m_pData;
		T*			m_pFirstFree;

		uint		m_count;
		uint		m_capacity;

	};
}

#include "../../../source/pool.inl"

#endif // TIKI_LIST_HPP