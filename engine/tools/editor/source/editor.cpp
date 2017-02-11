#include "editor.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/genericdataeditor/genericdataeditor.hpp"
#include "tiki/packageeditor/package_editor.hpp"

#include "editorwindow.hpp"
#include "editorfile.hpp"

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

namespace tiki
{
	Editor::Editor( EditorWindow* pWindow )
		: m_pWindow( pWindow )
		, m_pCurrentFile( nullptr )
		, m_openShortcut( QKeySequence( QKeySequence::Open ), m_pWindow )
		, m_saveShortcut( QKeySequence( QKeySequence::Save ), m_pWindow )
		, m_closeShortcut( QKeySequence( Qt::CTRL + Qt::Key_W ), m_pWindow )
	{
		setProjectPathes();

		m_pPackageEditor = new PackageEditor( this );
		m_pGenericDataEditor = new GenericDataEditor( this );

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

		delete m_pGenericDataEditor;
		delete m_pPackageEditor;
	}

	IFile* Editor::openFile( const QString& fileName )
	{
		if( fileName.isEmpty() )
		{
			return nullptr;
		}

		foreach( EditorFile* pFile, m_files )
		{
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

		EditorFile* pFile = new EditorFile( this, fileName, pEditor );

		QWidget* pEditWidget = pEditor->openFile( pFile );
		if( pEditWidget == nullptr )
		{
			delete pFile;
			return nullptr;
		}

		pFile->setEditWidget( pEditWidget );
		m_files.insert( pFile );

		if( pEditor == m_pPackageEditor )
		{
			setPackagePath();
		}

		m_pWindow->openFileTab( pEditWidget, pFile->getTabTitle() );
		beginEditing( pFile );
		return pFile;
	}

	void Editor::saveFile( IFile* pFile )
	{
		TIKI_ASSERT( pFile != nullptr );

		EditorFile* pEditorFile = (EditorFile*)pFile;
		if( !pEditorFile->isDirty() )
		{
			return;
		}

		if( !pEditorFile->getFileEditor()->saveFile( pEditorFile ) )
		{
			return;
		}

		pEditorFile->markAsSaved();
		m_pWindow->changeFileTab( pFile->getEditWidget(), pEditorFile->getTabTitle() );
	}

	void Editor::closeFile( IFile* pFile )
	{
		TIKI_ASSERT( pFile != nullptr );

		EditorFile* pEditorFile = (EditorFile*)pFile;
		if( pEditorFile->isDirty() )
		{
			QFileInfo fileInfo( pEditorFile->getFileName() );
			QString text = "Do you want to save changes to " + fileInfo.completeBaseName() + "?";

			const QMessageBox::StandardButton button = (QMessageBox::StandardButton)QMessageBox::information( m_pWindow, m_pWindow->windowTitle(), text, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes );
			switch( button )
			{
			case QMessageBox::Yes:
				saveFile( pEditorFile );
				break;

			case QMessageBox::Cancel:
				return;

			default:
				break;
			}
		}

		if( pEditorFile == m_pCurrentFile )
		{
			endEditing( nullptr );
		}

		m_pWindow->closeFileTab( pEditorFile->getEditWidget() );
		pEditorFile->getFileEditor()->closeFile( pEditorFile );
		m_files.remove( pEditorFile );
		delete pFile;
	}

	void Editor::closeAllFiles()
	{
		foreach( EditorFile* pfile, m_files )
		{
			closeFile( pfile );
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

	void Editor::markFileAsDirty( EditorFile* pFile )
	{
		m_pWindow->changeFileTab( pFile->getEditWidget(), pFile->getTabTitle() );
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
		if( m_pCurrentFile != nullptr )
		{
			saveFile( m_pCurrentFile );
		}
	}

	void Editor::fileCloseShortcut()
	{
		if( m_pCurrentFile != nullptr )
		{
			closeFile( m_pCurrentFile );
		}
	}

	void Editor::fileCloseRequest( QWidget* pWidget )
	{
		foreach( EditorFile* pFile, m_files )
		{
			if( pFile->getEditWidget() == pWidget )
			{
				closeFile( pFile );
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

	void Editor::beginEditing( EditorFile* pFile )
	{
		TIKI_ASSERT( pFile != nullptr );

		if( endEditing( pFile ) )
		{
			IFileEditor* pNextEditor = pFile->getFileEditor();

			foreach( QtRibbonTab* pTab, pNextEditor->getRibbonTabs() )
			{
				m_pWindow->addRibbonTab( pTab );
			}

			foreach( QDockWidget* pDockWidget, pNextEditor->getDockWidgets() )
			{
				m_pWindow->addDockWidget( Qt::LeftDockWidgetArea, pDockWidget );
			}
		}

		m_pCurrentFile = pFile;
	}

	bool Editor::endEditing( EditorFile* pNextFile )
	{
		if( m_pCurrentFile == nullptr )
		{
			return true;
		}

		if( pNextFile == nullptr || m_pCurrentFile->getFileEditor() != pNextFile->getFileEditor() )
		{
			IFileEditor* pCurrentEditor = m_pCurrentFile->getFileEditor();

			foreach( QtRibbonTab* pTab, pCurrentEditor->getRibbonTabs() )
			{
				m_pWindow->removeRibbonTab( pTab );
			}

			foreach( QDockWidget* pDockWidget, pCurrentEditor->getDockWidgets() )
			{
				m_pWindow->removeDockWidget( pDockWidget );
			}

			m_pCurrentFile = nullptr;
			return true;
		}

		m_pCurrentFile = nullptr;
		return false;
	}
}