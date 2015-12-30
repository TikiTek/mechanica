#pragma once
#ifndef TIKI_WINDOWEVENTBUFFER_HPP
#define TIKI_WINDOWEVENTBUFFER_HPP

#include "tiki/base/types.hpp"
#include "tiki/container/queue.hpp"
#include "tiki/runtimeshared/windowevent.hpp"

namespace tiki
{
	class WindowEventBuffer
	{
	public:

		bool						create( uint maxEventCount );
		void						dispose();

		bool						popEvent( WindowEvent& targetEvent );
		WindowEvent&				pushEvent( WindowEventType type );

		uint						getEventCount() const;

	private:
		
		typedef Queue< WindowEvent > WindowEventArray;

		WindowEventArray			m_events;

	};
}

#endif // TIKI_WINDOWEVENTBUFFER_HPP
