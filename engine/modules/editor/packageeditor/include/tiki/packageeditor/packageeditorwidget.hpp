#pragma once
#ifndef TIKI_PACKAGEEDITORWIDGET_HPP_INCLUDED
#define TIKI_PACKAGEEDITORWIDGET_HPP_INCLUDED

#include <QWidget>

namespace tiki
{
	class PackageEditorWidget : public QWidget
	{
		Q_OBJECT

	public:

		explicit		PackageEditorWidget();
		virtual			~PackageEditorWidget();

		bool			openPackage( IEditorFile* pFile );
		void			savePackage();

	private:

		IEditorFile*	m_pFile;
	};
}

#endif // TIKI_PACKAGEEDITORWIDGET_HPP_INCLUDED
