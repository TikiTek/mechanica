#pragma once
#ifndef __TIKI_RENDEREFFECT_HPP_INCLUDED__
#define __TIKI_RENDEREFFECT_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	enum RenderPass;
	struct FrameData;
	struct RenderSequence;
	struct RendererContext;

	class RenderEffect
	{
		TIKI_NONCOPYABLE_CLASS( RenderEffect );

	public:

		virtual void	setFrameData( const FrameData& frameData ) = 0;
		virtual void	setRendererContext( const RendererContext& rendererContext ) = 0;

		virtual void	executeRenderCommands( RenderPass pass, const RenderSequence* pSequences, uint sequenceCount ) = 0;

	};
}

#endif // __TIKI_RENDEREFFECT_HPP_INCLUDED__
