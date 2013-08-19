
#include "tiki/framework/windoweventbuffer.hpp"

namespace tiki
{
	void WindowEventBuffer::create()
	{
		m_events.create( TIKI_WINDOWEVENTBUFFER_SIZE );
	}

	void WindowEventBuffer::dispose()
	{
		m_events.dispose();
	}

	void WindowEventBuffer::clear()
	{
		m_events.clear();
	}

	WindowEvent& WindowEventBuffer::pushEvent( WindowEventTypes type )
	{
		if ( m_events.getCount() > 0u && ( m_events.getTop().type == type || m_events.getCount() == m_events.getCapacity() ) )
		{
			WindowEvent& event	= m_events.push();
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

	const WindowEvent& WindowEventBuffer::getEventByIndex( size_t index ) const
	{
		return m_events[ index ];
	}
}