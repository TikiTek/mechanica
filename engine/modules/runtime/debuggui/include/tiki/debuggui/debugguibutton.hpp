#pragma once
#ifndef __TIKI_DEBUGGUIBUTTON_HPP_INCLUDED__
#define __TIKI_DEBUGGUIBUTTON_HPP_INCLUDED__

#include "tiki/debuggui/debugguicontrol.hpp"

namespace tiki
{
	class DebugGuiButton : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiButton );

	public:

		void			create( const char* pText );
		void			dispose();

		const char*		getText() const;
		void			setText( const char* pText );

		virtual Vector2	getMinimumSize() TIKI_OVERRIDE_FINAL;

		virtual void	update( double elapsedTime ) TIKI_OVERRIDE_FINAL;
		virtual void	render( ImmediateRenderer& renderer ) TIKI_OVERRIDE_FINAL;

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state ) TIKI_OVERRIDE_FINAL;

	protected:

		virtual void	handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle ) TIKI_OVERRIDE_FINAL;

	private:

		enum
		{
			MaxTextLength = 32
		};

		bool			m_mouseOver;
		bool			m_mouseDown;
		char			m_aText[ MaxTextLength ];

	};
}

#endif // __TIKI_DEBUGGUIBUTTON_HPP_INCLUDED__
