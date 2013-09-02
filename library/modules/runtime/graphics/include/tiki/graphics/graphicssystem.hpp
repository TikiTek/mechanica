#pragma once
#ifndef TIKI_GRAPHICSYSTEM_HPP
#define TIKI_GRAPHICSYSTEM_HPP

#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphicsbase/graphicsstateobject.hpp"
#include "tiki/math/vector2.hpp"

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
		
		size_t					m_frameCount;

		Vector2					m_backBufferSizeFloat;
		uint2					m_backBufferSize;

		GraphicsHandles*		m_pHandles;
		GraphicsContext			m_commandBuffer;

		RenderTarget*			m_pBackBufferTarget;

		GraphicsStateObjectCollection< SamplerState >	m_samplerStates;

	};
}

#endif // TIKI_GRAPHICSYSTEM_HPP
