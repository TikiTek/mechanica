#pragma once

#include "tiki/base/memory.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class ZoneAllocator
	{
		TIKI_NONCOPYABLE_CLASS( ZoneAllocator );

	public:

		ZoneAllocator();
		~ZoneAllocator();

		bool		create( uint sizeInBytes, uint alignment = TIKI_MINIMUM_ALIGNMENT );
		void		dispose();

		void*		allocate( uint sizeInBytes, uint alignment = TIKI_DEFAULT_ALIGNMENT );
		void		free( void* pMemory );

		void		clear();

		uint		getCurrentAllocationSize() const { return m_pCurrent - m_pMemory; }

	private:

		uint8*		m_pMemory;
		uint		m_size;
		uint		m_baseAlignment;

		void*		m_pPrev;
		void*		m_pPrevBase;

		uint8*		m_pCurrent;
	};
}
