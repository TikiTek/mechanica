#pragma once
#ifndef TIKI_EDITOR_HPP_INCLUDED
#define TIKI_EDITOR_HPP_INCLUDED

#include "tiki/editorinterface/ieditorinterface.hpp"
#include "tiki/packageeditor/packageeditor.hpp"

#include <QSet>
#include <QShortcut>

class QWidget;
class QCommandLineOption;

namespace tiki
{
	class EditorFile;
	class EditorWindow;
	class IEditorFile;

	class Editor : public QObject, public IEditorInterface
	{
		Q_OBJECT
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
		virtual IFileEditor*	findEditorForFile( const QString& fileName ) const TIKI_OVERRIDE_FINAL;

		virtual void			addGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_OVERRIDE_FINAL;
		virtual void			removeGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_OVERRIDE_FINAL;

		virtual void			addGlobalDockWidget( QDockWidget* pWidget ) TIKI_OVERRIDE_FINAL;
		virtual void			removeGlobalDockWidget( QDockWidget* pWidget ) TIKI_OVERRIDE_FINAL;

		virtual QDir			getProjectPath() const TIKI_OVERRIDE_FINAL;
		virtual QDir			getContentPath() const TIKI_OVERRIDE_FINAL;
		virtual QDir			getPackagePath() const TIKI_OVERRIDE_FINAL;

		virtual QWidget*		getDialogParent() const TIKI_OVERRIDE_FINAL;
		virtual QString			getDialogTitle() const TIKI_OVERRIDE_FINAL;

		void					markFileAsDirty( EditorFile* pFile );

	private slots:

	void						fileOpenShortcut();
	void						fileSaveShortcut();
	void						fileCloseShortcut();
	void						fileCloseRequest( QWidget* pWidget );

	private:

		EditorWindow*			m_pWindow;

		QDir					m_projectPath;
		QDir					m_contentPath;
		QDir					m_packagePath;

		QSet< IFileEditor* >	m_editors;
		PackageEditor*			m_pPackageEditor;

		QSet< QDockWidget* >	m_docks;

		QSet< EditorFile* >		m_files;
		EditorFile*				m_pCurrentFile;

		QShortcut				m_openShortcut;
		QShortcut				m_saveShortcut;
		QShortcut				m_closeShortcut;

		void					setProjectPathes();
		void					setPackagePath();

		void					beginEditing( EditorFile* pFile );
		bool					endEditing( EditorFile* pNextFile );
	};
}

#endif // TIKI_EDITOR_HPP_INCLUDED
