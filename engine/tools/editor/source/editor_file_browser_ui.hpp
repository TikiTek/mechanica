#pragma once

#include "tiki/toolapplication/tool_image.hpp"

namespace tiki
{
	class EditorInterface;

	class EditorFileBrowserUi
	{
		TIKI_NONCOPYABLE_CLASS( EditorFileBrowserUi );

	public:

		explicit					EditorFileBrowserUi( EditorInterface* pInterface );
		virtual						~EditorFileBrowserUi();

		void						create();

		void						openPackage( const Package& package );
		void						closePakage();

		void						doUi();

	private:

		struct FileTreeNode
		{
			FileTreeNode*			pParentNode;
			FileTreeNode*			pNextSibling;
			FileTreeNode*			pFirstChild;

			const ToolImage*		pIcon;
			Path					filename;
		};

		EditorInterface*			m_pInterface;
		const Package*				m_pPackage;

		ChunkedPool< FileTreeNode >	m_nodes;
		FileTreeNode*				m_pFirstNode;

		ToolImage					m_packageIcon;
		ToolImage					m_folderIcon;
		ToolImage					m_unknownIcon;

		FileTreeNode*				addFiles( FileTreeNode* pParentNode, const Path& path );

		void						doNodeUi( const FileTreeNode* pNode );
	};
}
