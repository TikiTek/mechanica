#pragma once
#ifndef __TIKI_DEBUGGUIEVENT_HPP_INCLUDED__
#define __TIKI_DEBUGGUIEVENT_HPP_INCLUDED__

namespace tiki
{
	class DebugGuiControl;

	enum DebugGuiEventType
	{
		DebugGuiEventType_Invalid	= -1,

		DebugGuiEventType_Click,
		DebugGuiEventType_ValueChanged,

		DebugGuiEventType_Count
	};

	struct DebugGuiEvent
	{
		DebugGuiEvent()
		{
			pControl	= nullptr;
			eventType	= DebugGuiEventType_Invalid;
		}

		DebugGuiEvent( const DebugGuiControl* _pControl, DebugGuiEventType _eventType )
		{
			pControl	= _pControl;
			eventType	= _eventType;
		}

		const DebugGuiControl*	pControl;
		DebugGuiEventType		eventType;
	};	
}

#endif // __TIKI_DEBUGGUIEVENT_HPP_INCLUDED__
