#ifndef TIKI_GRAPHICCOMMANDWRITER_HPP__
#define TIKI_GRAPHICCOMMANDWRITER_HPP__

#include "tiki/base/array.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/basebuffer.hpp"
#include "tiki/graphicsbase/color.hpp"
#include "tiki/graphicsbase/graphissystemlimits.hpp"
#include "tiki/graphicsbase/primitivetopologies.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "win_d3d11/graphicscontext_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "global_opengl4/graphicscontext_d3d11.hpp"
#else
#	error Platform not supported
#endif

namespace tiki
{
	class BaseBuffer;
	class BlendState;
	class ConstantBuffer;
	class DepthStencilState;
	class GraphicsSystem;
	class IndexBuffer;
	class RasterizerState;
	class RenderTarget;
	class SamplerState;
	class Shader;
	class TextureData;
	class VertexBuffer;
	class VertexFormat;
	class VertexInputBinding;
	struct GraphicsHandles;
	struct Rectangle;
	struct Vector2;
	struct Vector3;

	enum ClearMask
	{
		ClearMask_Color0		= 1u << 0u,
		ClearMask_Color1		= 1u << 1u,
		ClearMask_Color2		= 1u << 2u,
		ClearMask_Color3		= 1u << 3u,
		ClearMask_Color4		= 1u << 4u,
		ClearMask_Color5		= 1u << 5u,
		ClearMask_Color6		= 1u << 6u,
		ClearMask_Color7		= 1u << 7u,

		ClearMask_Depth			= 1u << 8u,
		ClearMask_Stencil		= 1u << 9u,

		ClearMask_Color			= ClearMask_Color0 | ClearMask_Color1 | ClearMask_Color2 | ClearMask_Color3 | ClearMask_Color4 | ClearMask_Color5 | ClearMask_Color6 | ClearMask_Color7,
		ClearMask_DepthStencil	= ClearMask_Depth | ClearMask_Stencil,
		ClearMask_All			= ClearMask_Color | ClearMask_DepthStencil,
	};
	TIKI_COMPILETIME_ASSERT( GraphicsSystemLimits_RenderTargetSlots == 8u );

	class GraphicsContext
	{
		friend class GraphicsSystem;

	public:

		void				clear( const RenderTarget& renderTarget, Color color = TIKI_COLOR_BLACK, float depthValue = 1.0f, uint8 stencilValue = 0u, ClearMask clearMask = ClearMask_All );

		void				beginRenderPass( const RenderTarget& renderTarget );
		void				endRenderPass();

		void				setVertexShader( const Shader* pVertexShader );
		void				setPixelShader( const Shader* pPixelShader );
		void				setVertexInputBinding( const VertexInputBinding* pVertexInputBinding );

		void				setBlendState( const BlendState* pBlendState );
		void				setDepthStencilState( const DepthStencilState* pDepthStencilState );
		void				setRasterizerState( const RasterizerState* pRasterizerState );

		void				setIndexBuffer( const IndexBuffer& buffer );
		void				setVertexBuffer( uint slot, const VertexBuffer& buffer );

		void				setVertexShaderSamplerState( uint slot, const SamplerState* pSampler );
		void				setVertexShaderTexture( uint slot, const TextureData* pTextureData );
		void				setVertexShaderConstant( uint slot, const ConstantBuffer& buffer );

		void				setPixelShaderSamplerState( uint slot, const SamplerState* pSampler );
		void				setPixelShaderTexture( uint slot, const TextureData* pTextureData );
		void				setPixelShaderConstant( uint slot, const ConstantBuffer& buffer );

		void				setPrimitiveTopology( PrimitiveTopology topology );

		void*				beginImmediateGeometry( uint vertexStride, uint vertexCount );
		void				endImmediateGeometry();

		void				drawGeometry( uint vertexCount, uint baseVertexOffset = 0u );
		void				drawIndexedGeometry( uint indexCount, uint baseIndexOffset = 0u, uint baseVertexOffset = 0u );

		void*				mapBuffer( BaseBuffer& buffer );
		void				unmapBuffer( BaseBuffer& buffer );

		const RenderTarget&	getBackBuffer() const;

	private: // friend

							GraphicsContext();
							~GraphicsContext();

		bool				create( GraphicsSystem& graphicsSystem );
		void				dispose( GraphicsSystem& graphicsSystem );

	private:

		enum MyEnum
		{
			MaxImmediateGeometrySize = 10240u
		};

		GraphicsSystem*				m_pGraphicsSystem;
		GraphicsContextPlatformData	m_platformData;

		const RenderTarget*			m_pRenderTarget;

		const RenderTarget*			m_pRenderPassesStack[ GraphicsSystemLimits_RenderPassStackDepth ];
		uint						m_currentRenderPassDepth;

		const BlendState*			m_pBlendState;
		const DepthStencilState*	m_pDepthStencilState;
		const RasterizerState*		m_pRasterizerState;

		const Shader*				m_pVertexShader;
		const VertexInputBinding*	m_pVertexInputBinding;
		const SamplerState*			m_pVertexSamplerStates[ GraphicsSystemLimits_VertexShaderTextureSlots ];
		const TextureData*			m_pVertexTextures[ GraphicsSystemLimits_VertexShaderTextureSlots ];
		const ConstantBuffer*		m_pVertexConstants[ GraphicsSystemLimits_VertexShaderConstantSlots ];

		const Shader*				m_pPixelShader;
		const SamplerState*			m_pPixelSamplerStates[ GraphicsSystemLimits_PixelShaderTextureSlots ];
		const TextureData*			m_pPixelTextures[ GraphicsSystemLimits_PixelShaderTextureSlots ];
		const ConstantBuffer*		m_pPixelConstants[ GraphicsSystemLimits_PixelShaderConstantSlots ];

		BaseBuffer					m_immediateVertexData;
		uint						m_immediateVertexStride;
		uint						m_immediateVertexCount;

		void						invalidateState();
		bool						validateDrawCall() const;

	};
}

#endif // TIKI_GRAPHICCOMMANDWRITER_HPP__


