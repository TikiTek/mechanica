#pragma once
#ifndef __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__
#define __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__

#include "tiki/debuggui/debugguicontrol.hpp"

#include "tiki/base/types.hpp"

namespace tiki
{
	class DebugGuiWindow : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiWindow );

	public:

		const char*		getTitle() const;
		void			setTitle( const char* pTitle );
		
		bool			getVisibility() const;
		void			setVisibility( bool visible );
		
	};
}

#endif // __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__
