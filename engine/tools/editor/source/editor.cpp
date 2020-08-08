#include "editor.hpp"

#include "tiki/asset_converter_interface/asset_converter_interface.hpp"
#include "tiki/base/assert.hpp"
#include "tiki/converter_editor/converter_editor.hpp"
#include "tiki/editor_interface/editable_file.hpp"
#include "tiki/entity_template_editor/entity_template_editor.hpp"
#include "tiki/generic_data_editor/generic_data_editor.hpp"
#include "tiki/io/directory.hpp"
#include "tiki/io/file.hpp"
#include "tiki/package_editor/package_editor.hpp"
#include "tiki/resource/resource_manager.hpp"
#include "tiki/resource/resource_request.hpp"
#include "tiki/tool_application/tool_framework.hpp"
#include "tiki/tool_application/tool_ui.hpp"
#include "tiki/tool_project/package.hpp"

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

		m_pPackageEditor = TIKI_NEW( PackageEditor )( *this );
		m_pGenericDataEditor = TIKI_NEW( GenericDataEditor )( *this, tool::getResourceManager(), tool::getGraphicsSystem() );
		m_pEntityTemplateEditor = TIKI_NEW( EntityTemplateEditor )( *this, *m_pGenericDataEditor );
		m_pConverterEditor = TIKI_NEW( ConverterEditor )( *this, m_pAssetConverter );

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
		for( Editable* pEditable : m_editables )
		{
			closeEditableInternal( *pEditable );
		}

		unregisterFileEditor( m_pGenericDataEditor );
		unregisterFileEditor( m_pPackageEditor );

		TIKI_DELETE( m_pConverterEditor );
		TIKI_DELETE( m_pEntityTemplateEditor );
		TIKI_DELETE( m_pGenericDataEditor );
		TIKI_DELETE( m_pPackageEditor );
	}

	void Editor::update( float deltaTime )
	{
		m_pGenericDataEditor->update();

		if( m_pCurrentEditable != nullptr )
		{
			m_pCurrentEditable->update( deltaTime );
		}
	}

	void Editor::render( GraphicsContext& graphicsContext )
	{
		if( m_pCurrentEditable != nullptr )
		{
			m_pCurrentEditable->render( graphicsContext );
		}
	}

	bool Editor::processToolInputEvent( const InputEvent& inputEvent )
	{
		if( m_pCurrentEditable != nullptr )
		{
			return m_pCurrentEditable->processToolInputEvent( inputEvent );
		}

		return false;
	}

	void Editor::doUi()
	{
		doRibbonUi();

		ImHorizontalSplitter splitter;
		ImGui::BeginHorizontalSplitter( splitter, 5.0f, 106.0f, 5.0f, 5.0f );
		doFileBrowserUi( splitter );
		doEditableUi( splitter );
		ImGui::EndHorizontalSplitter( splitter );

		for( BaseEditor* pEditor : m_editors )
		{
			pEditor->doUi();
		}
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
			showMessageBox( "File not found. Please choose an other file.", ToolMessageBoxButtons_Ok, ToolMessageBoxIcon::Warning );
			return nullptr;
		}

		FileEditor* pEditor = findEditorForFile( fileName );
		if( pEditor == nullptr )
		{
			showMessageBox( "File type is not supported. Please choose an other file.", ToolMessageBoxButtons_Ok, ToolMessageBoxIcon::Warning );
			return nullptr;
		}

		EditableFile* pFile = pEditor->openFile( fileName );
		if( pFile == nullptr )
		{
			return nullptr;
		}

		openEditable( *pFile );
		return pFile;
	}

	void Editor::openEditable( Editable& editable )
	{
		if( !m_editables.contains( &editable ) )
		{
			m_editables.pushBack( &editable );
		}

		if( &editable.getEditor() == m_pPackageEditor )
		{
			setPackagePath();
		}

		m_pCurrentEditable = &editable;
	}

	void Editor::saveEditable( Editable& editable )
	{
		if( !editable.isDirty() )
		{
			return;
		}

		if( !editable.getEditor().saveEditable( editable ) )
		{
			return;
		}
	}

	void Editor::closeEditable( Editable& editable )
	{
		if( editable.isDirty() )
		{
			EditableFile* pFile = editable.asFile();

			DynamicString fileName;
			if( pFile != nullptr )
			{
				fileName = pFile->getFileName().getFilenameWithExtension();
			}
			else
			{
				fileName = editable.getTitle();
			}

			const DynamicString text = "Do you want to save changes of '" + fileName + "'?";
			showMessageBox( text, ToolMessageBoxButtons_YesNoCancel, ToolMessageBoxIcon::Question, ToolMessageBoxCallbackDelegate( this, &Editor::saveOnCloseCallback ), UserData( &editable ) );
			return;
		}

		closeEditableInternal( editable );
	}

	void Editor::closeAll()
	{
		List< Editable* > editables = m_editables;
		for( Editable* pEditable : editables )
		{
			closeEditable( *pEditable );
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

	void Editor::showMessageBox( const DynamicString& message, ToolMessageBoxButtonFlagMask buttons /* = ToolMessageBoxButtons_Ok */, ToolMessageBoxIcon icon /* = ToolMessageBoxIcon::None */, ToolMessageBoxCallbackDelegate callback /* = ToolMessageBoxCallbackDelegate() */, UserData userData /* = UserData() */ )
	{
		DynamicString title = "TikiEditor";
		if( m_pPackageEditor->getPackage() != nullptr )
		{
			title += " - " + m_pPackageEditor->getPackage()->getName();
		}

		m_messageBox.open( title, message, buttons, icon, callback, userData );
	}

	EditorResourceResult Editor::getResource( const Resource** ppResource, const char* pFilename, fourcc resourceType )
	{
		const crc32 resourceNameCrc	= crcString( pFilename );
		const ResourceId resourceId	= { resourceNameCrc, resourceType };

		const ResourceMap::InsertResult insertResult = m_resources.insertKey( resourceId );
		EditorResource* pResource = insertResult.pValue;
		if( insertResult.isNew )
		{
			pResource->result		= EditorResourceResult_Busy;
			pResource->pResource	= nullptr;

			pResource->pRequest = &tool::getResourceManager().beginGenericResourceLoading( pFilename, resourceId.type, resourceId.nameCrc );
		}
		else if( pResource->pRequest != nullptr &&
				 !pResource->pRequest->isLoading() )
		{
			if( pResource->pRequest->isSuccessful() )
			{
				pResource->pResource	= pResource->pRequest->getResource< Resource >();
				pResource->result		= EditorResourceResult_Ok;
			}
			else
			{
				pResource->result		= EditorResourceResult_Error;
			}

			tool::getResourceManager().endResourceLoading( *pResource->pRequest );
			pResource->pRequest = nullptr;
		}

		if( pResource->result == EditorResourceResult_Ok )
		{
			TIKI_ASSERT( pResource->pResource != nullptr );
			*ppResource = pResource->pResource;
		}

		return pResource->result;
	}

	void Editor::saveOnCloseCallback( ToolMessageBoxButton button, UserData userData )
	{
		Editable& editable = *static_cast< Editable* >( userData.pContext );

		switch( button )
		{
		case ToolMessageBoxButton::Yes:
			saveEditable( editable );
			break;

		case ToolMessageBoxButton::Cancel:
			return;

		default:
			break;
		}

		closeEditableInternal( editable );
	}

	void Editor::closeEditableInternal( Editable& editable )
	{
		if( &editable == m_pCurrentEditable )
		{
			m_pCurrentEditable = nullptr;
		}

		if( m_pCurrentRibbon == &m_editableRibbon )
		{
			m_pCurrentRibbon = nullptr;
		}

		m_editables.removeSortedByValue( &editable );
		editable.getEditor().closeEditable( editable );
	}

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
		if( ImGui::Begin( "Ribbon", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav ) )
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

	void Editor::doFileBrowserUi( ImHorizontalSplitter& splitter )
	{
		if( ImGui::BeginHorizontalSplitterElement( splitter, "Browser", &m_fileBrowserWidth ) )
		{
			m_fileBrowserUi.doUi();
			ImGui::EndHorizontalSplitterElement( splitter );
		}
	}

	void Editor::doEditableUi( ImHorizontalSplitter& splitter )
	{
		if( ImGui::BeginHorizontalSplitterElement(splitter, "Editable", nullptr ) )
		{
			char titleBuffer[ 128u ];
			for( Editable* pEditable : m_editables )
			{
				if( m_pCurrentEditable == nullptr )
				{
					m_pCurrentEditable = pEditable;
				}

				bool wasSelected = false;
				if( m_pCurrentEditable == pEditable )
				{
					const ImGuiStyle& style = ImGui::GetStyle();
					ImGui::PushStyleColor( ImGuiCol_Button, style.Colors[ ImGuiCol_ButtonHovered ] );
					wasSelected = true;
				}

				formatStringBuffer( titleBuffer, sizeof( titleBuffer ), "%s%s", pEditable->getTitle().cStr(), pEditable->isDirty() ? "*" : "" );
				if( ImGui::Button( titleBuffer ) )
				{
					m_pCurrentEditable = pEditable;
				}

				if( wasSelected )
				{
					ImGui::PopStyleColor();
				}

				ImGui::SameLine();
			}
			ImGui::NewLine();

			if( ImGui::BeginChild( "Editor" ) )
			{
				if( m_pCurrentEditable != nullptr )
				{
					m_pCurrentEditable->doUi();
				}

			}
			ImGui::EndChild();
		}
		ImGui::EndHorizontalSplitterElement( splitter );
	}
}