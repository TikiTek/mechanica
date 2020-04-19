#include "tiki/container/chunked_storage.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"

namespace tiki
{
	struct Chunk
	{
		Chunk*			pNextChunkOfSameType;

		uint8*			pData;
		ChunkState*		pFirstFreeState;
		ChunkState*		pLastFreeState;

		ChunkTypeId		typeId;
		uint16			usedCount;
		uint16			maxCount;
	};

	ChunkedStorage::ChunkedStorage()
	{
	}

	ChunkedStorage::~ChunkedStorage()
	{
	}

	bool ChunkedStorage::create( const ChunkedStorageParameters& parameters )
	{
		if ( !isValueAligned( parameters.chunkSize, parameters.chunkAlignment ) )
		{
			return false;
		}

		m_chunkSize = parameters.chunkSize;

		const uintreg dataSize = parameters.chunkSize * parameters.chunkCount;
		if( !m_typeRegister.create( parameters.typeCount ) ||
			!m_memory.create( dataSize, parameters.chunkAlignment ) ||
			!m_chunks.create( parameters.chunkCount ) ||
			!m_firstChunk.create( parameters.typeCount ) ||
			!m_lastState.create( parameters.typeCount ) )
		{
			dispose();
			return false;
		}

		for( uintreg i = 0u; i < m_typeRegister.getCount(); ++i )
		{
			m_typeRegister[ i ] = 0u;
		}

		for (uintreg i = 0u; i < m_firstChunk.getCount(); ++i)
		{
			m_firstChunk[ i ] = nullptr;
			m_lastState[ i ] = nullptr;
		}

		for (uintreg i = 0u; i < parameters.chunkCount; ++i)
		{
			Chunk& chunk = m_chunks[ i ];

			chunk.pNextChunkOfSameType	= nullptr;
			chunk.pData					= m_memory.getBegin() + (parameters.chunkSize * i );

			chunk.typeId				= InvalidChunkTypeId;
			chunk.usedCount				= 0u;
			chunk.maxCount				= 0u;
		}

		return true;
	}

	void ChunkedStorage::dispose()
	{
		m_chunks.dispose();
		m_firstChunk.dispose();
		m_lastState.dispose();

		m_memory.dispose();

		m_typeRegister.dispose();
	}

	ChunkState* ChunkedStorage::allocateState( ChunkTypeId typeId )
	{
		TIKI_ASSERT( typeId != InvalidChunkTypeId );

		Chunk* pChunk = m_firstChunk[ typeId ];

		Chunk* pLastFullChunk = nullptr;
		while ( pChunk != nullptr && pChunk->usedCount == pChunk->maxCount )
		{
			pLastFullChunk	= pChunk;
			pChunk			= pChunk->pNextChunkOfSameType;
		}

		if ( pChunk == nullptr )
		{
			for (uintreg chunkIndex = 0u; chunkIndex < m_chunks.getCount(); ++chunkIndex)
			{
				Chunk& chunk = m_chunks[ chunkIndex ];

				if ( chunk.typeId == InvalidChunkTypeId )
				{
					const uintreg stateSize = m_typeRegister[ typeId ];
					TIKI_ASSERT( stateSize >= sizeof( ChunkState ) );

					chunk.pNextChunkOfSameType	= nullptr;
					chunk.typeId				= typeId;
					chunk.usedCount				= 0u;
					chunk.maxCount				= uint16( m_chunkSize / stateSize );
					TIKI_ASSERT( chunk.maxCount != 0u );

					if ( pLastFullChunk != nullptr )
					{
						pLastFullChunk->pNextChunkOfSameType = &chunk;
					}

					ChunkState* pPrevState = nullptr;
					for (uintreg stateIndex = 0u; stateIndex < chunk.maxCount; ++stateIndex)
					{
						ChunkState* pState = (ChunkState*)( chunk.pData + ( stateSize * stateIndex ) );

						pState->pPrevStateOfSameType = pPrevState;

						if ( pPrevState != nullptr )
						{
							pPrevState->pNextStateOfSameType = pState;
						}
						else
						{
							chunk.pFirstFreeState = pState;
						}

						pPrevState = pState;
					}
					chunk.pLastFreeState = pPrevState;
					chunk.pLastFreeState->pNextStateOfSameType = nullptr;

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
			TIKI_TRACE_ERROR( "Could not allocate ComponentState from type %i, because of out of memory.\n", typeId );
			return nullptr;
		}
		TIKI_ASSERT( pChunk->usedCount != pChunk->maxCount );

		ChunkState* pState = pChunk->pFirstFreeState;
		pChunk->pFirstFreeState = pState->pNextStateOfSameType;
		pChunk->usedCount++;

		if ( pChunk->pFirstFreeState == nullptr )
		{
			TIKI_ASSERT( pChunk->usedCount == pChunk->maxCount );
			pChunk->pLastFreeState = nullptr;
		}

		pState->typeId					= typeId;
		pState->pNextStateOfSameType	= nullptr;

		if ( m_lastState[ typeId ] == nullptr )
		{
			pState->pPrevStateOfSameType = nullptr;
			m_lastState[ typeId ] = pState;
		}
		else
		{
			pState->pPrevStateOfSameType = m_lastState[ typeId ];
			m_lastState[ typeId ]->pNextStateOfSameType = pState;
			m_lastState[ typeId ] = pState;
		}

		return pState;
	}

	void ChunkedStorage::freeState( ChunkState* pState )
	{
		if ( pState->pPrevStateOfSameType != nullptr )
		{
			pState->pPrevStateOfSameType->pNextStateOfSameType = pState->pNextStateOfSameType;
		}

		if ( pState->pNextStateOfSameType != nullptr )
		{
			pState->pNextStateOfSameType->pPrevStateOfSameType = pState->pPrevStateOfSameType;
		}

		const uintreg chunkIndex = (uintreg)( ( (uintreg)pState - (uintreg)m_memory.getBegin() ) / m_chunkSize );
		Chunk& chunk = m_chunks[ chunkIndex ];
		TIKI_ASSERT( chunk.typeId == pState->typeId );

		if ( m_lastState[ chunk.typeId ] == pState )
		{
			TIKI_ASSERT( pState->pNextStateOfSameType == nullptr );
			m_lastState[ chunk.typeId ] = pState->pPrevStateOfSameType;
		}

		if ( chunk.pLastFreeState == nullptr )
		{
			chunk.pFirstFreeState	= pState;
			chunk.pLastFreeState	= pState;

			pState->pPrevStateOfSameType = nullptr;
			pState->pNextStateOfSameType = nullptr;
		}
		else
		{
			pState->pPrevStateOfSameType = chunk.pLastFreeState;
			pState->pNextStateOfSameType = nullptr;

			chunk.pLastFreeState->pNextStateOfSameType = pState;
			chunk.pLastFreeState = pState;
		}

		pState->typeId = InvalidChunkTypeId;
		chunk.usedCount--;
	}
}