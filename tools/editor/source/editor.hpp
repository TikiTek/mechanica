#pragma once
#ifndef TIKI_EDITOR_HPP_INCLUDED
#define TIKI_EDITOR_HPP_INCLUDED

#include "tiki/editorinterface/ieditorinterface.hpp"

#include <QVector>

class QWidget;

namespace tiki
{
	class EditorWindow;

	class Editor : public IEditorInterface
	{
		TIKI_NONCOPYABLE_CLASS( Editor );

	public:

						Editor( EditorWindow* pWindow );
		virtual			~Editor();

		virtual void	openFile( const QString& fileName ) TIKI_OVERRIDE_FINAL;
		virtual void	closeFile( const QString& fileName ) TIKI_OVERRIDE_FINAL;
		virtual void	closeAllFiles() TIKI_OVERRIDE_FINAL;

		virtual void	registerFileEditor( IFileEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual void	unregisterFileEditor( IFileEditor* pEditor ) TIKI_OVERRIDE_FINAL;

		virtual void	addGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_OVERRIDE_FINAL;
		virtual void	removeGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_OVERRIDE_FINAL;

		virtual void	addGlobalDockWidget( QDockWidget* pWidget ) TIKI_OVERRIDE_FINAL;
		virtual void	removeGlobalDockWidget( QDockWidget* pWidget ) TIKI_OVERRIDE_FINAL;

	private:

		struct EditorFile
		{
			QString			fileName;
			IFileEditor*	pEditor;
			QWidget*		pEditWidget;
		};

		EditorWindow*			m_pWindow;

		QVector< IFileEditor* >	m_pEditors;
		QVector< QDockWidget* >	m_pDocks;

		QVector< EditorFile >	m_pFiles;

		EditorFile*				m_pCurrentFile;
		IFileEditor*			m_pCurrentEditor;
		QVector< QtRibbonTab* >	m_currentEditorTabs;
		QVector< QDockWidget* >	m_currentEditorDocks;

		void					beginEditing( EditorFile& pFile );
		void					endEditing( EditorFile& file );
	};
}

#endif // TIKI_EDITOR_HPP_INCLUDED
