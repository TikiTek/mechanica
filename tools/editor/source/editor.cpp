#include "editor.hpp"

#include "tiki/base/assert.hpp"

#include "editorwindow.hpp"
#include "editorfile.hpp"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

namespace tiki
{
	Editor::Editor( EditorWindow* pWindow )
		: m_pWindow( pWindow )
		, m_pCurrentFile( nullptr )
	{
		findProjectPathes();
	}

	Editor::~Editor()
	{
	}

	IEditorFile* Editor::openFile( const QString& fileName )
	{
		if( fileName.isEmpty() )
		{
			return nullptr;
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

		m_pWindow->openFileTab( pEditWidget, pFile->getTabTitle() );
		beginEditing( pFile );
		return pFile;
	}

	void Editor::saveFile( IEditorFile* pFile )
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

	void Editor::closeFile( IEditorFile* pFile )
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

		m_pWindow->closeFileTab( pEditorFile->getEditWidget() );
		pEditorFile->getFileEditor()->closeFile( pEditorFile );
		m_files.remove( pEditorFile );
		delete pFile;
	}

	void Editor::closeAllFiles()
	{

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

	QString Editor::getProjectPath() const
	{
		return m_projectPath;
	}

	QString Editor::getContentPath() const
	{
		return m_contentPath;
	}

	QString Editor::getPackagePath() const
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

	void Editor::findProjectPathes()
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