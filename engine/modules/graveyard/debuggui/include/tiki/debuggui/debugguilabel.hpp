#pragma once
#ifndef __TIKI_DEBUGGUILABEL_HPP_INCLUDED__
#define __TIKI_DEBUGGUILABEL_HPP_INCLUDED__

#include "tiki/debuggui/debugguicontrol.hpp"

namespace tiki
{
	class DebugGuiLabel : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiLabel );

	public:

		void				create( const char* pText );
		void				dispose();

		const char*			getText() const;
		void				setText( const char* pText );

		virtual Vector2		getMinimumSize() TIKI_OVERRIDE_FINAL;

		virtual void		update( double elapsedTime ) TIKI_OVERRIDE_FINAL;
		virtual void		render( ImmediateRenderer& renderer ) TIKI_OVERRIDE_FINAL;

		virtual bool		processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state ) TIKI_OVERRIDE_FINAL;

	protected:

		virtual void		handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle ) TIKI_OVERRIDE_FINAL;

	private:

		char	m_aText[ DebugGui_MaxTextLength ];

	};
}

#endif // __TIKI_DEBUGGUILABEL_HPP_INCLUDED__
