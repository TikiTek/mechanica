#pragma once

#include "tiki/container/array.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	struct Chunk;

	typedef uint8 ChunkTypeId;

	enum
	{
		InvalidChunkTypeId = 0xffu
	};

	struct ChunkState
	{
		ChunkTypeId		typeId;

		ChunkState*		pPrevStateOfSameType;
		ChunkState*		pNextStateOfSameType;
	};

	struct ChunkedStorageParameters
	{
		ChunkedStorageParameters()
		{
			chunkSize		= 4096u;
			chunkAlignment	= TIKI_DEFAULT_ALIGNMENT;
			chunkCount		= 32u;

			typeCount		= 32u;
		}

		uint chunkSize;
		uint chunkAlignment;
		uint chunkCount;

		uint typeCount;
	};

	class ChunkedStorage
	{
		TIKI_NONCOPYABLE_CLASS( ChunkedStorage );

	public:

								ChunkedStorage();
								~ChunkedStorage();

		bool					create( const ChunkedStorageParameters& parameters );
		void					dispose();

		ChunkTypeId				registerType( uint stateSize );
		void					unregisterType( ChunkTypeId typeId );

		ChunkState*				allocateState( ChunkTypeId typeId );
		void					freeState( ChunkState* pState );

	private:

		Array< uint >			m_typeRegister;

		uint					m_chunkSize;

		Array< uint8 >			m_memory;
		Array< Chunk >			m_chunks;
		Array< Chunk* >			m_firstChunk;
		Array< ChunkState* >	m_lastState;
	};
}
