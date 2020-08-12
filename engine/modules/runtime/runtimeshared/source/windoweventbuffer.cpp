
#include "tiki/runtimeshared/windoweventbuffer.hpp"

namespace tiki
{
	bool WindowEventBuffer::create( uint maxEventCount )
	{
		return m_events.create( maxEventCount );
	}

	void WindowEventBuffer::dispose()
	{
		m_events.dispose();
	}

	bool WindowEventBuffer::popEvent( WindowEvent& targetEvent )
	{
		return m_events.pop( targetEvent );
	}

	WindowEvent& WindowEventBuffer::pushEvent( WindowEventType type )
	{
		if ( m_events.getCount() > 0u && ( m_events.getBottom().type == type || m_events.getCount() == m_events.getCapacity() ) )
		{
			WindowEvent& event	= m_events.getBottom();
			event.type			= type;

			return event;
		}

		WindowEvent& event	= m_events.push();
		event.type			= type;

		return event;
	}

	size_t WindowEventBuffer::getEventCount() const
	{
		return m_events.getCount();
	}
}