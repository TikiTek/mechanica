#pragma once
#ifndef TIKI_GRAPHICSYSTEM_HPP
#define TIKI_GRAPHICSYSTEM_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/math/vector2.hpp"

namespace tiki
{
	struct GraphicsHandles;

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

		bool					createContext( void );
		void					disposeContext( void );

		GraphicsContext*		beginFrame();
		void					endFrame();

		const Vector2&			getBackBufferSizeFloat() const	{ return m_backBufferSizeFloat; }
		const uint2&			getBackBufferSize() const		{ return m_backBufferSize; }

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

	};
}

#endif // TIKI_GRAPHICSYSTEM_HPP
