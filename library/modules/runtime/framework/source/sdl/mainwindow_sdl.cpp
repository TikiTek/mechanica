
#include "tiki/framework/mainwindow.hpp"

#include "tiki/base/memory.hpp"

#include "SDL.h"
#include "SDL_syswm.h"

namespace tiki
{
	bool MainWindow::s_mainWindowCreated = false;
	
	MainWindow::MainWindow()
	{
	}
	
	bool MainWindow::create( const WindowParameters& params )
	{
		TIKI_ASSERT( !s_mainWindowCreated );
		s_mainWindowCreated = true;

		uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_RESIZABLE;
#if TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
		flags |= SDL_WINDOW_OPENGL;
#endif

		m_platformData.pWindow = SDL_CreateWindow(
			params.pWindowTitle,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			int( params.width ),
			int( params.height ),
			flags
		);

		return true;
	}

	void MainWindow::dispose()
	{
		SDL_DestroyWindow( m_platformData.pWindow );
		m_platformData.pWindow = nullptr;

		m_eventBuffer.dispose();

		s_mainWindowCreated	= false;
	}

	void MainWindow::update()
	{
		m_eventBuffer.clear();
	}

	WindowHandle MainWindow::getHandle() const
	{
		SDL_SysWMinfo info;
		SDL_VERSION( &info.version );

		if ( SDL_GetWindowWMInfo( m_platformData.pWindow, &info ) )
		{
			return (WindowHandle)info.info.win.window;
		}

		return InvalidWindowHandle;
	}

	uint2 MainWindow::getClientSize() const
	{
		int width;
		int height;
		SDL_GetWindowSize( m_platformData.pWindow, &width, &height );
				
		const uint2 size = { uint32( width ), uint32( height ) };
		return size;
	}

	const WindowEventBuffer& MainWindow::getEventBuffer() const
	{
		return m_eventBuffer;
	}

}
