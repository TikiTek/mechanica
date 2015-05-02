#pragma once
#ifndef TIKI_WINDOWEVENTBUFFER_HPP
#define TIKI_WINDOWEVENTBUFFER_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/runtimeshared/windowevent.hpp"

namespace tiki
{
	class WindowEventBuffer
	{
	public:

		void						create();
		void						dispose();

		void						clear();
		WindowEvent&				pushEvent( WindowEventType type );

		size_t						getEventCount() const;
		const WindowEvent&			getEventByIndex( size_t index ) const;
		const WindowEvent*			getEventByType( WindowEventType type ) const;

	private:
		
		typedef FixedSizedArray< WindowEvent, TIKI_WINDOWEVENTBUFFER_SIZE > WindowEventArray;

		WindowEventArray			m_events;

	};
}

#endif // TIKI_WINDOWEVENTBUFFER_HPP
