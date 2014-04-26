#pragma once
#ifndef __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__
#define __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class Font;
	class ImmediateRenderer;
	struct DebugGuiEvent;
	struct DebugGuiInputState;
	struct InputEvent;
	struct Rectangle;

	enum
	{
		DebugGui_DefaultMargin = 5u
	};

	class DebugGuiControl
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiControl );
		friend class DebugGui;

	public:

		const Rectangle&		getRectangle() const;
		void					setRectangle( const Rectangle& boundingRectangle );
		virtual Vector2			getMinimumSize() = 0;

		virtual void			update() = 0;
		virtual void			render( ImmediateRenderer& renderer ) = 0;

		virtual bool			processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state ) = 0;

	protected:

		virtual void			handleRectangleChanged( const Rectangle& boundingRectangle ) = 0;

		static const Font*		getDefaultFont();
		static void				pushEvent( const DebugGuiEvent& guiEvent );

	private: // friend

		static void				initialize( const Font* pDefaultFont, DebugGui* pDebugGui );
		static void				shutdown();

	private:

		static const Font*		s_pDefaultFont;
		static DebugGui*		s_pDebugGui;

		Rectangle				m_rectangle;

	};
}

#endif // __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__
