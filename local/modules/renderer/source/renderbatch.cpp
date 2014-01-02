
#include "tiki/renderer/renderbatch.hpp"

namespace tiki
{
	RenderBatch::RenderBatch()
	{
		m_pFirstSequence	= nullptr;
		m_sequenceCount		= 0u;

		m_currentSequenceFailed	= false;
		m_pCurrentSequence		= nullptr;
		m_pFirstSequenceCommand	= nullptr;
	}

	RenderBatch::~RenderBatch()
	{
		TIKI_ASSERT( m_currentSequenceFailed == false );
		TIKI_ASSERT( m_pCurrentSequence == nullptr );
		TIKI_ASSERT( m_pFirstSequenceCommand == nullptr );
	}

	bool RenderBatch::create( uint sequenceCount, uint commandCount )
	{
		TIKI_ASSERT( m_currentSequenceFailed == false );
		TIKI_ASSERT( m_pCurrentSequence == nullptr );
		TIKI_ASSERT( m_pFirstSequenceCommand == nullptr );

		if ( m_sequences.create( sizeof( RenderSequence ) * sequenceCount ) == false )
		{
			return false;
		}

		if ( m_commands.create( sizeof( RenderCommand ) * commandCount ) == false )
		{
			return false;
		}

		return true;
	}

	void RenderBatch::dispose()
	{
		TIKI_ASSERT( m_currentSequenceFailed == false );
		TIKI_ASSERT( m_pCurrentSequence == nullptr );
		TIKI_ASSERT( m_pFirstSequenceCommand == nullptr );

		m_sequences.dispose();
		m_commands.dispose();
	}

	void RenderBatch::reset()
	{
		m_pFirstSequence	= nullptr;
		m_sequenceCount		= 0u;

		m_currentSequenceFailed	= false;
		m_pCurrentSequence		= nullptr;
		m_pFirstSequenceCommand	= nullptr;

		m_sequences.clear();
		m_commands.clear();
	}

	void RenderBatch::beginSequence( uint8 renderEffectId )
	{
		TIKI_ASSERT( m_pCurrentSequence == nullptr );

		m_pCurrentSequence = static_cast< RenderSequence* >( m_sequences.allocate( sizeof( RenderSequence ), 1u ) );
		
		if ( m_pCurrentSequence == nullptr )
		{
			return;
		}

		if ( m_pFirstSequence == nullptr )
		{
			m_pFirstSequence = m_pCurrentSequence;
		}

		m_sequenceCount++;
		m_currentSequenceFailed = false;
		m_pFirstSequenceCommand = nullptr;

		m_pCurrentSequence->renderEffectId	= renderEffectId;
		m_pCurrentSequence->commandCount	= 0u
		m_pCurrentSequence->sortValue		= 0u;
		m_pCurrentSequence->pCommands		= nullptr;
	}

	void RenderBatch::queueGeometry( const ModelGeometry* pGeometry, const Matrix43* pWorldTransform /*= nullptr */ )
	{
		if ( m_pCurrentSequence == nullptr )
		{
			return;
		}

		RenderCommand* pCommand = static_cast< RenderCommand* >( m_commands.allocate( sizeof( RenderCommand ), 1u ) );
		if ( pCommand == nullptr )
		{
			return;
		}

		pCommand->pGeometry	= pGeometry;
		if ( pWorldTransform == nullptr )
		{
			matrix::createIdentity( pCommand->worldTransform );
		}
		else
		{
			pCommand->worldTransform = *pWorldTransform;
		}

		if ( m_pCurrentSequence->pCommands == nullptr )
		{
			m_pCurrentSequence->pCommands = pCommand;
		}
		TIKI_ASSERT( pCommand == m_pCurrentSequence->pCommands + m_pCurrentSequence->commandCount );
		m_pCurrentSequence->commandCount++;
	}

	bool RenderBatch::endSequence()
	{
		if ( m_pCurrentSequence == nullptr )
		{
			return false;
		}

		m_currentSequenceFailed	= false;
		m_pCurrentSequence		= nullptr;
		m_pFirstSequenceCommand	= nullptr;

		return true;
	}
}