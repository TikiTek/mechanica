#pragma once
#ifndef TIKI_PACKAGEFILEBROWSER_HPP_INCLUDED
#define TIKI_PACKAGEFILEBROWSER_HPP_INCLUDED

#include <QDockWidget>

class QTreeView;

namespace tiki
{
	class PackageFileBrowser : public QDockWidget
	{
		Q_OBJECT

	public:

		explicit		PackageFileBrowser();
		virtual			~PackageFileBrowser();

		void			openPackage( IEditorFile* pFile );
		void			closePakage();

	private:

		QTreeView*		m_pTreeView;

		IEditorFile*	m_pPackage;
	};
}

#endif // TIKI_PACKAGEFILEBROWSER_HPP_INCLUDED
