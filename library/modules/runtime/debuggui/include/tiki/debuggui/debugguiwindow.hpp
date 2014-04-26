#pragma once
#ifndef __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__
#define __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__

#include "tiki/debuggui/debugguilayout.hpp"

#include "tiki/base/types.hpp"
#include "tiki/math/rectangle.hpp"

namespace tiki
{
	class DebugGui;

	class DebugGuiWindow : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_CLASS( DebugGuiWindow );
		friend class DebugGui;

	public:

		virtual Vector2	getMinimumSize();

		virtual void	update();
		virtual void	render( ImmediateRenderer& renderer );

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state );

		const char*		getTitle() const;

		bool			getVisibility() const;
		void			setVisibility( bool visible );

	protected:

		DebugGuiWindow();
		~DebugGuiWindow();

		void			create( DebugGui& debugGui, const char* pTitle, DebugGuiLayout& layout );
		void			dispose();

		void			setTitle( const char* pTitle );

		virtual void	handleRectangleChanged( const Rectangle& boundingRectangle );

	private: // friend

		virtual bool	processGuiEvent( const DebugGuiEvent& guiEvent );

	private:

		enum
		{
			MaxTitleLength	= 32u,
			TitleHeight		= 25
		};

		enum WindowResizeMask
		{
			WindowResizeMask_None	= 0u,

			WindowResizeMask_Left	= 1u << 0u,
			WindowResizeMask_Right	= 1u << 1u,
			WindowResizeMask_Top	= 1u << 2u,
			WindowResizeMask_Bottom	= 1u << 3u
		};
		
		DebugGui*			m_pDebugGui;
		DebugGuiLayout*		m_pLayout;

		char				m_aTitle[ MaxTitleLength ];

		bool				m_isMoving;
		bool				m_isVisible;
		uint				m_resizeMode;
		
		Rectangle			m_titleRectangle;
		Rectangle			m_clientRectangle;
		
	};
}

#endif // __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__
