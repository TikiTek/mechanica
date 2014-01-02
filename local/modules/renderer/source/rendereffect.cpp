
#include "tiki/renderer/rendereffect.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	RenderEffect::RenderEffect()
	{
		m_pFrameData		= nullptr;
		m_pRendererContext	= nullptr;
	}

	RenderEffect::~RenderEffect()
	{
		TIKI_ASSERT( m_pFrameData == nullptr );
		TIKI_ASSERT( m_pRendererContext = nullptr );
	}

	bool RenderEffect::create( const RendererContext& rendererContext )
	{
		m_pRendererContext = &rendererContext;

		return createInternal();
	}

	void RenderEffect::dispose()
	{
		disposeInternal();

		m_pFrameData		= nullptr;
		m_pRendererContext	= nullptr;
	}

	void RenderEffect::setFrameData( const FrameData& frameData )
	{
		m_pFrameData = &frameData;
	}

	void RenderEffect::executeRenderSequences( RenderPass pass, const RenderSequence* pSequences, uint sequenceCount )
	{
		TIKI_ASSERT( m_pFrameData != nullptr );
		TIKI_ASSERT( m_pRendererContext = nullptr );

		executeRenderSequencesInternal( pass, pSequences, sequenceCount );
	}
}