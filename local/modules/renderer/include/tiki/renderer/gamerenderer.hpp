#pragma once
#ifndef __TIKI_GAMERENDERER_HPP_INCLUDED__
#define __TIKI_GAMERENDERER_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/renderer/renderbatch.hpp"
#include "tiki/renderer/rendereffectsystem.hpp"
#include "tiki/renderer/renderercontext.hpp"

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class Model;

	struct  GameRendererParamaters
	{
		GameRendererParamaters()
		{
			rendererWidth	= 1280u;
			rendererHeight	= 720u;

			nearPlane	= 0.001f;
			farPlane	= 100.0f;

			maxSeqeuenceCount		= 100u;
			maxRenderCommandCount	= 500u;
		}

		uint	rendererWidth;
		uint	rendererHeight;

		float	nearPlane;
		float	farPlane;

		uint	maxSeqeuenceCount;
		uint	maxRenderCommandCount;
	};

	class GameRenderer
	{
		TIKI_NONCOPYABLE_CLASS( GameRenderer );

	public:

		GameRenderer();
		~GameRenderer();

		bool				create( GraphicsSystem& graphicsSystem, const GameRendererParamaters& parameters );
		void				dispose();

		bool				resize( uint width, uint height );

		void				registerRenderEffect( RenderEffect* pRenderEffect );
		void				unregisterRenderEffect( RenderEffect* pRenderEffect );

		FrameData&			getFrameData()			{ return m_frameData; }
		RendererContext&	getRendererContext()	{ return m_context; }

		const TextureData&	getGeometryBufferBxIndex( uint index ) const	{ return m_geometryBufferData[ index ]; }
		const TextureData&	getDepthBuffer() const							{ return m_readyOnlyDepthBuffer; }

		void				queueModel( const Model* pModel, const Matrix43* pWorldTransform = nullptr );

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
		FrameData			m_frameData;

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

		void				disposeTextureData();
		void				disposeRenderTargets();

	};
}

#endif // __TIKI_GAMERENDERER_HPP_INCLUDED__
