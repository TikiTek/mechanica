#pragma once
#ifndef __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__
#define __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/sizedarray.hpp"

namespace tiki
{
	class Font;
	class ImmediateRenderer;
	struct DebugGuiInputState;
	struct InputEvent;
	struct Rectangle;

	class DebugGuiControl
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiControl );
		friend class DebugGui;

	public:

		virtual void	setRectangle( const Rectangle& boundingRectangle ) = 0;

		void			addChildControl( DebugGuiControl* pChild );
		bool			removeChildControl( DebugGuiControl* pChild );

		virtual void	update();
		virtual void	render( ImmediateRenderer& renderer );

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state );

	protected:

		static const Font*		getDefaultFont() { return s_pDefaultFont; }

	private: // friend

		static void				setDefaultFont( const Font* pDefaultFont ) { s_pDefaultFont = pDefaultFont; }

	private:

		SizedArray< DebugGuiControl* >	m_childControls;

		static const Font*				s_pDefaultFont;

	};
}

#endif // __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__
