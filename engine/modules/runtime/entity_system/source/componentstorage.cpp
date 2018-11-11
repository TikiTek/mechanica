
#include "tiki/entitysystem/componentstorage.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/entitysystem/componenttyperegister.hpp"

namespace tiki
{
	struct ComponentChunk
	{
		ComponentChunk*		pNextChunkOfSameType;

		uint8*				pData;
		ComponentState*		pFirstFreeState;
		ComponentState*		pLastFreeState;
		
		ComponentTypeId		typeId;
		uint16				usedCount;
		uint16				maxCount;
	};

	ComponentStorage::ComponentStorage()
	{
		m_pMemory = nullptr;
	}

	ComponentStorage::~ComponentStorage()
	{
		TIKI_ASSERT( m_pMemory == nullptr );
	}

	bool ComponentStorage::create( uint chunkSize, uint chunkCount, const ComponentTypeRegister& typeRegister )
	{
		if ( chunkSize < MinChunkSize || chunkSize > MaxChunkSize || !isValueAligned( chunkSize, (uint)ChunkAlignment ) )
		{
			return false;
		}

		m_pTypeRegister = &typeRegister;
		m_chunkCount	= chunkCount;
		m_chunkSize		= chunkSize;

		const uint dataSize = chunkSize * chunkCount;
		m_pMemory = static_cast< uint8* >( TIKI_ALLOC_ALIGNED( dataSize, ChunkAlignment ) );

		if ( !m_chunks.create( chunkCount ) )
		{
			dispose();
			return false;
		}

		const uint typeCount = typeRegister.getMaxTypeCount();
		if ( !m_firstChunk.create( typeCount ) || !m_lastState.create( typeCount ) )
		{
			dispose();
			return false;
		}

		for (uint i = 0u; i < m_firstChunk.getCount(); ++i)
		{
			m_firstChunk[ i ] = nullptr;
			m_lastState[ i ] = nullptr;
		}

		for (uint i = 0u; i < chunkCount; ++i)
		{
			ComponentChunk& chunk = m_chunks[ i ];

			chunk.pNextChunkOfSameType	= nullptr;
			chunk.pData					= m_pMemory + ( chunkSize * i );

			chunk.typeId				= InvalidComponentTypeId;
			chunk.usedCount				= 0u;
			chunk.maxCount				= 0u;
		}

		return true;
	}

	void ComponentStorage::dispose()
	{
		if ( m_pMemory != nullptr )
		{
			TIKI_FREE( m_pMemory );
			m_pMemory = nullptr;
		}

		m_chunks.dispose();
		m_firstChunk.dispose();
		m_lastState.dispose();
	}

	ComponentState* ComponentStorage::allocateState( ComponentTypeId typeId )
	{
		TIKI_ASSERT( typeId != InvalidComponentTypeId );
		TIKI_ASSERT( m_pTypeRegister != nullptr );

		ComponentChunk* pChunk = m_firstChunk[ typeId ];

		ComponentChunk* pLastFullChunk = nullptr;
		while ( pChunk != nullptr && pChunk->usedCount == pChunk->maxCount )
		{
			pLastFullChunk	= pChunk;
			pChunk			= pChunk->pNextChunkOfSameType;
		}

		if ( pChunk == nullptr )
		{
			for (uint chunkIndex = 0u; chunkIndex < m_chunks.getCount(); ++chunkIndex)
			{
				ComponentChunk& chunk = m_chunks[ chunkIndex ];

				if ( chunk.typeId == InvalidComponentTypeId )
				{
					const uint stateSize = m_pTypeRegister->getTypeStateSize( typeId );
					TIKI_ASSERT( stateSize >= sizeof( ComponentState ) );

					chunk.pNextChunkOfSameType	= nullptr;
					chunk.typeId				= typeId;
					chunk.usedCount				= 0u;
					chunk.maxCount				= uint16( m_chunkSize / stateSize );
					TIKI_ASSERT( chunk.maxCount != 0u );

					if ( pLastFullChunk != nullptr )
					{
						pLastFullChunk->pNextChunkOfSameType = &chunk;
					}

					ComponentState* pPrevState = nullptr;
					for (uint stateIndex = 0u; stateIndex < chunk.maxCount; ++stateIndex)
					{
						ComponentState* pState = (ComponentState*)( chunk.pData + ( stateSize * stateIndex ) );

						pState->pPrevComponentOfSameType = pPrevState;
						
						if ( pPrevState != nullptr )
						{
							pPrevState->pNextComponentOfSameType = pState;
						}
						else
						{
							chunk.pFirstFreeState = pState;
						}

						pPrevState = pState;
					}
					chunk.pLastFreeState = pPrevState;
					chunk.pLastFreeState->pNextComponentOfSameType = nullptr;

					pChunk = &chunk;
					break;
				}
			}

			if ( m_firstChunk[ typeId ] == nullptr )
			{
				m_firstChunk[ typeId ] = pChunk;
			}
		}
				
		if ( pChunk == nullptr )
		{
			TIKI_TRACE_ERROR( "Could not allocate ComponentState from type '%s', because of out of memory.\n", m_pTypeRegister->getTypeName( typeId ) );
			return nullptr;
		}
		TIKI_ASSERT( pChunk->usedCount != pChunk->maxCount );

		ComponentState* pState = pChunk->pFirstFreeState;
		pChunk->pFirstFreeState = pState->pNextComponentOfSameType;
		pChunk->usedCount++;

		if ( pChunk->pFirstFreeState == nullptr )
		{
			TIKI_ASSERT( pChunk->usedCount == pChunk->maxCount );
			pChunk->pLastFreeState = nullptr;
		}
		
		pState->entityId					= InvalidEntityId;
		pState->typeId						= typeId;
		pState->pNextComponentOfSameType	= nullptr;
		pState->pNextComponentOfSameEntity	= nullptr;

		if ( m_lastState[ typeId ] == nullptr )
		{
			pState->pPrevComponentOfSameType = nullptr;
			m_lastState[ typeId ] = pState;
		}
		else
		{
			pState->pPrevComponentOfSameType = m_lastState[ typeId ];
			m_lastState[ typeId ]->pNextComponentOfSameType = pState;
			m_lastState[ typeId ] = pState;
		}		

		return pState;
	}

	void ComponentStorage::freeState( ComponentState* pState )
	{
		if ( pState->pPrevComponentOfSameType != nullptr )
		{
			pState->pPrevComponentOfSameType->pNextComponentOfSameType = pState->pNextComponentOfSameType;
		}

		if ( pState->pNextComponentOfSameType != nullptr )
		{
			pState->pNextComponentOfSameType->pPrevComponentOfSameType = pState->pPrevComponentOfSameType;
		}

		const uint chunkIndex = (uint)( ( (size_t)pState - (size_t)m_pMemory ) / m_chunkSize );
		ComponentChunk& chunk = m_chunks[ chunkIndex ];
		TIKI_ASSERT( chunk.typeId == pState->typeId );

		if ( m_lastState[ chunk.typeId ] == pState )
		{
			TIKI_ASSERT( pState->pNextComponentOfSameType == nullptr );
			m_lastState[ chunk.typeId ] = pState->pPrevComponentOfSameType;
		}

		if ( chunk.pLastFreeState == nullptr )
		{
			chunk.pFirstFreeState	= pState;
			chunk.pLastFreeState	= pState;

			pState->pPrevComponentOfSameType = nullptr;
			pState->pNextComponentOfSameType = nullptr;
		}
		else
		{
			pState->pPrevComponentOfSameType = chunk.pLastFreeState;
			pState->pNextComponentOfSameType = nullptr;

			chunk.pLastFreeState->pNextComponentOfSameType = pState;
			chunk.pLastFreeState = pState;
		}

		pState->entityId					= InvalidEntityId;
		pState->typeId						= InvalidComponentTypeId;
		pState->pNextComponentOfSameEntity	= nullptr;
				
		chunk.usedCount--;
	}
}