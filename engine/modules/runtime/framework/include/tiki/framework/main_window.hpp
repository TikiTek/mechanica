#pragma once

#include "tiki/base/platform.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/runtime_shared/window_event_buffer.hpp"

#if TIKI_ENABLED( TIKI_SDL )
#	include "../../../source/sdl/main_window_sdl.hpp"
#elif TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include "../../../source/win/main_window_win.hpp"
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
		const char*			pWindowTitle;

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
