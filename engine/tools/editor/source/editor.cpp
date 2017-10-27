#include "editor.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/converter_editor/converter_editor.hpp"
#include "tiki/generic_data_editor/generic_data_editor.hpp"
#include "tiki/package_editor/package_editor.hpp"

#include "editor_window.hpp"
#include "editor_editable.hpp"

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

namespace tiki
{
	Editor::Editor( EditorWindow* pWindow )
		: m_pWindow( pWindow )
		, m_pCurrentEditable( nullptr )
		, m_openShortcut( QKeySequence( QKeySequence::Open ), m_pWindow )
		, m_saveShortcut( QKeySequence( QKeySequence::Save ), m_pWindow )
		, m_closeShortcut( QKeySequence( Qt::CTRL + Qt::Key_W ), m_pWindow )
	{
		setProjectPathes();

		m_pPackageEditor = new PackageEditor( this );
		m_pGenericDataEditor = new GenericDataEditor( this );
		m_pConverterEditor = new ConverterEditor( this, nullptr );

		registerFileEditor( m_pPackageEditor );
		registerFileEditor( m_pGenericDataEditor );

		connect( m_pWindow, &EditorWindow::fileCloseRequest, this, &Editor::fileCloseRequest );
		connect( &m_openShortcut, &QShortcut::activated, this, &Editor::fileOpenShortcut );
		connect( &m_saveShortcut, &QShortcut::activated, this, &Editor::fileSaveShortcut );
		connect( &m_closeShortcut, &QShortcut::activated, this, &Editor::fileCloseShortcut );
	}

	Editor::~Editor()
	{
		unregisterFileEditor( m_pGenericDataEditor );
		unregisterFileEditor( m_pPackageEditor );

		delete m_pConverterEditor;
		delete m_pGenericDataEditor;
		delete m_pPackageEditor;
	}

	IEditable* Editor::openEditable( const QString& title, QWidget* pEditWidget, IEditor* pEditor )
	{
		TIKI_ASSERT( pEditWidget != nullptr );
		TIKI_ASSERT( pEditor != nullptr );

		foreach( IEditable* pEditable, m_editables )
		{
			if( pEditable->getEditWidget() == pEditWidget )
			{
				return pEditable;
			}
		}

		EditorEditable* pFile = new EditorEditable( this, title, pEditWidget, pEditor );
		m_editables.insert( pFile );

		m_pWindow->openFileTab( pEditWidget, pFile->getTabTitle() );
		beginEditing( pFile );
		return pFile;
	}

	IFile* Editor::openFile( const QString& fileName )
	{
		if( fileName.isEmpty() )
		{
			return nullptr;
		}

		foreach( IEditable* pEditable, m_editables )
		{
			IFile* pFile = pEditable->asFile();
			if( pFile == nullptr )
			{
				continue;
			}

			if( pFile->getFileName() == fileName )
			{
				return pFile;
			}
		}

		if( !QFile::exists( fileName ) )
		{
			QMessageBox::warning( getDialogParent(), getDialogTitle(), "File not found. Please choose an other file." );
			return nullptr;
		}

		IFileEditor* pEditor = findEditorForFile( fileName );
		if( pEditor == nullptr )
		{
			QMessageBox::warning( getDialogParent(), getDialogTitle(), "File type is not supported. Please choose an other file." );
			return nullptr;
		}

		EditorEditable* pFile = new EditorEditable( this, fileName, pEditor );

		QWidget* pEditWidget = pEditor->openFile( pFile );
		if( pEditWidget == nullptr )
		{
			delete pFile;
			return nullptr;
		}

		pFile->setEditWidget( pEditWidget );
		m_editables.insert( pFile );

		if( pEditor == m_pPackageEditor )
		{
			setPackagePath();
		}

		m_pWindow->openFileTab( pEditWidget, pFile->getTabTitle() );
		beginEditing( pFile );
		return pFile;
	}

	void Editor::saveEditable( IEditable* pEditable )
	{
		TIKI_ASSERT( pEditable != nullptr );

		if( !pEditable->isDirty() )
		{
			return;
		}

		if( !pEditable->getEditor()->saveEditable( pEditable ) )
		{
			return;
		}

		EditorEditable* pEditorFile = (EditorEditable*)pEditable;
		pEditorFile->markAsSaved();

		m_pWindow->changeFileTab( pEditable->getEditWidget(), pEditorFile->getTabTitle() );
	}

	void Editor::closeEditable( IEditable* pEditable )
	{
		TIKI_ASSERT( pEditable != nullptr );

		if( pEditable->isDirty() )
		{
			QString fileName;

			IFile* pFile = pEditable->asFile();
			if( pFile != nullptr )
			{
				fileName = QFileInfo( pFile->getFileName() ).completeBaseName();
			}
			else
			{
				fileName = pEditable->getTitle();
			}

			const QString text = "Do you want to save changes to '" + fileName + "'?";
			const QMessageBox::StandardButton button = (QMessageBox::StandardButton)QMessageBox::information( m_pWindow, m_pWindow->windowTitle(), text, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes );
			switch( button )
			{
			case QMessageBox::Yes:
				saveEditable( pEditable );
				break;

			case QMessageBox::Cancel:
				return;

			default:
				break;
			}
		}

		if( pEditable == m_pCurrentEditable )
		{
			endEditing( nullptr );
		}

		m_pWindow->closeFileTab( pEditable->getEditWidget() );
		pEditable->getEditor()->closeEditable( pEditable );

		EditorEditable* pEditorFile = (EditorEditable*)pEditable;
		m_editables.remove( pEditorFile );
		delete pEditorFile;
	}

	void Editor::closeAll()
	{
		foreach( IEditable* pEditable, m_editables )
		{
			closeEditable( pEditable );
		}
	}

	void Editor::registerFileEditor( IFileEditor* pEditor )
	{
		m_editors.insert( pEditor );
	}

	void Editor::unregisterFileEditor( IFileEditor* pEditor )
	{
		m_editors.remove( pEditor );
	}

	void Editor::addGlobalRibbonTab( QtRibbonTab* pTab )
	{
		m_pWindow->addRibbonTab( pTab );
	}

	void Editor::removeGlobalRibbonTab( QtRibbonTab* pTab )
	{
		m_pWindow->removeRibbonTab( pTab );
	}

	void Editor::addGlobalDockWidget( QDockWidget* pWidget )
	{
		m_pWindow->addDockWidget( Qt::LeftDockWidgetArea, pWidget );
	}

	void Editor::removeGlobalDockWidget( QDockWidget* pWidget )
	{
		m_pWindow->removeDockWidget( pWidget );
	}

	QDir Editor::getProjectPath() const
	{
		return m_projectPath;
	}

	QDir Editor::getContentPath() const
	{
		return m_contentPath;
	}

	QDir Editor::getPackagePath() const
	{
		return m_packagePath;
	}

	QWidget* Editor::getDialogParent() const
	{
		return m_pWindow;
	}

	QString Editor::getDialogTitle() const
	{
		return m_pWindow->windowTitle();
	}

	void Editor::markFileAsDirty( EditorEditable* pEditable )
	{
		EditorEditable* pEditorFile = (EditorEditable*)pEditable;
		m_pWindow->changeFileTab( pEditable->getEditWidget(), pEditorFile->getTabTitle() );
	}

	void Editor::fileOpenShortcut()
	{
		QString allFilter = "All supported files (";
		QString filter;
		foreach( IFileEditor* pEditor, m_editors )
		{
			if( !filter.isEmpty() )
			{
				allFilter += " ";
				filter += ";;";
			}
			allFilter += QString( "*.%1" ).arg( pEditor->getFileExtension() );
			filter += QString( "%1 (*.%2)" ).arg( pEditor->getFileTypeName(), pEditor->getFileExtension() );
		}
		filter = allFilter + ");;" + filter + ";;All files (*.*)";

		const QString fileName = QFileDialog::getOpenFileName(
			getDialogParent(),
			getDialogTitle(),
			getPackagePath().absolutePath(),
			filter
		);

		openFile( fileName );
	}

	void Editor::fileSaveShortcut()
	{
		if( m_pCurrentEditable != nullptr )
		{
			saveEditable( m_pCurrentEditable );
		}
	}

	void Editor::fileCloseShortcut()
	{
		if( m_pCurrentEditable != nullptr )
		{
			closeEditable( m_pCurrentEditable );
		}
	}

	void Editor::fileCloseRequest( QWidget* pWidget )
	{
		foreach( IEditable* pEditable, m_editables )
		{
			if( pEditable->getEditWidget() == pWidget )
			{
				closeEditable( pEditable );
				return;
			}
		}
	}

	void Editor::setProjectPathes()
	{
		QDir currentDir( QApplication::applicationDirPath() );
		do
		{
			if( currentDir.exists( "tikiproject.xml" ) )
			{
				m_projectPath = currentDir.absolutePath();

				QDir contentDir = currentDir;
				contentDir.cd( "content" );
				if( !currentDir.exists() )
				{
					currentDir.mkdir( "content" );
				}
				m_contentPath = contentDir.absolutePath();

				return;
			}
		}
		while ( currentDir.cdUp() );

		QMessageBox::critical( getDialogParent(), getDialogTitle(), "Unable to find Project directory. Please place tikiproject.xml in the project root." );
		QApplication::exit( 1 );
	}

	void Editor::setPackagePath()
	{
		QString packageName = m_pPackageEditor->getPackageName();

		QDir contentDir( m_contentPath );
		if( !contentDir.exists( packageName ) )
		{
			contentDir.mkdir( packageName );
		}
		contentDir.cd( packageName );

		m_packagePath = contentDir.absolutePath();
	}

	IFileEditor* Editor::findEditorForFile( const QString& fileName ) const
	{
		const QString extension = QFileInfo( fileName ).suffix();
		foreach( IFileEditor* pEditor, m_editors )
		{
			if( pEditor->getFileExtension() == extension )
			{
				return pEditor;
			}
		}

		return nullptr;
	}

	void Editor::beginEditing( EditorEditable* pEditorFile )
	{
		TIKI_ASSERT( pEditorFile != nullptr );

		if( endEditing( pEditorFile ) )
		{
			IEditor* pNextEditor = pEditorFile->getEditor();

			foreach( QtRibbonTab* pTab, pNextEditor->getEditableRibbonTabs() )
			{
				m_pWindow->addRibbonTab( pTab );
			}

			foreach( QDockWidget* pDockWidget, pNextEditor->getEditableDockWidgets() )
			{
				m_pWindow->addDockWidget( Qt::LeftDockWidgetArea, pDockWidget );
			}
		}

		m_pCurrentEditable = pEditorFile;
	}

	bool Editor::endEditing( EditorEditable* pNextEditorFile )
	{
		if( m_pCurrentEditable == nullptr )
		{
			return true;
		}

		if( pNextEditorFile == nullptr || m_pCurrentEditable->getEditor() != pNextEditorFile->getEditor() )
		{
			IEditor* pCurrentEditor = m_pCurrentEditable->getEditor();

			foreach( QtRibbonTab* pTab, pCurrentEditor->getEditableRibbonTabs() )
			{
				m_pWindow->removeRibbonTab( pTab );
			}

			foreach( QDockWidget* pDockWidget, pCurrentEditor->getEditableDockWidgets() )
			{
				m_pWindow->removeDockWidget( pDockWidget );
			}

			m_pCurrentEditable = nullptr;
			return true;
		}

		m_pCurrentEditable = nullptr;
		return false;
	}
}