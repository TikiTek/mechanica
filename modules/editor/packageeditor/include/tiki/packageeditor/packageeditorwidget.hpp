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

					PackageEditorWidget();
					~PackageEditorWidget();

		bool		openPackage( const QString& fileName );
		void		savePackage();

	private:

		QString		m_fileName;
	};
}

#endif // TIKI_PACKAGEEDITORWIDGET_HPP_INCLUDED
