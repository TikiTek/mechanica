
#include "tiki/renderer/rendereffect.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	RenderEffect::RenderEffect()
	{
	}

	RenderEffect::~RenderEffect()
	{
	}

	bool RenderEffect::create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool )
	{
		return createInternal( graphicsSystem, resourceRequestPool );
	}

	bool RenderEffect::createShaderResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool )
	{
		return createShaderResourcesInternal( graphicsSystem, resourceRequestPool );
	}

	void RenderEffect::dispose( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool )
	{
		disposeInternal( graphicsSystem, resourceRequestPool );
	}

	void RenderEffect::executeRenderSequences( GraphicsContext& graphisContext, const RendererContext& rendererContext, const RenderView& view, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount )
	{
		executeRenderSequencesInternal( graphisContext, pass, pSequences, sequenceCount, view, rendererContext );
	}
}