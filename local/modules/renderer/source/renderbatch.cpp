
#include "tiki/renderer/renderbatch.hpp"

#include "tiki/base/functions.hpp"
#include "tiki/graphics/material.hpp"
#include "tiki/graphics/model.hpp"

#include "renderer.hpp"

namespace tiki
{
	RenderBatch::RenderBatch()
	{
		m_pFirstSequence	= nullptr;
		m_pEndSequence		= nullptr;

		m_currentSequenceFailed	= false;
		m_pCurrentSequence		= nullptr;
	}

	RenderBatch::~RenderBatch()
	{
		TIKI_ASSERT( m_currentSequenceFailed == false );
		TIKI_ASSERT( m_pCurrentSequence == nullptr );
	}

	bool RenderBatch::create( uint sequenceCount, uint commandCount )
	{
		TIKI_ASSERT( m_currentSequenceFailed == false );
		TIKI_ASSERT( m_pCurrentSequence == nullptr );

		if ( m_sequences.create( sizeof( RenderSequence ) * sequenceCount, TIKI_ALIGNOF( RenderSequence ) ) == false )
		{
			return false;
		}

		if ( m_commands.create( sizeof( RenderCommand ) * commandCount, TIKI_ALIGNOF( RenderCommand ) ) == false )
		{
			return false;
		}

		return true;
	}

	void RenderBatch::dispose()
	{
		TIKI_ASSERT( m_currentSequenceFailed == false );
		TIKI_ASSERT( m_pCurrentSequence == nullptr );

		m_sequences.dispose();
		m_commands.dispose();
	}

	void RenderBatch::reset()
	{
		m_pFirstSequence	= nullptr;
		m_pEndSequence		= nullptr;

		m_currentSequenceFailed	= false;
		m_pCurrentSequence		= nullptr;

		m_sequences.clear();
		m_commands.clear();
	}

	void RenderBatch::beginSequence( RenderPassMask passMask, RenderEffectId renderEffectId, uint8 renderFlags )
	{
		TIKI_ASSERT( m_pCurrentSequence == nullptr );

		m_pCurrentSequence = static_cast< RenderSequence* >( m_sequences.allocate( sizeof( RenderSequence ), 1u ) );
		
		if ( m_pCurrentSequence == nullptr )
		{
			return;
		}

		if ( m_pFirstSequence == nullptr )
		{
			m_pFirstSequence	= m_pCurrentSequence;
			m_pEndSequence		= m_pCurrentSequence;
		}

		m_pEndSequence++;
		m_currentSequenceFailed = false;

		m_pCurrentSequence->renderEffectId	= renderEffectId;
		m_pCurrentSequence->renderPassMask	= passMask;
		m_pCurrentSequence->renderFlags		= renderFlags;
		m_pCurrentSequence->commandCount	= 0u;
		m_pCurrentSequence->sortValue		= 0u;
		m_pCurrentSequence->pCommands		= nullptr;
	}

	void RenderBatch::queueModel( const Model* pModel, const Matrix43* pWorldTransform /*= nullptr*/, const SkinningData** ppSkinningData /*= nullptr*/ )
	{
		beginSequence( RenderPassMask_Geometry, (RenderEffectId)pModel->getMaterial()->getData()->renderEffectId, 0u );
		for( uint i = 0u; i < pModel->getGeometryCount(); ++i )
		{
			const SkinningData* pSkinningData = (ppSkinningData != nullptr ? ppSkinningData[ i ] : nullptr);
			queueGeometry( pModel->getGeometryByIndex( i ), pModel->getMaterial(), pWorldTransform, pSkinningData );
		}
		endSequence();
	}

	void RenderBatch::queueGeometry( const ModelGeometry& geometry, const Material* pMaterial, const Matrix43* pWorldTransform /*= nullptr */, const SkinningData* pSkinningData /*= nullptr*/ )
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

		pCommand->pGeometry		= &geometry;
		pCommand->pSkinningData	= pSkinningData;

		if ( pMaterial == nullptr )
		{
			static RenderEffectData fallbackRenderEffectData = { (uint16)RenderEffectId_Invalid, 0xffffu, 0x0000u, RenderEffectValues_InvalidTextureOffset };
			pCommand->pRenderEffectData = &fallbackRenderEffectData;
		}
		else
		{
			pCommand->pRenderEffectData = pMaterial->getData();
		}

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

		return true;
	}

	RenderSequenceEnumerator RenderBatch::createEnumerator( RenderPass pass ) const
	{
		return RenderSequenceEnumerator( m_pFirstSequence, m_pEndSequence, pass );
	}

	RenderSequenceEnumerator::RenderSequenceEnumerator( const RenderSequence* pBegin, const RenderSequence* pEnd, RenderPass pass )
	{
		TIKI_ASSERT( ( pBegin != nullptr && pEnd != nullptr ) || ( pBegin == nullptr && pEnd == nullptr ) );
		TIKI_ASSERT( pass != RenderPass_Invalid );

		m_renderPass = pass;

		m_pBeginSequence	= pBegin;
		m_pEndSequence		= pEnd;

		m_pIterator			= pBegin;
	}

	const RenderSequence* RenderSequenceEnumerator::enumerate()
	{
		TIKI_ASSERT( m_renderPass != RenderPass_Invalid );
		TIKI_ASSERT( m_pIterator != nullptr || m_pBeginSequence == nullptr );

		if ( m_pIterator == m_pEndSequence )
		{
			return nullptr;
		}

		while ( isBitSet( m_pIterator->renderPassMask, m_renderPass ) == false )
		{
			m_pIterator++;

			if ( m_pIterator == m_pEndSequence )
			{
				return nullptr;
			}
		}

		return m_pIterator++;
	}

}