#pragma once
#ifndef __TIKI_COMPONENTSTORAGE_HPP_INCLUDED__
#define __TIKI_COMPONENTSTORAGE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/components/component_types.hpp"
#include "tiki/container/array.hpp"

namespace tiki
{
	class ComponentTypeRegister;
	struct ComponentChunk;
	struct ComponentState;

	class ComponentStorage
	{
		TIKI_NONCOPYABLE_CLASS( ComponentStorage );

	public:

		ComponentStorage();
		~ComponentStorage();

		bool				create( uint chunkSize, uint chunkCount, const ComponentTypeRegister& typeRegister );
		void				dispose();

		ComponentState*		allocateState( ComponentTypeId typeId );
		void				freeState( ComponentState* pState );

	private:

		enum
		{
			ChunkAlignment	= 128u,
			MinChunkSize	= 1024u,
			MaxChunkSize	= 8192u
		};

		const ComponentTypeRegister*	m_pTypeRegister;

		uint							m_chunkSize;
		uint							m_chunkCount;

		uint8*							m_pMemory;

		Array< ComponentChunk >			m_chunks;
		Array< ComponentChunk* >		m_firstChunk;
		Array< ComponentState* >		m_lastState;

	};
}

#endif // __TIKI_COMPONENTSTORAGE_HPP_INCLUDED__
