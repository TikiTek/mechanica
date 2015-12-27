#pragma once
#ifndef TIKI_MAINWINDOW_HPP
#define TIKI_MAINWINDOW_HPP

#include "tiki/base/platform.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/runtimeshared/windoweventbuffer.hpp"

#if TIKI_ENABLED( TIKI_SDL )
#	include "../../../source/sdl/mainwindow_sdl.hpp"
#elif TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include "../../../source/win/mainwindow_win.hpp"
#else
#	error Platform not supported
#endif

#include "base.hpp"

namespace tiki
{
	struct WindowParameters
	{
		WindowParameters()
		{
			instanceHandle	= InvalidInstanceHandle;

			width			= 0u;
			height			= 0u;
			pWindowTitle	= "";

			maxWindowEvents	= 32u;
		}

		InstanceHandle	instanceHandle;

		uint			width;
		uint			height;				
		cstring			pWindowTitle;

		uint			maxWindowEvents;
	};
	
	class MainWindow
	{
		friend class MainWindowPlatform;
		TIKI_NONCOPYABLE_CLASS( MainWindow );

	public:

		MainWindow();
		~MainWindow();

		bool						create( const WindowParameters& params );
		void						dispose();

		void						update();

		WindowHandle				getHandle() const;
		uint2						getClientSize() const;
		WindowEventBuffer&			getEventBuffer();

	private:

		MainWindowPlatformData		m_platformData;

		WindowEventBuffer			m_eventBuffer;

	};
}

#endif // TIKI_MAINWINDOW_HPP
