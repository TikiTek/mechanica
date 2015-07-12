#pragma once
#ifndef TIKI_MAINWINDOW_SDL_HPP_INCLUDED__
#define TIKI_MAINWINDOW_SDL_HPP_INCLUDED__

struct SDL_Window;

namespace tiki
{
	struct MainWindowPlatformData
	{
		MainWindowPlatformData()
		{
			pWindow = nullptr;
		}

		SDL_Window*		pWindow;
	};
}

#endif // TIKI_MAINWINDOW_SDL_HPP_INCLUDED__
