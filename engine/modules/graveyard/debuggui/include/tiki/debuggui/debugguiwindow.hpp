#pragma once
#ifndef __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__
#define __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__

#include "tiki/debuggui/debugguilayout.hpp"

#include "tiki/container/linkedlist.hpp"
#include "tiki/base/types.hpp"
#include "tiki/debuggui/debugguibutton.hpp"
#include "tiki/math/rectangle.hpp"

namespace tiki
{
	class DebugGui;

	class DebugGuiWindow : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_CLASS( DebugGuiWindow );
		friend class DebugGui;

	public:

		virtual bool	isInHierarchy() const TIKI_OVERRIDE_FINAL;

		virtual Vector2	getMinimumSize() TIKI_OVERRIDE;

		virtual void	update( double elapsedTime ) TIKI_OVERRIDE;
		virtual void	render( ImmediateRenderer& renderer ) TIKI_OVERRIDE;

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state ) TIKI_OVERRIDE;

		const char*		getTitle() const;

		bool			getVisibility() const;
		void			setVisibility( bool visible );

	protected:

		DebugGuiWindow();
		~DebugGuiWindow();

		void			create( DebugGui& debugGui, const char* pTitle, DebugGuiLayout& layout );
		void			dispose();

		void			setTitle( const char* pTitle );

		virtual void	handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle ) TIKI_OVERRIDE;

		virtual bool	processGuiEvent( const DebugGuiEvent& guiEvent );

	private: // friend

		DebugGuiButton&	getMinimizedButton();

	private:

		enum
		{
			TitleHeight = 25
		};

		enum WindowResizeMask
		{
			WindowResizeMask_None	= 0u,

			WindowResizeMask_Left	= 1u << 0u,
			WindowResizeMask_Right	= 1u << 1u,
			WindowResizeMask_Top	= 1u << 2u,
			WindowResizeMask_Bottom	= 1u << 3u
		};
		
		DebugGui*				m_pDebugGui;
		DebugGuiLayout*			m_pLayout;

		DebugGuiButton			m_minimizeButton;
		DebugGuiButton			m_minimizedButton;

		char					m_aTitle[ DebugGui_MaxTextLength ];

		bool					m_isMoving;
		bool					m_isVisible;
		uint32					m_resizeMode;
		
		AxisAlignedRectangle	m_titleRectangle;
		AxisAlignedRectangle	m_clientRectangle;
		
	};
}

#endif // __TIKI_DEBUGGUIWINDOW_HPP_INCLUDED__
