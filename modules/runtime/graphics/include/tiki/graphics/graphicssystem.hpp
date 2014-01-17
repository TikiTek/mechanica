#pragma once
#ifndef TIKI_GRAPHICSYSTEM_HPP
#define TIKI_GRAPHICSYSTEM_HPP

#include "tiki/base/poolallocator.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphics/vertexinputbinding.hpp"
#include "tiki/graphicsbase/graphicsstateobject.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "win_d3d11/graphicssystem_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "global_opengl4/graphicssystem_opengl4.hpp"
#else
#	error Platform not implemented
#endif

namespace tiki
{
	enum GraphicsRendererMode
	{
		GraphicsRendererMode_Hardware,
		GraphicsRendererMode_Software,
		GraphicsRendererMode_Wrapper,
	};

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
		TIKI_NONCOPYABLE_CLASS( GraphicsSystem );

	public:

		GraphicsSystem();
		~GraphicsSystem();

		bool						create( const GraphicsSystemParameters& params );
		void						dispose();

		bool						resize( uint width, uint height );

		const SamplerState*			createSamplerState( const SamplerStateParamters& creationParameters );
		const SamplerState*			createSamplerState( AddressMode addressU, AddressMode addressV, AddressMode addressW, FilterMode magFilter, FilterMode mipFilter, size_t maxAnisotropy = 1, Color borderColor = TIKI_COLOR_BLACK );
		void						disposeSamplerState( const SamplerState* samplerState );

		const VertexFormat*			createVertexFormat( const VertexFormatParameters& creationParameters );
		const VertexFormat*			createVertexFormat( const VertexAttribute* pVertexAttributes, uint vertexAttrubuteCount );
		void						disposeVertexFormat( const VertexFormat* pVertexFormat );

		const VertexInputBinding*	createVertexInputBinding( const VertexInputBindingParameters& parameters );
		const VertexInputBinding*	createVertexInputBinding( const Shader* pShader, const VertexFormat* pVertexFormat );
		void						disposeVertexInputBinding( const VertexInputBinding* pVertexInputBinding );

		const VertexFormat*			getStockVertexFormat( StockVertexFormat format ) const;

		GraphicsContext&			beginFrame();
		void						endFrame();

		const RenderTarget&			getBackBuffer() const { return m_backBufferTarget; }

	private:
		
		uint												m_frameNumber;
		
		GraphicsSystemPlatformData							m_platformData;
		GraphicsContext										m_commandBuffer;

		const VertexFormat*									m_pStockVertexFormats[ StockVertexFormat_Count ];

		RenderTarget										m_backBufferTarget;

		GraphicsStateObjectCollection< SamplerState >		m_samplerStates;
		GraphicsStateObjectCollection< VertexFormat >		m_vertexFormats;
		GraphicsStateObjectCollection< VertexInputBinding >	m_vertexInputBindings;

		bool												createPlatform( const GraphicsSystemParameters& params );
		void												disposePlatform();

	};
}

#endif // TIKI_GRAPHICSYSTEM_HPP
