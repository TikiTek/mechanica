#pragma once
#ifndef TIKI_MAINWINDOW_SDL_HPP_INCLUDED__
#define TIKI_MAINWINDOW_SDL_HPP_INCLUDED__

union SDL_Event;
struct SDL_QuitEvent;
struct SDL_Window;
struct SDL_WindowEvent;

namespace tiki
{
	class MainWindow;

	struct MainWindowPlatformData
	{
		MainWindowPlatformData()
		{
			pWindow = nullptr;
		}

		SDL_Window*		pWindow;
	};

	class MainWindowPlatform
	{
	public:

		static void		processEvent( MainWindow& mainWindow, const SDL_Event& sdlEvent );

	private:

		static void		processWindowEvent( WindowEventBuffer& eventBuffer, const SDL_WindowEvent& windowEvent );
		static void		processQuitEvent( WindowEventBuffer& eventBuffer, const SDL_QuitEvent& quitEvent );

	};
}

#endif // TIKI_MAINWINDOW_SDL_HPP_INCLUDED__
