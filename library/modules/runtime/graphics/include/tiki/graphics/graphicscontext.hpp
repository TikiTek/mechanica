#ifndef TIKI_GRAPHICCOMMANDWRITER_HPP__
#define TIKI_GRAPHICCOMMANDWRITER_HPP__

#include "tiki/base/array.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/staticarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/graphicssystemlimits.hpp"
#include "tiki/graphics/primitivetopologies.hpp"
#include "tiki/graphics/vertexbuffer.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/graphicscontext_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/graphicscontext_d3d12.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "../../../source/global_opengl4/graphicscontext_opengl4.hpp"
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
	struct Viewport;

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
		void				copyTextureData( const TextureData& sourceData, const TextureData& targetData );

		void				beginRenderPass( const RenderTarget& renderTarget, const Viewport* pViewport = nullptr );
		void				endRenderPass();

		void				setVertexShader( const Shader* pVertexShader );
		void				setPixelShader( const Shader* pPixelShader );
		void				setVertexInputBinding( const VertexInputBinding* pVertexInputBinding );

		void				setBlendState( const BlendState* pBlendState );
		void				setDepthStencilState( const DepthStencilState* pDepthStencilState );
		void				setRasterizerState( const RasterizerState* pRasterizerState );

		void				setIndexBuffer( const IndexBuffer& indexBuffer );
		void				setVertexBuffer( uint slot, const VertexBuffer& vertexBuffer );

		void				setVertexShaderSamplerState( uint slot, const SamplerState* pSampler );
		void				setVertexShaderTexture( uint slot, const TextureData* pTextureData );
		void				setVertexShaderConstant( uint slot, const ConstantBuffer& constantBuffer );

		void				setPixelShaderSamplerState( uint slot, const SamplerState* pSampler );
		void				setPixelShaderTexture( uint slot, const TextureData* pTextureData );
		void				setPixelShaderConstant( uint slot, const ConstantBuffer& constantBuffer );

		void				setPrimitiveTopology( PrimitiveTopology topology );

		void*				beginImmediateGeometry( uint vertexStride, uint vertexCount );
		template<typename T>
		void				beginImmediateGeometry( StaticArray< T >& targetArray, uint vertexCount );
		void				endImmediateGeometry();

		void				drawGeometry( uint vertexCount, uint baseVertexOffset = 0u );
		void				drawIndexedGeometry( uint indexCount, uint baseIndexOffset = 0u, uint baseVertexOffset = 0u );

		void				drawFullScreenQuadPos2();
		void				drawFullScreenQuadPos2Tex2( float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f );
		void				drawFullScreenQuadPos3( float z = 0.0f );
		void				drawFullScreenQuadPos3Tex2( float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f, float z = 0.0f );

		void*				mapBuffer( const BaseBuffer& buffer );
		template<typename T>
		T*					mapBuffer( const BaseBuffer& buffer );
		void				unmapBuffer( const BaseBuffer& buffer );


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

		const RenderTarget*			m_apRenderPassesStack[ GraphicsSystemLimits_RenderPassStackDepth ];
		uint						m_currentRenderPassDepth;

		PrimitiveTopology			m_primitiveTopology;

		const BlendState*			m_pBlendState;
		const DepthStencilState*	m_pDepthStencilState;
		const RasterizerState*		m_pRasterizerState;

		const Shader*				m_pVertexShader;
		const VertexInputBinding*	m_pVertexInputBinding;
		const SamplerState*			m_apVertexSamplerStates[ GraphicsSystemLimits_VertexShaderTextureSlots ];
		const TextureData*			m_apVertexTextures[ GraphicsSystemLimits_VertexShaderTextureSlots ];
		const ConstantBuffer*		m_apVertexConstants[ GraphicsSystemLimits_VertexShaderConstantSlots ];

		const Shader*				m_pPixelShader;
		const SamplerState*			m_apPixelSamplerStates[ GraphicsSystemLimits_PixelShaderTextureSlots ];
		const TextureData*			m_apPixelTextures[ GraphicsSystemLimits_PixelShaderTextureSlots ];
		const ConstantBuffer*		m_apPixelConstants[ GraphicsSystemLimits_PixelShaderConstantSlots ];

		VertexBuffer				m_immediateVertexData;
		uint						m_immediateVertexStride;
		uint						m_immediateVertexCount;

		void						invalidateState();
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		bool						validateDrawCall() const;
#endif

		void						prepareDrawCall();
	};
}

#include "../../../source/global/graphicscontext.inl"

#endif // TIKI_GRAPHICCOMMANDWRITER_HPP__


