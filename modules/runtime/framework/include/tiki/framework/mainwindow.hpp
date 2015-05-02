#pragma once
#ifndef TIKI_MAINWINDOW_HPP
#define TIKI_MAINWINDOW_HPP

#include "tiki/base/platform.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/runtimeshared/windoweventbuffer.hpp"

namespace tiki
{
	struct WindowParameters
	{
		uint		width;
		uint		height;

		cstring		pClassName;
		cstring		pWindowTitle;
	};

	class MainWindow
	{
	public:

		MainWindow();

		bool						create( const WindowParameters& params );
		void						dispose();

		void						update();

		WindowHandle				getHandle() const;
		uint2						getClientSize() const;
		const WindowEventBuffer&	getEventBuffer() const;

	private:

		WindowHandle				m_pHandle;

		WindowEventBuffer			m_eventBuffer;

	};
}

#endif // TIKI_MAINWINDOW_HPP
