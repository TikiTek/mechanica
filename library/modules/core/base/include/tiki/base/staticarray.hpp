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

		typedef T			Type;

		typedef T*			Iterator;
		typedef const T*	ConstIterator;

		typedef T&			Reference;
		typedef const T&	ConstReference;

		TIKI_FORCE_INLINE					StaticArray();
		TIKI_FORCE_INLINE					~StaticArray();

		TIKI_FORCE_INLINE void				create( T* pData, uint count );
		TIKI_FORCE_INLINE void				dispose();

		TIKI_FORCE_INLINE uint				getCount() const	{ return (uint)m_count; }
		TIKI_FORCE_INLINE uint				getCapacity() const	{ return (uint)m_count; }

		TIKI_FORCE_INLINE Iterator			getBegin()			{ return m_pData; }
		TIKI_FORCE_INLINE ConstIterator		getBegin() const	{ return m_pData; }

		TIKI_FORCE_INLINE Iterator			getEnd()			{ return m_pData + m_count; }
		TIKI_FORCE_INLINE ConstIterator		getEnd() const		{ return m_pData + m_count; }

		TIKI_FORCE_INLINE Reference			getFirst()			{ return m_pData[ 0u ]; }
		TIKI_FORCE_INLINE ConstReference	getFirst() const	{ return m_pData[ 0u ]; }

		TIKI_FORCE_INLINE Reference			getLast()			{ return m_pData[ m_count - 1u ]; }
		TIKI_FORCE_INLINE ConstReference	getLast() const		{ return m_pData[ m_count - 1u ]; }

		TIKI_FORCE_INLINE uint				getIndexOf( ConstIterator pValue ) const;
		TIKI_FORCE_INLINE uint				getIndexOf( ConstReference value ) const;

		TIKI_FORCE_INLINE Reference			operator[]( uint index );
		TIKI_FORCE_INLINE ConstReference	operator[]( uint index ) const;

		TIKI_FORCE_INLINE Iterator			begin()				{ return getBegin(); }
		TIKI_FORCE_INLINE ConstIterator		begin() const		{ return getBegin(); }

		TIKI_FORCE_INLINE Iterator			end()				{ return getEnd(); }
		TIKI_FORCE_INLINE ConstIterator		end() const			{ return getEnd(); }

	private:

		T*			m_pData;
		uint64		m_count;

	};
}

#include "../../../source/staticarray.inl"

#endif // TIKI_STATICARRAY_HPP
