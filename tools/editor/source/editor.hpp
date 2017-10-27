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
	class EditorEditable;
	class EditorEditable;
	class EditorWindow;
	class GenericDataEditor;
	class PackageEditor;

	class Editor : public QObject, public IEditorInterface
	{
		Q_OBJECT
		TIKI_NONCOPYABLE_CLASS( Editor );

	public:

								Editor( EditorWindow* pWindow );
		virtual					~Editor();

		virtual IEditable*		openEditable( const QString& title, QWidget* pEditWidget, IEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual IFile*			openFile( const QString& fileName ) TIKI_OVERRIDE_FINAL;
		virtual void			saveEditable( IEditable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void			closeEditable( IEditable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void			closeAll() TIKI_OVERRIDE_FINAL;

		virtual void			registerFileEditor( IFileEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual void			unregisterFileEditor( IFileEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual IFileEditor*	findEditorForFile( const QString& fileName ) const;

		virtual void			addGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_OVERRIDE_FINAL;
		virtual void			removeGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_OVERRIDE_FINAL;
		virtual void			addGlobalDockWidget( QDockWidget* pWidget ) TIKI_OVERRIDE_FINAL;
		virtual void			removeGlobalDockWidget( QDockWidget* pWidget ) TIKI_OVERRIDE_FINAL;

		virtual QDir			getProjectPath() const TIKI_OVERRIDE_FINAL;
		virtual QDir			getContentPath() const TIKI_OVERRIDE_FINAL;
		virtual QDir			getPackagePath() const TIKI_OVERRIDE_FINAL;

		virtual QWidget*		getDialogParent() const TIKI_OVERRIDE_FINAL;
		virtual QString			getDialogTitle() const TIKI_OVERRIDE_FINAL;

		void					markFileAsDirty( EditorEditable* pEditorFile );

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

		PackageEditor*				m_pPackageEditor;
		GenericDataEditor*			m_pGenericDataEditor;
		ConverterEditor*			m_pConverterEditor;

		QSet< QDockWidget* >		m_docks;

		QSet< EditorEditable* >			m_editables;
		EditorEditable*					m_pCurrentEditable;

		QShortcut					m_openShortcut;
		QShortcut					m_saveShortcut;
		QShortcut					m_closeShortcut;

		void						setProjectPathes();
		void						setPackagePath();

		void						beginEditing( EditorEditable* pEditorFile );
		bool						endEditing( EditorEditable* pNextEditorFile );
	};
}

#endif // TIKI_EDITOR_HPP_INCLUDED
