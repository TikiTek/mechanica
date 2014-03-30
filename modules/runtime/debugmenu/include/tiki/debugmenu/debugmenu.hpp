#pragma once
#ifndef __TIKI_DEBUGMENU_HPP_INCLUDED__
#define __TIKI_DEBUGMENU_HPP_INCLUDED__

#include "tiki/base/sizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class DebugMenuPage;
	class Font;
	class GraphicsContext;
	struct InputEvent;

	class DebugMenu
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugMenu );

	public:

		bool		create( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager, uint maxPageCount );
		void		dispose( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager );

		void		addPage( DebugMenuPage& page );
		void		removePage( DebugMenuPage& page );

		bool		getActive() const			{ return m_isActive; }
		void		setActive( bool isActive )	{ m_isActive = isActive; }

		void		setScreenSize( const Vector2& screenSize );

		void		update();
		void		render( GraphicsContext& graphicsContext );

		bool		processInputEvent( const InputEvent& inputEvent );

	private:

		bool							m_isActive;
		uint							m_activePageIndex;
		Vector2							m_screenSize;

		const Font*						m_pFont;

		SizedArray< DebugMenuPage* >	m_pages;

		ImmediateRenderer				m_renderer;

	};
}

#endif // __TIKI_DEBUGMENU_HPP_INCLUDED__
