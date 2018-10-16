#pragma once

#include "tiki/toolapplication/tool_application.hpp"

#include "editor.hpp"

namespace tiki
{
	class EditorApplication : public ToolApplication
	{
		TIKI_NONCOPYABLE_CLASS( EditorApplication );

	public:

						EditorApplication();
		virtual			~EditorApplication();

		virtual void	fillToolParameters( ToolApplicationParamters& parameters ) TIKI_OVERRIDE_FINAL;
		virtual bool	initializeTool() TIKI_OVERRIDE_FINAL;
		virtual void	shutdownTool() TIKI_OVERRIDE_FINAL;

		virtual void	updateTool( bool wantToShutdown ) TIKI_OVERRIDE_FINAL;
		virtual void	renderTool( GraphicsContext& graphicsContext ) const TIKI_OVERRIDE_FINAL;

		virtual void	doUi() TIKI_OVERRIDE_FINAL;

		virtual bool	processToolInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE_FINAL;
		virtual void	processToolWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE_FINAL;

		//void			openFileTab( QWidget* pWidget, const QString& title );
		//void			changeFileTab( QWidget* pWidget, const QString& title );
		//void			selectFileTab( QWidget* pWidget );
		//void			closeFileTab( QWidget* pWidget );

	//signals:
	//
	//	void			fileCloseRequest( QWidget* pWidget );
	//
	//private slots:
	//
	//	void			fileCloseRequested( int index );

	private:

		Project						m_project;
		Editor						m_editor;

		EditorRibbon*				m_pCurrentRibbon;
	};
}
