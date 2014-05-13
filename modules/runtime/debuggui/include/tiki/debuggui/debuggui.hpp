#pragma once
#ifndef __TIKI_DEBUGMENU_HPP_INCLUDED__
#define __TIKI_DEBUGMENU_HPP_INCLUDED__

#include "tiki/base/array.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/debuggui/debugguibutton.hpp"
#include "tiki/debuggui/debugguievent.hpp"
#include "tiki/debuggui/debugguiverticallayout.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/input/mousebutton.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class DebugGuiWindow;
	class Font;
	class GraphicsContext;
	struct InputEvent;

	struct DebugGuiInputState
	{
		Vector2		mousePosition;
		float		mouseWheel;
		bool		mouseButtonState[ MouseButton_Count ];
	};

	class DebugGui
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGui );
		friend class DebugGuiControl;
		friend class DebugGuiWindow;

	public:

		bool		create( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager, uint maxPageCount );
		void		dispose( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager );

		bool		getActive() const			{ return m_isActive; }
		void		setActive( bool isActive )	{ m_isActive = isActive; }

		void		setScreenSize( const Vector2& screenSize );

		void		update();
		void		render( GraphicsContext& graphicsContext );

		bool		processInputEvent( const InputEvent& inputEvent );

	private: // friend

		void		addWindow( DebugGuiWindow& window );
		void		removeWindow( DebugGuiWindow& window );

		void		pushEvent( const DebugGuiEvent& guiEvent );

	private:

		struct WindowMinimizedData
		{
			bool			isVisible;
			DebugGuiButton	button;
		};

		bool							m_isActive;
		Vector2							m_screenSize;

		SizedArray< DebugGuiWindow* >	m_windows;
		Array< WindowMinimizedData >	m_minimizedData;
		DebugGuiVerticalLayout			m_minimizedLayout;

		const Font*						m_pDefaultFont;
		ImmediateRenderer				m_renderer;

		DebugGuiInputState				m_inputState;

	};
}

#endif // __TIKI_DEBUGMENU_HPP_INCLUDED__
