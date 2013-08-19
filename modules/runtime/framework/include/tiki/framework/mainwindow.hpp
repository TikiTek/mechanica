#pragma once
#ifndef TIKI_MAINWINDOW_HPP
#define TIKI_MAINWINDOW_HPP

#include "tiki/base/string.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/framework/windoweventbuffer.hpp"

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

		void*						getHandle() const;
		uint2						getClientSize() const;
		const WindowEventBuffer&	getEventBuffer() const;

	private:

		void*						m_pHandle;
		wstring						m_windowClass;
		wstring						m_windowTitle;

		WindowEventBuffer			m_eventBuffer;

	};
}

#endif // TIKI_MAINWINDOW_HPP
