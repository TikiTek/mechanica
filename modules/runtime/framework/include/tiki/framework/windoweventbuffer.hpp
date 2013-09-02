#pragma once
#ifndef TIKI_WINDOWEVENTBUFFER_HPP
#define TIKI_WINDOWEVENTBUFFER_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/framework/windowevent.hpp"

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

	private:
		
		SizedArray< WindowEvent >	m_events;

	};
}

#endif // TIKI_WINDOWEVENTBUFFER_HPP
