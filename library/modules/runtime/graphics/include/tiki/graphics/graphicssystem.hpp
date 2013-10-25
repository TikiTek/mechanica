#pragma once
#ifndef TIKI_GRAPHICSYSTEM_HPP
#define TIKI_GRAPHICSYSTEM_HPP

#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphicsbase/graphicsstateobject.hpp"
#include "tiki/math/vector2.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "win_d3d11/graphicssystem_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "global_opengl4/graphicssystem_opengl4.hpp"
#else
#	error Platform not implemented
#endif

namespace tiki
{
	struct GraphicsHandles;	
	class RenderTarget;

	enum GraphicsRendererMode
	{
		GraphicsRendererMode_Hardware,
		GraphicsRendererMode_Software,
		GraphicsRendererMode_Wrapper,
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
		friend GraphicsHandles* getHandles( GraphicsSystem& graphicsSystem );
		friend class GameFramework;
		friend struct FrameworkData;

	public:

		const SamplerState*		createSamplerState( const SamplerStateParamters& creationParameters );
		const SamplerState*		createSamplerState( AddressMode addressU, AddressMode addressV, AddressMode addressW, FilterMode magFilter, FilterMode mipFilter, size_t maxAnisotropy = 1, Color borderColor = TIKI_COLOR_BLACK );
		void					disposeSamplerState( const SamplerState* samplerState );

		GraphicsContext*		beginFrame();
		void					endFrame();

		const RenderTarget*		getBackBuffer() const { return m_pBackBufferTarget; }

	protected:

		GraphicsSystem();
		~GraphicsSystem();

		bool					create( const GraphicsSystemParameters& params );
		void					dispose();

	private:
		
		size_t											m_frameNumber;
		
		GraphicsSystemPlatformData						m_platformData;
		GraphicsContext									m_commandBuffer;

		RenderTarget*									m_pBackBufferTarget;

		GraphicsStateObjectCollection< SamplerState >	m_samplerStates;

	};
}

#endif // TIKI_GRAPHICSYSTEM_HPP
