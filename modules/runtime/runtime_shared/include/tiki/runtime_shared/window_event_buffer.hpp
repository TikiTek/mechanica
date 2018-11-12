#pragma once

#include "tiki/base/types.hpp"
#include "tiki/container/queue.hpp"
#include "tiki/runtime_shared/window_event.hpp"

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
