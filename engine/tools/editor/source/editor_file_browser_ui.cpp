#include "editor_file_browser_ui.hpp"

#include "tiki/io/directoryiterator.hpp"

#include "res_editor.hpp"

namespace tiki
{
	EditorFileBrowserUi::EditorFileBrowserUi( EditorInterface* pInterface )
		: m_pInterface( pInterface )
		, m_pPackage( nullptr )
	{
	}

	EditorFileBrowserUi::~EditorFileBrowserUi()
	{
		closePakage();
		m_nodes.dispose();
	}

	void EditorFileBrowserUi::create()
	{
		m_packageIcon.createFromMemory( getEditorResource( EditorResources_BrowserPackage ) );
		m_folderIcon.createFromMemory( getEditorResource( EditorResources_BrowserFolder ) );
		m_unknownIcon.createFromMemory( getEditorResource( EditorResources_BrowserFileUnknown ) );

		m_nodes.create( 128u );
	}

	void EditorFileBrowserUi::openPackage( const Package& package )
	{
		if( &package == m_pPackage )
		{
			return;
		}

		closePakage();
		m_pPackage = &package;

		m_pFirstNode = addFiles( nullptr, package.getBasepath() );
	}

	void EditorFileBrowserUi::closePakage()
	{
		FileTreeNode* pNode = m_pFirstNode;
		while( pNode != nullptr )
		{
			if( pNode->pFirstChild != nullptr )
			{
				FileTreeNode* pNextNode = pNode->pFirstChild;
				pNode->pFirstChild = nullptr;
				pNode = pNextNode;
			}
			else if( pNode->pNextSibling != nullptr )
			{
				FileTreeNode* pNextNode = pNode->pNextSibling;
				m_nodes.removeUnsortedByValue( *pNode );
				pNode = pNextNode;
			}
			else
			{
				FileTreeNode* pNextNode = pNode->pParentNode;
				m_nodes.removeUnsortedByValue( *pNode );
				pNode = pNextNode;
			}
		}
		TIKI_ASSERT( m_nodes.isEmpty() );

		m_pPackage = nullptr;
	}

	void EditorFileBrowserUi::doUi()
	{
		doNodeUi( m_pFirstNode );
	}

	EditorFileBrowserUi::FileTreeNode* EditorFileBrowserUi::addFiles( FileTreeNode* pParentNode, const Path& path )
	{
		FileTreeNode* pDirectoryNode = m_nodes.push();
		pDirectoryNode->pParentNode		= pParentNode;
		pDirectoryNode->pNextSibling	= nullptr;
		pDirectoryNode->pFirstChild		= nullptr;
		pDirectoryNode->pIcon			= pParentNode == nullptr ? &m_packageIcon : &m_folderIcon;
		pDirectoryNode->filename		= path;

		DirectoryIterator directoryIterator;
		TIKI_VERIFY( directoryIterator.create( path ) );

		FileTreeNode* pPreviousFileNode = nullptr;
		while( directoryIterator.findNextFile() )
		{
			const char* pName = directoryIterator.getCurrentFileName();
			if( pName[ 0u ] == '.' )
			{
				continue;
			}

			Path childPath = path;
			childPath.push( pName );

			if( directoryIterator.isCurrentDirectory() )
			{
				FileTreeNode* pChildDirectoryNode = addFiles( pDirectoryNode, childPath );
				pChildDirectoryNode->pNextSibling = pPreviousFileNode;
				pPreviousFileNode = pChildDirectoryNode;
				continue;
			}

			FileTreeNode* pFileNode = m_nodes.push();
			pFileNode->pParentNode	= pDirectoryNode;
			pFileNode->pNextSibling	= pPreviousFileNode;
			pFileNode->pFirstChild	= nullptr;
			pFileNode->pIcon		= &m_unknownIcon;
			pFileNode->filename		= childPath;

			FileEditor* pEditor = m_pInterface->findEditorForFile( childPath );
			if( pEditor != nullptr )
			{
				pFileNode->pIcon = &pEditor->getEditableIcon();
			}

			pPreviousFileNode = pFileNode;
		}

		if( pDirectoryNode->pFirstChild == nullptr )
		{
			pDirectoryNode->pFirstChild = pPreviousFileNode;
		}

		return pDirectoryNode;
	}

	void EditorFileBrowserUi::doNodeUi( const FileTreeNode* pNode )
	{
		while( pNode != nullptr )
		{
			const char* pName = pNode->filename.getFilenameWithExtension();
			ImGui::AlignTextToFramePadding();
			if( pNode->pFirstChild == nullptr )
			{
				if( ImGui::TreeNodeEx( pName, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_OpenOnDoubleClick ) )
				{
					m_pInterface->openFile( pNode->filename );
				}
			}
			else if( ImGui::TreeNode( pName ) )
			{
				doNodeUi( pNode->pFirstChild );
				ImGui::TreePop();
			}

			pNode = pNode->pNextSibling;
		}

		//ImGui::PushID( 1 );
		//ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
		//ImGui::TreeNodeEx( "##hidden1", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet );
		//ImGui::SameLine();
		//ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
		//ImGui::Image( ImGui::Tex( m_unknownIcon ), ImVec2( m_unknownIcon.getData().getWidth(), m_unknownIcon.getData().getHeight() ) );
		//ImGui::SameLine();
		//ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
		//ImGui::Text( "Objects1" );
		//ImGui::PopID();

		//ImGui::PushID( 2 );
		//ImGui::AlignTextToFramePadding();
		//if( ImGui::TreeNode( "Object2" ) )
		//{
		//	ImGui::PushID( 21 );
		//	ImGui::AlignTextToFramePadding();
		//	ImGui::TreeNodeEx( "Object21", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet );
		//	ImGui::PopID();

		//	ImGui::PushID( 22 );
		//	ImGui::AlignTextToFramePadding();
		//	ImGui::TreeNodeEx( "Object22", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet );
		//	ImGui::PopID();

		//	ImGui::TreePop();
		//}

		//ImGui::PopID();
	}
}
