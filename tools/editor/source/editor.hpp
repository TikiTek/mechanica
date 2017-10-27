#pragma once
#ifndef TIKI_EDITOR_HPP_INCLUDED
#define TIKI_EDITOR_HPP_INCLUDED

#include "tiki/editor_interface/ieditorinterface.hpp"

#include <QSet>
#include <QShortcut>

class QDockWidget;

namespace tiki
{
	class ConverterEditor;
	class EditorFile;
	class EditorWindow;
	class GenericDataEditor;
	class IFile;
	class PackageEditor;

	class Editor : public QObject, public IEditorInterface
	{
		Q_OBJECT
		TIKI_NONCOPYABLE_CLASS( Editor );

	public:

								Editor( EditorWindow* pWindow );
		virtual					~Editor();

		virtual IFile*			openFile( const QString& fileName ) TIKI_OVERRIDE_FINAL;
		virtual void			saveFile( IFile* pFile ) TIKI_OVERRIDE_FINAL;
		virtual void			closeFile( IFile* pFile ) TIKI_OVERRIDE_FINAL;
		virtual void			closeAllFiles() TIKI_OVERRIDE_FINAL;

		virtual void			registerFileEditor( IFileEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual void			unregisterFileEditor( IFileEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual IFileEditor*	findEditorForFile( const QString& fileName ) const TIKI_OVERRIDE_FINAL;

		virtual void			registerEditorExtension( IEditorExtension* pExtension ) TIKI_OVERRIDE_FINAL;
		virtual void			unregisterEditorExtension( IEditorExtension* pExtension ) TIKI_OVERRIDE_FINAL;

		virtual QDir			getProjectPath() const TIKI_OVERRIDE_FINAL;
		virtual QDir			getContentPath() const TIKI_OVERRIDE_FINAL;
		virtual QDir			getPackagePath() const TIKI_OVERRIDE_FINAL;

		virtual QWidget*		getDialogParent() const TIKI_OVERRIDE_FINAL;
		virtual QString			getDialogTitle() const TIKI_OVERRIDE_FINAL;

		void					markFileAsDirty( EditorFile* pFile );

	private slots:

		void					fileOpenShortcut();
		void					fileSaveShortcut();
		void					fileCloseShortcut();
		void					fileCloseRequest( QWidget* pWidget );

	private:

		EditorWindow*				m_pWindow;

		QDir						m_projectPath;
		QDir						m_contentPath;
		QDir						m_packagePath;

		QSet< IFileEditor* >		m_editors;
		QSet< IEditorExtension* >	m_extensions;

		PackageEditor*				m_pPackageEditor;
		GenericDataEditor*			m_pGenericDataEditor;
		ConverterEditor*			m_pConverterEditor;

		QSet< QDockWidget* >		m_docks;

		QSet< EditorFile* >			m_files;
		EditorFile*					m_pCurrentFile;

		QShortcut					m_openShortcut;
		QShortcut					m_saveShortcut;
		QShortcut					m_closeShortcut;

		void						setProjectPathes();
		void						setPackagePath();

		void						beginEditing( EditorFile* pFile );
		bool						endEditing( EditorFile* pNextFile );
	};
}

#endif // TIKI_EDITOR_HPP_INCLUDED
