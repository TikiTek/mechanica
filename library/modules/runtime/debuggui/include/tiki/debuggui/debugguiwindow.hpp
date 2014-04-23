#pragma once
#ifndef __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__
#define __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__

#include "tiki/debuggui/debugguicontrol.hpp"

#include "tiki/base/types.hpp"
#include "tiki/math/rectangle.hpp"

namespace tiki
{
	class DebugGui;

	class DebugGuiWindow : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_CLASS( DebugGuiWindow );

	public:

		DebugGuiWindow();
		~DebugGuiWindow();

		const char*		getTitle() const;
		
		bool			getVisibility() const;
		void			setVisibility( bool visible );

		virtual void	setRectangle( const Rectangle& boundingRectangle );

		virtual void	update();
		virtual void	render( ImmediateRenderer& renderer );

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state );

	protected:

		void			create( DebugGui& debugGui, const char* pTitle );
		void			dispose();

		void			setTitle( const char* pTitle );

	private:

		enum
		{
			MaxTitleLength	= 32u,
			WindowMargin	= 5,
			TitleHeight		= 25
		};
		
		DebugGui*		m_pDebugGui;

		char			m_aTitle[ MaxTitleLength ];

		bool			m_isMoving;
		bool			m_isVisible;
		Rectangle		m_fullRectangle;
		Rectangle		m_titleRectangle;
		Rectangle		m_clientRectangle;

	};
}

#endif // __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__
