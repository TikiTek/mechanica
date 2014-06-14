#pragma once
#ifndef __TIKI_GRAPHICSSTATEOBJECT_INL_INCLUDED__
#define __TIKI_GRAPHICSSTATEOBJECT_INL_INCLUDED__

#include "tiki/base/assert.hpp"

namespace tiki
{
	GraphicsStateObject::GraphicsStateObject()
	{
		m_refCount = 0u;
		m_hashValue = 0u;
	}

	GraphicsStateObject::~GraphicsStateObject()
	{
		TIKI_ASSERT( m_refCount == 0u );
		TIKI_ASSERT( m_hashValue == 0u );
	}

	TIKI_FORCE_INLINE void GraphicsStateObject::create( crc32 hashValue )
	{
		m_refCount = 1u;
		m_hashValue = hashValue;
	}

	TIKI_FORCE_INLINE void GraphicsStateObject::dispose()
	{
		m_refCount = 0u;
		m_hashValue = 0u;
	}

	TIKI_FORCE_INLINE bool GraphicsStateObject::isCompatible( crc32 hashValue ) const
	{
		TIKI_ASSERT( m_refCount != 0u );
		return m_hashValue == hashValue;
	}

	TIKI_FORCE_INLINE crc32	GraphicsStateObject::getHashValue() const
	{
		TIKI_ASSERT( m_refCount != 0u );
		return m_hashValue;
	}

	TIKI_FORCE_INLINE void GraphicsStateObject::addRef()
	{
		TIKI_ASSERT( m_refCount != 0u );
		m_refCount++;
	}

	TIKI_FORCE_INLINE bool GraphicsStateObject::releaseRef()
	{
		TIKI_ASSERT( m_refCount != 0u );
		return --m_refCount != 0u;
	}
}

#endif // __TIKI_GRAPHICSSTATEOBJECT_INL_INCLUDED__
