#pragma once
#ifndef TIKI_UIEDITOR_HPP_INCLUDED
#define TIKI_UIEDITOR_HPP_INCLUDED

#include "tiki/toolapplication/toolapplication.hpp"

namespace tiki
{
	class UiElement;
	struct UiEvent;

	class Application : public ToolApplication
	{
		TIKI_NONCOPYABLE_CLASS( Application );

	public:

									Application();
		virtual						~Application();

	protected:

		virtual void				fillToolParameters( ToolApplicationParamters& parameters ) TIKI_OVERRIDE TIKI_FINAL;
		virtual bool				initializeTool() TIKI_OVERRIDE TIKI_FINAL;
		virtual void				shutdownTool() TIKI_OVERRIDE TIKI_FINAL;

		virtual void				updateTool( bool wantToShutdown ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void				renderTool( GraphicsContext& graphicsContext ) const TIKI_OVERRIDE TIKI_FINAL;

		virtual bool				processToolInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void				processToolWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE TIKI_FINAL;

	private:

		UiElement*					m_pTestElement;

		static void					handleMouseOverEvent( UiElement* pSender, const UiEvent& eventData );

	};
}

#endif // TIKI_UIEDITOR_HPP_INCLUDED
