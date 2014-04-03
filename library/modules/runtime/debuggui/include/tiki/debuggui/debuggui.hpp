#pragma once
#ifndef __TIKI_DEBUGMENU_HPP_INCLUDED__
#define __TIKI_DEBUGMENU_HPP_INCLUDED__

#include "tiki/base/sizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class DebugGuiWindow;
	class Font;
	class GraphicsContext;
	struct InputEvent;

	class DebugGui
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGui );

	public:

		bool		create( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager, uint maxPageCount );
		void		dispose( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager );

		void		addWindow( DebugGuiWindow& window );
		void		removeWindow( DebugGuiWindow& window );

		bool		getActive() const			{ return m_isActive; }
		void		setActive( bool isActive )	{ m_isActive = isActive; }

		void		setScreenSize( const Vector2& screenSize );

		void		update();
		void		render( GraphicsContext& graphicsContext );

		bool		processInputEvent( const InputEvent& inputEvent );

	private:

		bool							m_isActive;
		Vector2							m_screenSize;

		SizedArray< DebugGuiWindow* >	m_windows;

		const Font*						m_pDefaultFont;
		ImmediateRenderer				m_renderer;

	};
}

#endif // __TIKI_DEBUGMENU_HPP_INCLUDED__
