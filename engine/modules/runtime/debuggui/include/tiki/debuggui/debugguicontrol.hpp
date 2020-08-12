#pragma once
#ifndef __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__
#define __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__

#include "tiki/container/linkedlist.hpp"
#include "tiki/base/types.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/math/thickness.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class DebugGuiLayout;
	class DebugGuiWindow;
	class Font;
	class ImmediateRenderer;
	struct DebugGuiEvent;
	struct DebugGuiInputState;
	struct InputEvent;
	struct Rectangle;
    class DebugGui;

	enum
	{
		DebugGui_DefaultPadding = 5u,
		DebugGui_MaxTextLength = 32u
	};

	class DebugGuiControl : public LinkedItem< DebugGuiControl >
	{
		TIKI_NONCOPYABLE_CLASS( DebugGuiControl );
		friend class DebugGui;
		friend class DebugGuiLayout;
		friend class DebugGuiWindow;

	public:

		DebugGuiControl();

		const DebugGuiControl*	getParent() const;
		virtual bool			isInHierarchy() const;

		const Thickness&		getPadding() const;
		void					setPadding( const Thickness& padding );

		const Rectangle&		getRectangle() const;
		void					setRectangle( const Rectangle& boundingRectangle );
		
		virtual Vector2			getMinimumSize() TIKI_PURE;

		virtual void			update( double elapsedTime ) TIKI_PURE;
		virtual void			render( ImmediateRenderer& renderer ) TIKI_PURE;

		virtual bool			processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state ) TIKI_PURE;

	protected:

		void					refreshRectangle();
		virtual void			handleRectangleChanged( const Rectangle& boundingRectangle ) TIKI_PURE;

		static const Font*		getDefaultFont();
		static void				pushEvent( const DebugGuiEvent& guiEvent );

	private: // friend

		void					setParent( const DebugGuiControl* pParent );

		static void				initialize( const Font* pDefaultFont, DebugGui* pDebugGui );
		static void				shutdown();
		
	private:

		static const Font*		s_pDefaultFont;
		static DebugGui*		s_pDebugGui;

		const DebugGuiControl*	m_pParent;

		Thickness				m_padding;
		Rectangle				m_rectangle;

	};
}

#endif // __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__
