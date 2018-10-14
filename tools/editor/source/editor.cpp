#include "editor.hpp"

#include "tiki/asset_converter_interface/asset_converter_interface.hpp"
#include "tiki/base/assert.hpp"
#include "tiki/converter_editor/converter_editor.hpp"
#include "tiki/editor_interface/editable_file.hpp"
#include "tiki/generic_data_editor/generic_data_editor.hpp"
#include "tiki/io/directory.hpp"
#include "tiki/io/file.hpp"
#include "tiki/package_editor/package_editor.hpp"
#include "tiki/resource/resourcemanager.hpp"
#include "tiki/toolapplication/tool_framework.hpp"

namespace tiki
{
	Editor::Editor()
		: m_pCurrentEditable( nullptr )
		//, m_openShortcut( QKeySequence( QKeySequence::Open ), m_pWindow )
		//, m_saveShortcut( QKeySequence( QKeySequence::Save ), m_pWindow )
		//, m_closeShortcut( QKeySequence( Qt::CTRL + Qt::Key_W ), m_pWindow )
	{
	}

	Editor::~Editor()
	{
	}

	bool Editor::create()
	{
		setProjectPathes();

		AssetConverterParamter assetConverterParameters;
		assetConverterParameters.pProject = &m_project;

		m_pAssetConverter = tool::getResourceManager().getAssetConverter();

		m_pPackageEditor = new PackageEditor( this );
		m_pGenericDataEditor = new GenericDataEditor( this );
		m_pConverterEditor = new ConverterEditor( this, m_pAssetConverter );

		registerFileEditor( m_pPackageEditor );
		registerFileEditor( m_pGenericDataEditor );

		//connect( m_pWindow, &EditorWindow::fileCloseRequest, this, &Editor::fileCloseRequest );
		//connect( &m_openShortcut, &QShortcut::activated, this, &Editor::fileOpenShortcut );
		//connect( &m_saveShortcut, &QShortcut::activated, this, &Editor::fileSaveShortcut );
		//connect( &m_closeShortcut, &QShortcut::activated, this, &Editor::fileCloseShortcut );

		return true;
	}

	void Editor::dispose()
	{
		unregisterFileEditor( m_pGenericDataEditor );
		unregisterFileEditor( m_pPackageEditor );

		delete m_pConverterEditor;
		delete m_pGenericDataEditor;
		delete m_pPackageEditor;
	}

	Editable* Editor::openEditable( const DynamicString& title, BaseEditor* pEditor )
	{
		TIKI_ASSERT( pEditor != nullptr );

		for( Editable* pEditable : m_editables )
		{
			if( pEditable->getTitle() == title &&
				pEditable->getEditor() == pEditor )
			{
				m_pCurrentEditable = pEditable;
				return pEditable;
			}
		}

		Editable* pEditable = pEditor->openEditable( title );
		if( pEditable != nullptr )
		{
			m_editables.add( pEditable );
		}

		m_pCurrentEditable = pEditable;
		return pEditable;
	}

	EditableFile* Editor::openFile( const Path& fileName )
	{
		if( fileName.isEmpty() )
		{
			return nullptr;
		}

		for( Editable* pEditable : m_editables )
		{
			EditableFile* pFile = pEditable->asFile();
			if( pFile == nullptr )
			{
				continue;
			}

			if( pFile->getFileName() == fileName )
			{

				return pFile;
			}
		}

		if( !file::exists( fileName.getCompletePath() ) )
		{
			//QMessageBox::warning( getDialogParent(), getDialogTitle(), "File not found. Please choose an other file." );
			return nullptr;
		}

		FileEditor* pEditor = findEditorForFile( fileName );
		if( pEditor == nullptr )
		{
			//QMessageBox::warning( getDialogParent(), getDialogTitle(), "File type is not supported. Please choose an other file." );
			return nullptr;
		}

		EditableFile* pFile = pEditor->openFile( fileName );
		if( pFile == nullptr )
		{
			return nullptr;
		}

		m_editables.pushBack( pFile );

		if( pEditor == m_pPackageEditor )
		{
			setPackagePath();
		}

		m_pCurrentEditable = pFile;
		return pFile;
	}

	void Editor::saveEditable( Editable* pEditable )
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
	}

	void Editor::closeEditable( Editable* pEditable )
	{
		TIKI_ASSERT( pEditable != nullptr );

		if( pEditable->isDirty() )
		{
			EditableFile* pFile = pEditable->asFile();

			DynamicString fileName;
			if( pFile != nullptr )
			{
				fileName = pFile->getFileName().getFilenameWithExtension();
			}
			else
			{
				fileName = pEditable->getTitle();
			}

			const DynamicString text = "Do you want to save changes to '" + fileName + "'?";
			//const QMessageBox::StandardButton button = (QMessageBox::StandardButton)QMessageBox::information( m_pWindow, m_pWindow->windowTitle(), text, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes );
			//switch( button )
			//{
			//case QMessageBox::Yes:
			//	saveEditable( pEditable );
			//	break;
			//
			//case QMessageBox::Cancel:
			//	return;
			//
			//default:
			//	break;
			//}
		}

		if( pEditable == m_pCurrentEditable )
		{
			m_pCurrentEditable = nullptr;
		}

		m_editables.removeSortedByValue( pEditable );
		pEditable->getEditor()->closeEditable( pEditable );
	}

	void Editor::closeAll()
	{
		List< Editable* > editables = m_editables;
		for( Editable* pEditable : editables )
		{
			closeEditable( pEditable );
		}
	}

	void Editor::registerFileEditor( FileEditor* pEditor )
	{
		m_editors.pushBack( pEditor );
	}

	void Editor::unregisterFileEditor( FileEditor* pEditor )
	{
		m_editors.removeSortedByValue( pEditor );
	}

	void Editor::addGlobalRibbon( EditorRibbon* pRibbon )
	{
		m_ribbons.pushBack( pRibbon );
	}

	void Editor::removeGlobalRibbon( EditorRibbon* pRibbon )
	{
		m_ribbons.removeSortedByValue( pRibbon );
	}

	//void Editor::addGlobalDockWidget( QDockWidget* pWidget, Qt::DockWidgetArea area )
	//{
	//	m_pWindow->addDockWidget( area, pWidget );
	//}

	//void Editor::removeGlobalDockWidget( QDockWidget* pWidget )
	//{
	//	m_pWindow->removeDockWidget( pWidget );
	//}

	const Path& Editor::getProjectPath() const
	{
		return m_projectPath;
	}

	const Path& Editor::getContentPath() const
	{
		return m_contentPath;
	}

	const Path& Editor::getPackagePath() const
	{
		return m_packagePath;
	}

	//QWidget* Editor::getDialogParent() const
	//{
	//	return m_pWindow;
	//}

	//QString Editor::getDialogTitle() const
	//{
	//	return m_pWindow->windowTitle();
	//}

	//void Editor::markFileAsDirty( EditorEditable* pEditable )
	//{
	//	EditorEditable* pEditorFile = (EditorEditable*)pEditable;
	//	m_pWindow->changeFileTab( pEditable->getEditWidget(), pEditorFile->getTabTitle() );
	//}

	//void Editor::fileOpenShortcut()
	//{
	//	QString allFilter = "All supported files (";
	//	QString filter;
	//	foreach( IFileEditor* pEditor, m_editors )
	//	{
	//		if( !filter.isEmpty() )
	//		{
	//			allFilter += " ";
	//			filter += ";;";
	//		}
	//		allFilter += QString( "*.%1" ).arg( pEditor->getFileExtension() );
	//		filter += QString( "%1 (*.%2)" ).arg( pEditor->getFileTypeName(), pEditor->getFileExtension() );
	//	}
	//	filter = allFilter + ");;" + filter + ";;All files (*.*)";

	//	const QString fileName = QFileDialog::getOpenFileName(
	//		getDialogParent(),
	//		getDialogTitle(),
	//		getPackagePath().absolutePath(),
	//		filter
	//	);

	//	openFile( fileName );
	//}

	//void Editor::fileSaveShortcut()
	//{
	//	if( m_pCurrentEditable != nullptr )
	//	{
	//		saveEditable( m_pCurrentEditable );
	//	}
	//}

	//void Editor::fileCloseShortcut()
	//{
	//	if( m_pCurrentEditable != nullptr )
	//	{
	//		closeEditable( m_pCurrentEditable );
	//	}
	//}

	//void Editor::fileCloseRequest( QWidget* pWidget )
	//{
	//	foreach( IEditable* pEditable, m_editables )
	//	{
	//		if( pEditable->getEditWidget() == pWidget )
	//		{
	//			closeEditable( pEditable );
	//			return;
	//		}
	//	}
	//}

	void Editor::setProjectPathes()
	{
		Path currentPath( platform::getExecutablePath() );
		do
		{
			currentPath.push( "tikiproject.xml" );
			if( file::exists( currentPath.getCompletePath() ) )
			{
				currentPath.pop();
				m_projectPath = currentPath;

				Path contentPath = currentPath;
				contentPath.push( "content" );
				if( !directory::exists( contentPath.getCompletePath() ) )
				{
					directory::create( contentPath.getCompletePath() );
				}
				m_contentPath = contentPath;

				return;
			}
			currentPath.pop();
		}
		while ( currentPath.pop() );

		//QMessageBox::critical( getDialogParent(), getDialogTitle(), "Unable to find Project directory. Please place tikiproject.xml in the project root." );
		//QApplication::exit( 1 );
	}

	void Editor::setPackagePath()
	{
		const DynamicString packageName = m_pPackageEditor->getPackageName();

		Path packagePath = m_contentPath;
		packagePath.push( packageName.cStr() );
		if( !directory::exists( packagePath.getCompletePath() ) )
		{
			directory::create( packagePath.getCompletePath() );
		}
		m_packagePath = packagePath;
	}

	FileEditor* Editor::findEditorForFile( const Path& filename ) const
	{
		const char* pExtension = filename.getExtension();
		for( BaseEditor* pEditor : m_editors )
		{
			FileEditor* pFileEditor = pEditor->asFileEditor();
			if( pFileEditor == nullptr )
			{
				continue;
			}

			if( pFileEditor->getFileExtension() == pExtension )
			{
				return pFileEditor;
			}
		}

		return nullptr;
	}

	//void Editor::beginEditing( Editable* pEditorFile )
	//{
	//	TIKI_ASSERT( pEditorFile != nullptr );

	//	if( endEditing( pEditorFile ) )
	//	{
	//		IEditor* pNextEditor = pEditorFile->getEditor();

	//		foreach( QtRibbonTab* pTab, pNextEditor->getEditableRibbonTabs() )
	//		{
	//			m_pWindow->addRibbonTab( pTab );
	//		}

	//		foreach( QDockWidget* pDockWidget, pNextEditor->getEditableDockWidgets() )
	//		{
	//			m_pWindow->addDockWidget( Qt::LeftDockWidgetArea, pDockWidget );
	//		}
	//	}

	//	m_pCurrentEditable = pEditorFile;
	//}

	//bool Editor::endEditing( Editable* pNextEditorFile )
	//{
	//	if( m_pCurrentEditable == nullptr )
	//	{
	//		return true;
	//	}

	//	if( pNextEditorFile == nullptr || m_pCurrentEditable->getEditor() != pNextEditorFile->getEditor() )
	//	{
	//		IEditor* pCurrentEditor = m_pCurrentEditable->getEditor();

	//		foreach( QtRibbonTab* pTab, pCurrentEditor->getEditableRibbonTabs() )
	//		{
	//			m_pWindow->removeRibbonTab( pTab );
	//		}

	//		foreach( QDockWidget* pDockWidget, pCurrentEditor->getEditableDockWidgets() )
	//		{
	//			m_pWindow->removeDockWidget( pDockWidget );
	//		}

	//		m_pCurrentEditable = nullptr;
	//		return true;
	//	}

	//	m_pCurrentEditable = nullptr;
	//	return false;
	//}
}