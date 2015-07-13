
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

	MainWindow::~MainWindow()
	{
	}
	
	bool MainWindow::create( const WindowParameters& params )
	{
		TIKI_ASSERT( !s_mainWindowCreated );
		s_mainWindowCreated = true;

		m_platformData.pWindow = SDL_CreateWindow(
			params.pWindowTitle,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			int( params.width ),
			int( params.height ),
			SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_RESIZABLE
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

		//SDL_UpdateWindowSurface( m_platformData.pWindow );
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

	void MainWindowPlatform::processEvent( MainWindow& mainWindow, const SDL_Event& sdlEvent )
	{
		switch ( sdlEvent.type )
		{
		case SDL_WINDOWEVENT:
			processWindowEvent( mainWindow.m_eventBuffer, sdlEvent.window );
			break;

		case SDL_QUIT:
			processQuitEvent( mainWindow.m_eventBuffer, sdlEvent.quit );
			break;

		default:
			break;
		}
	}

	void MainWindowPlatform::processWindowEvent( WindowEventBuffer& eventBuffer, const SDL_WindowEvent& windowEvent )
	{
		switch ( windowEvent.type )
		{
		case SDL_WINDOWEVENT_RESIZED:
			{
				WindowEvent& event				= eventBuffer.pushEvent( WindowEventType_SizeChanged );
				event.data.sizeChanged.size.x	= windowEvent.data1;
				event.data.sizeChanged.size.y	= windowEvent.data2;
			}
			break;

		default:
			break;
		}
	}

	void MainWindowPlatform::processQuitEvent( WindowEventBuffer& eventBuffer, const SDL_QuitEvent& quitEvent )
	{
		eventBuffer.pushEvent( WindowEventType_Destroy );
	}
}
