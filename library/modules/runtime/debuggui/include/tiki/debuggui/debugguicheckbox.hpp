#pragma once
#ifndef __TIKI_DEBUGGUICHECKBOX_HPP_INCLUDED__
#define __TIKI_DEBUGGUICHECKBOX_HPP_INCLUDED__

#include "tiki/debuggui/debugguicontrol.hpp"

namespace tiki
{
	class DebugGuiCheckBox : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiCheckBox );

	public:

		void			create( const char* pText, bool value );
		void			dispose();

		bool			getValue() const;

		virtual Vector2 getMinimumSize();

		virtual void	update();
		virtual void	render( ImmediateRenderer& renderer );

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state );

	protected:

		virtual void	handleRectangleChanged( const Rectangle& boundingRectangle );

	private:

		enum
		{
			BoxSize		= DebugGui_DefaultMargin * 3,
			CheckerSize	= 9
		};

		bool			m_value;
		bool			m_mouseOver;
		bool			m_mouseDown;

		char			m_aText[ DebugGui_MaxTextLength ];

		Vector2			m_textPosition;
		Rectangle		m_box;
		Rectangle		m_checker;

	};
}

#endif // __TIKI_DEBUGGUICHECKBOX_HPP_INCLUDED__
