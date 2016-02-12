#pragma once
#ifndef TIKI_GRAPHICSYSTEM_HPP
#define TIKI_GRAPHICSYSTEM_HPP

#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/graphics/blendstate.hpp"
#include "tiki/graphics/depthstencilstate.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicsrenderermode.hpp"
#include "tiki/graphics/graphicsstateobject.hpp"
#include "tiki/graphics/graphicsstateobjectcollection.hpp"
#include "tiki/graphics/graphicstypes.hpp"
#include "tiki/graphics/rasterizerstate.hpp"
#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphics/vertexinputbinding.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/graphicssystem_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/graphicssystem_d3d12.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "../../../source/global_opengl4/graphicssystem_opengl4.hpp"
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
			backBufferWidth		= 900;
			backBufferHeight	= 600;
			fullScreen			= false;
			rendererMode		= GraphicsRendererMode_Hardware;

			pWindowHandle		= nullptr;
		}

		uint					backBufferWidth;
		uint					backBufferHeight;
		bool					fullScreen;
		GraphicsRendererMode	rendererMode;

		void*					pWindowHandle;
	};

	class GraphicsSystem
	{
		friend class GraphicsSystemPlatform;
		TIKI_NONCOPYABLE_CLASS( GraphicsSystem );

	public:

		GraphicsSystem();
		~GraphicsSystem();

		bool						create( const GraphicsSystemParameters& params );
		void						dispose();

		bool						resize( uint width, uint height );

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
		const SamplerState*			createSamplerState( AddressMode addressU = AddressMode_Clamp, AddressMode addressV = AddressMode_Clamp, AddressMode addressW = AddressMode_Clamp, FilterMode magFilter = FilterMode_Linear, FilterMode mipFilter = FilterMode_Linear, size_t maxAnisotropy = 1, Color borderColor = TIKI_COLOR_BLACK );
		void						disposeSamplerState( const SamplerState*& pSamplerState );

		const VertexFormat*			createVertexFormat( const VertexFormatParameters& creationParameters );
		const VertexFormat*			createVertexFormat( const VertexAttribute* pVertexAttributes, uint vertexAttrubuteCount );
		void						disposeVertexFormat( const VertexFormat*& pVertexFormat );

		const VertexInputBinding*	createVertexInputBinding( const VertexInputBindingParameters& parameters );
		const VertexInputBinding*	createVertexInputBinding( const Shader* pShader, const VertexFormat* pVertexFormat );
		void						disposeVertexInputBinding( const VertexInputBinding*& pVertexInputBinding );

		DynamicBuffer				allocateDynamicBuffer( GraphicsBufferType type, uint size );

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

		struct DynamicGpuBuffer : public LinkedItem< DynamicGpuBuffer >
		{
			GraphicsBufferType	type;

			BaseBuffer			gpuBuffer;
			uint				size;
			uint				currentGpuOffset;
		};
		typedef FixedArray< DynamicGpuBuffer*, GraphicsBufferType_Count > FixedDynamicGpuBufferArray;
		
		uint												m_frameNumber;
		
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

		FixedDynamicGpuBufferArray							m_lastDynamicBufferByType;
		LinkedList< DynamicGpuBuffer >						m_dynamicBuffers;
		
		bool												createPlatform( const GraphicsSystemParameters& params );
		void												disposePlatform();

	};
}

#endif // TIKI_GRAPHICSYSTEM_HPP
