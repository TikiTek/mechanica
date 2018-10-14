#pragma once

namespace tiki
{
	class EditorInterface;

	class EditorFileBrowserUi
	{
		TIKI_NONCOPYABLE_CLASS( EditorFileBrowserUi );

	public:

		explicit			EditorFileBrowserUi( EditorInterface* pInterface );
		virtual				~EditorFileBrowserUi();

		void				openPackage( const DynamicString& packageName );
		void				closePakage();

	private:

		EditorInterface*	m_pInterface;
		DynamicString		m_packageName;

		ToolImage			m_folderIcon;
		ToolImage			m_unknownIcon;

		//void				addFiles( QStandardItem* pParentItem, QDir dir );
	};
}
