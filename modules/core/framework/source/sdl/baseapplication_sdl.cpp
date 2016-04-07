
#include "tiki/framework/baseapplication.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/input/inputsystem.hpp"

#include "SDL.h"

namespace tiki
{
	bool BaseApplication::initializePlatform()
	{
		if ( SDL_Init( SDL_INIT_GAMECONTROLLER ) < 0 )
		{
			TIKI_TRACE_ERROR( "Failed to initialize SDL! Error: %s\n", SDL_GetError() );
			return false;
		}

		return true;
	}

	void BaseApplication::shutdownPlatform()
	{
		SDL_Quit();
	}

	void BaseApplication::updatePlatform()
	{
		SDL_Event sdlEvent;
		while ( SDL_PollEvent( &sdlEvent ) )
		{
			MainWindowPlatform::processEvent( getMainWindow(), sdlEvent );
			InputSystemPlatform::processEvent( getInputSystem(), sdlEvent );
		}
	}
}
