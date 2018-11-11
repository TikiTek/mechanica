#pragma once
#ifndef __TIKI_GAMERENDERER_HPP_INCLUDED__
#define __TIKI_GAMERENDERER_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/renderer/renderbatch.hpp"
#include "tiki/renderer/rendereffectsystem.hpp"
#include "tiki/renderer/renderercontext.hpp"

namespace tiki
{
	class BlendState;
	class DepthStencilState;
	class GraphicsContext;
	class GraphicsSystem;
	class Model;
	class RasterizerState;
	class RenderView;
	class ResourceRequestPool;
	class SamplerState;
	class ShaderSet;
	class Texture;
	class VertexInputBinding;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	enum VisualizationMode
	{
		VisualizationMode_Invalid = -1,

		VisualizationMode_Diffuse,
		VisualizationMode_SelfIllumination,
		VisualizationMode_Normal,

		VisualizationMode_Count
	};
#endif

	struct  GameRendererParamaters
	{
		GameRendererParamaters()
		{
			rendererWidth	= 1280u;
			rendererHeight	= 720u;

			maxSeqeuenceCount		= 256u;
			maxRenderCommandCount	= 1024u;
		}

		uint	rendererWidth;
		uint	rendererHeight;

		uint	maxSeqeuenceCount;
		uint	maxRenderCommandCount;
	};

	class GameRenderer
	{
		TIKI_NONCOPYABLE_CLASS( GameRenderer );

	public:

		GameRenderer();
		~GameRenderer();

		bool				create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool, const GameRendererParamaters& parameters );
		bool				createShaderResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool );
		void				dispose( ResourceRequestPool& resourceRequestPool );

		bool				resize( uint width, uint height );

		bool				registerRenderEffect( RenderEffect& renderEffect, ResourceRequestPool& resourceRequestPool );
		void				unregisterRenderEffect( RenderEffect& renderEffect, ResourceRequestPool& resourceRequestPool );

		RendererContext&	getRendererContext()	{ return m_context; }

		const TextureData&	getGeometryBufferBxIndex( uint index ) const	{ return m_geometryBufferData[ index ]; }
		const TextureData&	getAccumulationBuffer() const					{ return m_accumulationData; }
		const TextureData&	getDepthBuffer() const							{ return m_readOnlyDepthBuffer; }

		const RenderTarget&	getGeometryTarget() const						{ return m_geometryTarget; }

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		VisualizationMode	getVisualizationMode() const					{ return m_visualizationMode; }
		void				setVisualizationMode( VisualizationMode mode )	{ m_visualizationMode = mode; }
#endif

		void				renderView( GraphicsContext& graphicsContext, const RenderTarget& renderTarget, RenderView& view ) const;

	private:

		enum GeometryBuffer			// | red | green | blue	                  | alpha                  |
		{							// |-----|-------|------------------------|------------------------|
			GeometryBuffer_Target0,	// | diffuseColor.rgb                     | selfIlluminationFactor |
			GeometryBuffer_Target1,	// | specluarColor.rgb		              | specluarIntensity      |
			GeometryBuffer_Target2,	// | normal.xy   | glossiness			  | specluarPower          | // 16 bit?

			GeometryBuffer_Count
		};

		RendererContext				m_context;
		RenderEffectSystem			m_effectSystem;

		const BlendState*			m_pBlendStateAdd;
		const BlendState*			m_pBlendStateSet;
		const DepthStencilState*	m_pDepthStencilState;
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSamplerLinear;
		const SamplerState*			m_pSamplerNearst;

		const ShaderSet*			m_pLightingShader;
		const VertexInputBinding*	m_pLightingInputBinding;
		ConstantBuffer				m_lightingPixelConstants;
		ConstantBuffer				m_cameraParameterConstants;

		const Texture*				m_pReflectionTexture;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const ShaderSet*			m_pVisualizationShader;
		const VertexInputBinding*	m_pVisualizationInputBinding;

		VisualizationMode			m_visualizationMode;
#endif

		TextureData					m_depthBuffer;
		TextureData					m_readOnlyDepthBuffer;
		TextureData					m_geometryBufferData[ GeometryBuffer_Count ];
		TextureData					m_accumulationData;

		RenderTarget				m_geometryTarget;
		RenderTarget				m_accumulationTarget;

		bool						createTextureData();
		bool						createRenderTargets();

		void						disposeTextureData();
		void						disposeRenderTargets();

		void						renderGeometry( GraphicsContext& graphicsContext, const RenderView& view ) const;
		void						renderLighting( GraphicsContext& graphicsContext, const RenderTarget& renderTarget, const RenderView& view ) const;
		void						renderVisualization( GraphicsContext& graphicsContext, const RenderTarget& renderTarget, const RenderView& view ) const;
	};
}

#endif // __TIKI_GAMERENDERER_HPP_INCLUDED__
