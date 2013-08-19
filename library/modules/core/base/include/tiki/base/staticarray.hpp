#pragma once
#ifndef TIKI_STATICARRAY_HPP
#define TIKI_STATICARRAY_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/inline.hpp"

namespace tiki
{
	template<typename T>
	class StaticArray
	{
	public:

		StaticArray();
		~StaticArray();

		TIKI_FORCE_INLINE void		create( const T* pData, size_t count );
		TIKI_FORCE_INLINE void		dispose();

		TIKI_FORCE_INLINE size_t	getCount() const;

		TIKI_FORCE_INLINE T*		getData();
		TIKI_FORCE_INLINE const T*	getData() const;

		TIKI_FORCE_INLINE T&		operator[]( size_t index );
		TIKI_FORCE_INLINE const T&	operator[]( size_t index ) const;

	private:

		const T*	m_pData;
		size_t		m_count;

	};
}

#include "staticarray.inl"

#endif // TIKI_STATICARRAY_HPP
