#pragma once
#ifndef TIKI_EDITOR_HPP_INCLUDED
#define TIKI_EDITOR_HPP_INCLUDED

#include "tiki/editorinterface/ieditorinterface.hpp"

#include <QSet>

class QWidget;

namespace tiki
{
	class EditorFile;
	class EditorWindow;
	class IEditorFile;

	class Editor : public IEditorInterface
	{
		TIKI_NONCOPYABLE_CLASS( Editor );

	public:

								Editor( EditorWindow* pWindow );
		virtual					~Editor();

		virtual IEditorFile*	openFile( const QString& fileName ) TIKI_OVERRIDE_FINAL;
		virtual void			saveFile( IEditorFile* pFile ) TIKI_OVERRIDE_FINAL;
		virtual void			closeFile( IEditorFile* pFile ) TIKI_OVERRIDE_FINAL;
		virtual void			closeAllFiles() TIKI_OVERRIDE_FINAL;

		virtual void			registerFileEditor( IFileEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual void			unregisterFileEditor( IFileEditor* pEditor ) TIKI_OVERRIDE_FINAL;

		virtual void			addGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_OVERRIDE_FINAL;
		virtual void			removeGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_OVERRIDE_FINAL;

		virtual void			addGlobalDockWidget( QDockWidget* pWidget ) TIKI_OVERRIDE_FINAL;
		virtual void			removeGlobalDockWidget( QDockWidget* pWidget ) TIKI_OVERRIDE_FINAL;

		virtual QString			getProjectPath() const TIKI_OVERRIDE_FINAL;
		virtual QString			getContentPath() const TIKI_OVERRIDE_FINAL;
		virtual QString			getPackagePath() const TIKI_OVERRIDE_FINAL;

		virtual QWidget*		getDialogParent() const TIKI_OVERRIDE_FINAL;
		virtual QString			getDialogTitle() const TIKI_OVERRIDE_FINAL;

		void					markFileAsDirty( EditorFile* pFile );

	private:

		EditorWindow*			m_pWindow;

		QString					m_projectPath;
		QString					m_contentPath;
		QString					m_packagePath;

		QSet< IFileEditor* >	m_editors;
		QSet< QDockWidget* >	m_docks;

		QSet< EditorFile* >		m_files;
		EditorFile*				m_pCurrentFile;

		void					findProjectPathes();

		IFileEditor*			findEditorForFile( const QString& fileName ) const;

		void					beginEditing( EditorFile* pFile );
		bool					endEditing( EditorFile* pNextFile );
	};
}

#endif // TIKI_EDITOR_HPP_INCLUDED
