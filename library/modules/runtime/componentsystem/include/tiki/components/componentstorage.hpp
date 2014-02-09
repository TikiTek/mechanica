#pragma once
#ifndef __TIKI_COMPONENTSTORAGE_HPP_INCLUDED__
#define __TIKI_COMPONENTSTORAGE_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	struct ComponentState;
	struct ComponentChunkDefinition;

	class ComponentStorage
	{
		TIKI_NONCOPYABLE_CLASS( ComponentStorage );

	public:

		ComponentStorage();
		~ComponentStorage();

		bool				create( uint chunkSize, uint chunkCount );
		void				dispose();

		void				registerComponentType( uint typeId, uint stateSize );
		void				unregiserComponentType( uint typeId );

		ComponentState*		allocateState( uint typeId );
		void				freeState( ComponentState* pState );

	private:

		uint						m_chunkSize;
		uint						m_chunkCount;

		uint8*						m_pMemory;
		ComponentChunkDefinition*	m_pChunks;

	};
}

#endif // __TIKI_COMPONENTSTORAGE_HPP_INCLUDED__
