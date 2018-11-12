#pragma once

#include "tiki/tool_application/tool_image.hpp"

namespace tiki
{
	class Editor;

	class EditorFileBrowserUi
	{
		TIKI_NONCOPYABLE_CLASS( EditorFileBrowserUi );

	public:

		explicit					EditorFileBrowserUi( Editor& editor );
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

		Editor&						m_editor;
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
