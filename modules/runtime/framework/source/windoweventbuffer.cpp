
#include "tiki/framework/windoweventbuffer.hpp"

namespace tiki
{
	void WindowEventBuffer::create()
	{
	}

	void WindowEventBuffer::dispose()
	{
	}

	void WindowEventBuffer::clear()
	{
		m_events.clear();
	}

	WindowEvent& WindowEventBuffer::pushEvent( WindowEventType type )
	{
		if ( m_events.getCount() > 0u && ( m_events.getLast().type == type || m_events.getCount() == m_events.getCapacity() ) )
		{
			WindowEvent& event	= m_events.getLast();
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

	const WindowEvent* WindowEventBuffer::getEventByType( WindowEventType type ) const
	{
		for (uint i = 0u; i < m_events.getCount(); ++i)
		{
			const WindowEvent& event = m_events[ i ];

			if ( event.type == type )
			{
				return &event;
			}
		} 

		return nullptr;
	}

}