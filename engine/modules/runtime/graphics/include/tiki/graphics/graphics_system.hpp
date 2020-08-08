#pragma once

#include "tiki/base/types.hpp"
#include "tiki/container/linked_list.hpp"
#include "tiki/graphics/blend_state.hpp"
#include "tiki/graphics/depth_stencil_state.hpp"
#include "tiki/graphics/graphics_context.hpp"
#include "tiki/graphics/graphics_state_object.hpp"
#include "tiki/graphics/graphics_state_object_collection.hpp"
#include "tiki/graphics/rasterizer_state.hpp"
#include "tiki/graphics/render_target.hpp"
#include "tiki/graphics/sampler_state.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/texture_data.hpp"
#include "tiki/graphics/vertex_format.hpp"
#include "tiki/graphics/vertex_input_binding.hpp"

#include "tiki/graphics/shader/graphicstypes.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/graphics_system_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/graphics_system_d3d12.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_VULKAN )
#	include "../../../source/global_vulkan/graphics_system_vulkan.hpp"
#else
#	error Platform not implemented
#endif

#include "base.hpp"

namespace tiki
{
	enum StockVertexFormat
	{
		StockVertexFormat_Pos2,
		StockVertexFormat_Pos2Tex2,
		StockVertexFormat_Pos3,
		StockVertexFormat_Pos3Tex2,
		StockVertexFormat_Count
	};

	struct GraphicsSystemParameters
	{
		GraphicsSystemParameters()
		{
			backBufferWidth		= 0u;
			backBufferHeight	= 0u;
			backBufferFormat	= PixelFormat_Invalid;
			fullScreen			= false;

			pWindowHandle		= nullptr;
		}

		uint16					backBufferWidth;
		uint16					backBufferHeight;
		PixelFormat				backBufferFormat;
		bool					fullScreen;

		void*					pWindowHandle;
	};

	class GraphicsSystem
	{
		friend class GraphicsContext;
		friend class GraphicsSystemPlatform;
		TIKI_NONCOPYABLE_CLASS( GraphicsSystem );

	public:

		GraphicsSystem();
		~GraphicsSystem();

		bool						create( const GraphicsSystemParameters& params );
		void						dispose();

		bool						resize( uint16 width, uint16 height );

		const BlendState*			createBlendState( const BlendStateParamters& creationParameters );
		const BlendState*			createBlendState( bool blendEnabled, Blend sourceBlend = Blend_One, Blend destinationBlend = Blend_Zero, BlendOperation operation = BlendOperation_Add, ColorWriteMask colorWriteMask = ColorWriteMask_All );
		void						disposeBlendState( const BlendState*& pBlendState );

		const DepthStencilState*	createDepthStencilState( const DepthStencilStateParamters& creationParameters );
		const DepthStencilState*	createDepthStencilState( bool depthEnbaled, bool depthWriteEnabled );
		void						disposeDepthStencilState( const DepthStencilState*& pDepthStencilState );

		const RasterizerState*		createRasterizerState( const RasterizerStateParamters& creationParameters );
		const RasterizerState*		createRasterizerState( FillMode fillMode = FillMode_Solid, CullMode cullMode = CullMode_Back, WindingOrder windingOrder = WindingOrder_Clockwise );
		void						disposeRasterizerState( const RasterizerState*& pRasterizerState );

		const SamplerState*			createSamplerState( const SamplerStateParamters& creationParameters );
		const SamplerState*			createSamplerState( AddressMode addressU = AddressMode_Clamp, AddressMode addressV = AddressMode_Clamp, AddressMode addressW = AddressMode_Clamp, FilterMode magFilter = FilterMode_Linear, FilterMode mipFilter = FilterMode_Linear, uintreg maxAnisotropy = 1, Color borderColor = TIKI_COLOR_BLACK );
		void						disposeSamplerState( const SamplerState*& pSamplerState );

		const VertexFormat*			createVertexFormat( const VertexFormatParameters& creationParameters );
		const VertexFormat*			createVertexFormat( const VertexAttribute* pVertexAttributes, uintreg vertexAttrubuteCount );
		void						disposeVertexFormat( const VertexFormat*& pVertexFormat );

		const VertexInputBinding*	createVertexInputBinding( const VertexInputBindingParameters& parameters );
		const VertexInputBinding*	createVertexInputBinding( const Shader* pShader, const VertexFormat* pVertexFormat );
		void						disposeVertexInputBinding( const VertexInputBinding*& pVertexInputBinding );

		const VertexFormat*			getStockVertexFormat( StockVertexFormat format ) const;

		GraphicsContext&			beginFrame();
		void						endFrame();

		const RenderTarget&			getBackBuffer() const { return m_backBufferTarget; }

	private:

		enum
		{
			MaxBlendStateCount			= 32u,
			MaxDepthStencilStateCount	= 32u,
			MaxRasterizerStateCount		= 32u,
			MaxSamplerStateCount		= 32u,
			MaxVertexFormatCount		= 32u,
			MaxVertexInputBindingCount	= 32u
		};

		uintreg												m_frameNumber;

		GraphicsSystemPlatformData							m_platformData;
		GraphicsContext										m_commandBuffer;

		const VertexFormat*									m_pStockVertexFormats[ StockVertexFormat_Count ];

		TextureData											m_backBufferColorData;
		TextureData											m_backBufferDepthData;
		RenderTarget										m_backBufferTarget;

		GraphicsStateObjectCollection< BlendState >			m_blendStates;
		GraphicsStateObjectCollection< DepthStencilState >	m_depthStencilStates;
		GraphicsStateObjectCollection< RasterizerState >	m_rasterizerStates;
		GraphicsStateObjectCollection< SamplerState >		m_samplerStates;
		GraphicsStateObjectCollection< VertexFormat >		m_vertexFormats;
		GraphicsStateObjectCollection< VertexInputBinding >	m_vertexInputBindings;

		bool												createPlatform( const GraphicsSystemParameters& params );
		void												disposePlatform();
	};
}
