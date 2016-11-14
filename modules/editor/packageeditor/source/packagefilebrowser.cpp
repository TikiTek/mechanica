#include "tiki/packageeditor/packagefilebrowser.hpp"

#include <QTreeView>

namespace tiki
{
	PackageFileBrowser::PackageFileBrowser()
		: QDockWidget( "Asset Browser" )
		, m_pPackage( nullptr )
	{
		m_pTreeView = new QTreeView();

		setWidget( m_pTreeView );
	}

	PackageFileBrowser::~PackageFileBrowser()
	{
		delete m_pTreeView;
		m_pTreeView = nullptr;
	}

	void PackageFileBrowser::openPackage( IEditorFile* pFile )
	{
		closePakage();
		m_pPackage = pFile;

		// add all files
	}

	void PackageFileBrowser::closePakage()
	{
		if( m_pPackage == nullptr )
		{
			return;
		}

		foreach( QObject* pObject, m_pTreeView->children() )
		{
			delete pObject;
		}
	}
}
