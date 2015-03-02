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

		virtual Vector2	getMinimumSize();

		virtual void	update();
		virtual void	render( ImmediateRenderer& renderer );

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state );

	protected:

		virtual void	handleRectangleChanged( const Rectangle& boundingRectangle );

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
