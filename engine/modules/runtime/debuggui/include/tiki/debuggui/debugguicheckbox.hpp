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
		void			setValue( bool value );

		virtual Vector2 getMinimumSize() TIKI_OVERRIDE TIKI_FINAL;

		virtual void	update( double elapsedTime ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void	render( ImmediateRenderer& renderer ) TIKI_OVERRIDE TIKI_FINAL;

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state ) TIKI_OVERRIDE TIKI_FINAL;

	protected:

		virtual void	handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle ) TIKI_OVERRIDE TIKI_FINAL;

	private:

		enum
		{
			CheckerSize	= 9
		};

		bool					m_value;
		bool					m_mouseOver;
		bool					m_mouseDown;

		char					m_aText[ DebugGui_MaxTextLength ];

		Vector2					m_textPosition;
		AxisAlignedRectangle	m_box;
		AxisAlignedRectangle	m_checker;

	};
}

#endif // __TIKI_DEBUGGUICHECKBOX_HPP_INCLUDED__
