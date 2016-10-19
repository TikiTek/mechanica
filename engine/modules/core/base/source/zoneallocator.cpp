
#include "tiki/base/zoneallocator.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"

namespace tiki
{

	ZoneAllocator::ZoneAllocator()
	{
		m_pMemory	= nullptr;
		m_size		= 0u;

		m_pCurrent	= nullptr;
	}

	ZoneAllocator::~ZoneAllocator()
	{

	}

	bool ZoneAllocator::create( uint sizeInBytes, uint alignment /* = TIKI_MINIMUM_ALIGNMENT */ )
	{
		m_pMemory = static_cast< uint8* >( TIKI_ALLOC_ALIGNED( sizeInBytes, alignment ) );

		if ( m_pMemory == nullptr )
		{
			TIKI_TRACE_ERROR( "[ZoneAllocator] Could not allocator Memory.\n" );
			return false;
		}

		m_pCurrent = m_pMemory;

		m_size			= sizeInBytes;
		m_baseAlignment	= alignment;

		return true;
	}

	void ZoneAllocator::dispose()
	{
		TIKI_MEMORY_FREE( m_pMemory );
		m_pMemory		= nullptr;
		m_pCurrent		= nullptr;
		m_size			= 0u;
		m_baseAlignment	= 0u;
	}

	void* ZoneAllocator::allocate( uint sizeInBytes, uint alignment /*= TIKI_DEFAULT_ALIGNMENT */ )
	{
		TIKI_ASSERT( m_pCurrent != nullptr );

		if ( alignment == TIKI_DEFAULT_ALIGNMENT )
		{
			alignment = m_baseAlignment;
		}
		TIKI_ASSERT( m_baseAlignment >= alignment );

		const uint currentSize = m_pCurrent - m_pMemory;
		const uint alignedSize = alignValue( currentSize, alignment );
		if ( alignedSize + sizeInBytes > m_size )
		{
			TIKI_TRACE_ERROR( "[ZoneAllocator] Out of Memory(want to allocate: %u, free: %u, total size: %u).\n", sizeInBytes, ( m_size - ( m_pCurrent - m_pMemory ) ), m_size );
			return nullptr;
		}

		m_pPrevBase = m_pCurrent;
		m_pPrev		= m_pMemory + alignedSize;

		m_pCurrent	= m_pMemory + alignedSize + sizeInBytes;

		return m_pPrev;
	}

	void ZoneAllocator::free( void* pMemory )
	{
		TIKI_ASSERT( m_pCurrent != nullptr );
		TIKI_ASSERT( pMemory != nullptr );

		if ( pMemory == m_pPrev )
		{
			m_pCurrent = static_cast< uint8* >( m_pPrevBase );
		}
	}

	void ZoneAllocator::clear()
	{
		TIKI_ASSERT( m_pCurrent != nullptr );

		m_pPrev		= nullptr;
		m_pPrevBase	= nullptr;

		m_pCurrent	= m_pMemory;
	}
}