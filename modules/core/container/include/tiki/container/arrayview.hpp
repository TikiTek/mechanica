#pragma once
#ifndef TIKI_ARRAYVIEW_HPP_INCLUDED
#define TIKI_ARRAYVIEW_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	template< typename T >
	class ArrayView
	{
	public:

		typedef T							Type;
		typedef const T&					ConstReference;
		typedef const T*					ConstIterator;

		explicit							ArrayView();
		explicit							ArrayView( const T* pData, uint count );
		explicit							ArrayView( ConstIterator pBegin, ConstIterator pEnd );

		TIKI_FORCE_INLINE uint				getCount() const { return m_count; }

		TIKI_FORCE_INLINE ConstIterator		getBegin() const { return m_pData; }

		TIKI_FORCE_INLINE ConstIterator		getEnd() const { return m_pData + m_count; }

		TIKI_FORCE_INLINE ConstReference	getFirst() const { return m_pData[ 0u ]; }

		TIKI_FORCE_INLINE ConstReference	getLast() const { return m_pData[ m_count - 1u ]; }

		TIKI_FORCE_INLINE uint				getIndexOfIterator( ConstIterator pValue ) const;
		TIKI_FORCE_INLINE uint				getIndexOfValue( ConstReference value ) const;

		TIKI_FORCE_INLINE ConstReference	operator[]( uint index ) const;

		TIKI_FORCE_INLINE ConstIterator		begin() const { return getBegin(); }

		TIKI_FORCE_INLINE ConstIterator		end() const { return getEnd(); }

	private:

		const T*							m_pData;
		uint								m_count;
	};
}

#include "../../../source/arrayview.inl"

#endif // TIKI_ARRAYVIEW_HPP_INCLUDED
