#pragma once
#ifndef __TIKI_GAMERENDERER_HPP_INCLUDED__
#define __TIKI_GAMERENDERER_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/renderer/renderbatch.hpp"
#include "tiki/renderer/rendereffectsystem.hpp"

namespace tiki
{
	class GraphicsSystem;
	class GraphicsContext;

	struct  GameRendererParamaters
	{
		uint	rendererWidth;
		uint	rendererHeight;
	};

	struct RendererContext
	{

	};

	class GameRenderer
	{
		TIKI_NONCOPYABLE_CLASS( GameRenderer );

	public:

		GameRenderer();
		~GameRenderer();

		bool				create( GraphicsSystem& graphicsSystem, const GameRendererParamaters& parameters );
		void				dispose();

		RenderBatch&		getRenderBatch();
		RenderEffectSystem&	getRenderEffectSystem();

		void				update();
		void				render( GraphicsContext& graphicsContext ) const;

	private:

		enum GeometryBuffer			// | red	| green		| blue			| alpha					|
		{							// |--------|-----------|---------------|-----------------------|
			GeometryBuffer_Target0,	// | diffuseColor.rgb					| specluarBrightness	|
			GeometryBuffer_Target1,	// | selfIllumination.rgb				| specluarIntensity		|
			GeometryBuffer_Target2,	// | normal.xy			| specluarColor	| specluarPower			| // 16 bit?

			GeometryBuffer_Count
		};

		RendererContext		m_context;

		RenderBatch			m_renderBatch;
		RenderEffectSystem	m_renderEffectSystem;

		TextureData			m_depthBuffer;
		TextureData			m_readyOnlyDepthBuffer;
		TextureData			m_geometryBufferData[ GeometryBuffer_Count ];
		TextureData			m_accumulationData;

		RenderTarget		m_geometryTarget;
		RenderTarget		m_accumulationTarget;

		bool				createTextureData();
		bool				createRenderTargets();

	};
}

#endif // __TIKI_GAMERENDERER_HPP_INCLUDED__
