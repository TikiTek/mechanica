#pragma once
#ifndef TIKI_TOOLAPPLICATION_HPP_INCLUDED
#define TIKI_TOOLAPPLICATION_HPP_INCLUDED

#include "tiki/framework/baseapplication.hpp"

namespace tiki
{
	class ToolApplication;

	namespace framework
	{
		ToolApplication& getTool();
	}

	struct ToolApplicationParamters : public BaseApplicationParamters
	{
		ToolApplicationParamters()
		{
		}
	};

	class ToolApplication : public BaseApplication
	{
		TIKI_NONCOPYABLE_CLASS( ToolApplication );

	public:

	protected:

									ToolApplication();
		virtual						~ToolApplication();

	protected:

		virtual void				fillBaseParameters( BaseApplicationParamters& parameters ) TIKI_OVERRIDE TIKI_FINAL;

		virtual bool				initializeApplication() TIKI_OVERRIDE TIKI_FINAL;
		virtual void				shutdownApplication() TIKI_OVERRIDE TIKI_FINAL;

		virtual void				updateApplication( bool wantToShutdown ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void				renderApplication( GraphicsContext& graphicsContext ) const TIKI_OVERRIDE TIKI_FINAL;

		virtual bool				processBaseInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void				processBaseWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE TIKI_FINAL;

		virtual void				fillToolParameters( ToolApplicationParamters& parameters ) TIKI_PURE;
		virtual bool				initializeTool() TIKI_PURE;
		virtual void				shutdownTool() TIKI_PURE;

		virtual void				updateTool( bool wantToShutdown ) TIKI_PURE;
		virtual void				renderTool( GraphicsContext& graphicsContext ) const TIKI_PURE;

		virtual bool				processToolInputEvent( const InputEvent& inputEvent ) TIKI_PURE;
		virtual void				processToolWindowEvent( const WindowEvent& windowEvent ) TIKI_PURE;

	private:

		ToolApplicationParamters	m_parameters;

	};
}

#endif // TIKI_TOOLAPPLICATION_HPP_INCLUDED
