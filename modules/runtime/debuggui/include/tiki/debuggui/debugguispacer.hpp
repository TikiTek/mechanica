#pragma once
#ifndef TIKI_DEBUGGUISPACER_HPP_INCLUDED__
#define TIKI_DEBUGGUISPACER_HPP_INCLUDED__

#include "tiki/debuggui/debugguicontrol.hpp"

namespace tiki
{
	class DebugGuiSpacer : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiSpacer );

	public:

		void			create( const Vector2& minSize );
		void			dispose();

		virtual Vector2	getMinimumSize();

		virtual void	update();
		virtual void	render( ImmediateRenderer& renderer );

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state );

	protected:

		virtual void	handleRectangleChanged( const Rectangle& boundingRectangle );

	private:

		Vector2			m_minSize;

	};
}

#endif // TIKI_DEBUGGUISPACER_HPP_INCLUDED__
