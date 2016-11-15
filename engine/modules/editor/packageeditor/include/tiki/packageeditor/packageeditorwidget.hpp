#pragma once
#ifndef TIKI_PACKAGEEDITORWIDGET_HPP_INCLUDED
#define TIKI_PACKAGEEDITORWIDGET_HPP_INCLUDED

#include <QWidget>
#include <tinyxml2.h>

class QTextEdit;

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

		IEditorFile*			m_pFile;

		tinyxml2::XMLDocument	m_document;

		QTextEdit*				m_pDescriptionText;
	};
}

#endif // TIKI_PACKAGEEDITORWIDGET_HPP_INCLUDED
