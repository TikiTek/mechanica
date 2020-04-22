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
		InstanceHandle	instanceHandle	= InvalidInstanceHandle;

		sint16			postionX		= 0;
		sint16			postionY		= 0;
		uint16			width			= 0u;
		uint16			height			= 0u;
		const char*		pWindowTitle	= "";

		uintreg			maxWindowEvents	= 32u;
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
		sint2						getPosition() const;
		uint2						getSize() const;
		uint2						getClientSize() const;
		WindowEventBuffer&			getEventBuffer();

	private:

		MainWindowPlatformData		m_platformData;

		WindowEventBuffer			m_eventBuffer;
	};
}
