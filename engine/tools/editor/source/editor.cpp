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
#include "tiki/toolproject/package.hpp"

#include <imgui.h>

namespace tiki
{
	Editor::Editor( Project& project )
		: m_project( project )
		, m_pCurrentRibbon( nullptr )
		, m_pCurrentEditable( nullptr )
		, m_fileBrowserUi( *this )
		, m_editableRibbon( *this )
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

		m_fileBrowserUi.create();

		for( uint i = 1u; i < platform::getArguments().getCount(); ++i )
		{
			const char* pArgument = platform::getArguments()[ i ];
			openFile( Path( pArgument ) );
		}

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

	void Editor::doUi()
	{
		doRibbonUi();
		doBrowserUi();
		doEditableUi();

		for( BaseEditor* pEditor : m_editors )
		{
			pEditor->doUi();
		}
	}

	//Editable* Editor::openEditable( const DynamicString& title, BaseEditor* pEditor )
	//{
	//	TIKI_ASSERT( pEditor != nullptr );

	//	for( Editable* pEditable : m_editables )
	//	{
	//		if( pEditable->getTitle() == title &&
	//			pEditable->getEditor() == pEditor )
	//		{
	//			m_pCurrentEditable = pEditable;
	//			return pEditable;
	//		}
	//	}

	//	Editable* pEditable = pEditor->openEditable( title );
	//	if( pEditable != nullptr )
	//	{
	//		m_editables.add( pEditable );
	//	}

	//	m_pCurrentEditable = pEditable;
	//	return pEditable;
	//}

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

		openEditable( pFile );
		return pFile;
	}

	void Editor::openEditable( Editable* pEditable )
	{
		TIKI_ASSERT( pEditable != nullptr );

		if( !m_editables.contains( pEditable ) )
		{
			m_editables.pushBack( pEditable );
		}

		if( pEditable->getEditor() == m_pPackageEditor )
		{
			setPackagePath();
		}

		m_pCurrentEditable = pEditable;
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

		if( m_pCurrentRibbon == &m_editableRibbon )
		{
			m_pCurrentRibbon = nullptr;
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
		TIKI_ASSERT( pRibbon != nullptr );
		m_ribbons.pushBack( pRibbon );
	}

	void Editor::removeGlobalRibbon( EditorRibbon* pRibbon )
	{
		TIKI_ASSERT( pRibbon != nullptr );

		if( pRibbon == m_pCurrentRibbon )
		{
			m_pCurrentRibbon = nullptr;
		}

		m_ribbons.removeSortedByValue( pRibbon );
	}

	Project& Editor::getProject()
	{
		return m_project;
	}

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
		m_packagePath = m_pPackageEditor->getPackage()->getBasepath();
		if( !directory::exists( m_packagePath.getCompletePath() ) )
		{
			directory::create( m_packagePath.getCompletePath() );
		}

		m_fileBrowserUi.openPackage( *m_pPackageEditor->getPackage() );
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


	void Editor::doRibbonUi()
	{
		const ImGuiIO& io = ImGui::GetIO();

		ImGui::SetNextWindowPos( ImVec2( 5.0f, 5.0f ), ImGuiCond_Always, ImVec2() );
		ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x - 10.0f, 96.0f ), ImGuiCond_Always );
		if( ImGui::Begin( "Ribbon", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav ) )
		{
			//if( ImGui::Button( "test" ) )
			//{
			//	m_messageBox.open( "Hello", "World!\nBla\ndfgdfg\ndgdfgdfg dgdfg dfg d fgdfgd fgdfgd gdf gdfg dfg dfgd", ToolMessageBoxButtons_YesNo, ToolMessageBoxIcon_Error );
			//}
			//ImGui::SameLine();


			for( int i = -1; i < int( m_ribbons.getCount() ); ++i )
			{
				EditorRibbon* pRibbon = nullptr;
				if( i < 0 )
				{
					if( m_pCurrentEditable == nullptr )
					{
						continue;
					}

					pRibbon = &m_editableRibbon;
				}
				else
				{
					pRibbon = m_ribbons[ i ];
				}

				if( m_pCurrentRibbon == nullptr )
				{
					m_pCurrentRibbon = pRibbon;
				}

				bool wasSelected = false;
				if( m_pCurrentRibbon == pRibbon )
				{
					const ImGuiStyle& style = ImGui::GetStyle();
					ImGui::PushStyleColor( ImGuiCol_Button, style.Colors[ ImGuiCol_ButtonHovered ] );
					wasSelected = true;
				}

				if( ImGui::Button( pRibbon->getTitle().cStr() ) )
				{
					m_pCurrentRibbon = pRibbon;
				}

				if( wasSelected )
				{
					ImGui::PopStyleColor();
				}

				ImGui::SameLine();
			}
			ImGui::NewLine();

			if( m_pCurrentRibbon != nullptr )
			{
				m_pCurrentRibbon->doUi();
			}

			m_messageBox.doUi();
			ImGui::End();
		}
	}

	void Editor::doBrowserUi()
	{
		const ImGuiIO& io = ImGui::GetIO();

		ImGui::SetNextWindowPos( ImVec2( 5.0f, 106.0f ), ImGuiCond_Always, ImVec2() );
		ImGui::SetNextWindowSize( ImVec2( 250.0f, io.DisplaySize.y - 111.0f ), ImGuiCond_Always );
		if( ImGui::Begin( "Browser", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav ) )
		{
			m_fileBrowserUi.doUi();
			ImGui::End();
		}
	}

	void Editor::doEditableUi()
	{
		const ImGuiIO& io = ImGui::GetIO();

		ImGui::SetNextWindowPos( ImVec2( 260.0f, 106.0f ), ImGuiCond_Always, ImVec2() );
		ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x - 265.0f, io.DisplaySize.y - 111.0f ), ImGuiCond_Always );
		if( ImGui::Begin( "Editable", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav ) )
		{
			char titleBuffer[ 128u ];
			for( Editable* pEditable : m_editables )
			{
				if( m_pCurrentEditable == nullptr )
				{
					m_pCurrentEditable = pEditable;
				}

				formatStringBuffer( titleBuffer, sizeof( titleBuffer ), "%s%s", pEditable->getTitle().cStr(), pEditable->isDirty() ? "*" : "" );
				if( ImGui::Button( titleBuffer ) )
				{
					m_pCurrentEditable = pEditable;
				}
				ImGui::SameLine();
			}
			ImGui::NewLine();

			if( ImGui::BeginChild( "Editor", ImVec2( 0.0f, 0.0f ), true ) )
			{
				if( m_pCurrentEditable != nullptr )
				{
					m_pCurrentEditable->doUi();
				}

			}
			ImGui::End();
		}
		ImGui::End();
	}
}