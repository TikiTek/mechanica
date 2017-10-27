#pragma once
#ifndef TIKI_PACKAGE_EDITOR_WIDGET_HPP_INCLUDED
#define TIKI_PACKAGE_EDITOR_WIDGET_HPP_INCLUDED

#include <QWidget>
#include <tinyxml2.h>

class QLabel;
class QLineEdit;
class QListView;
class QTextEdit;
class QVBoxLayout;

namespace tiki
{
	class IFile;

	class PackageEditorWidget : public QWidget
	{
		Q_OBJECT

	public:

		explicit		PackageEditorWidget();
		virtual			~PackageEditorWidget();

		bool			openPackage( IFile* pFile );
		void			savePackage();

	private slots:

		void			textChanged();

	private:

		IFile*					m_pFile;

		bool					m_isLoading;
		tinyxml2::XMLDocument	m_document;

		QVBoxLayout*			m_pLayout;

		QLabel*					m_pAuthorLabel;
		QLineEdit*				m_pAuthorText;

		QLabel*					m_pDescriptionLabel;
		QTextEdit*				m_pDescriptionText;

		QLabel*					m_pDependenciesLabel;
		QListView*				m_pDependenciesList;

		QLabel*					m_pTemplatesLabel;
		QLineEdit*				m_pTemplatesText;

		QLabel*					m_pGenericDataTypesLabel;
		QLineEdit*				m_pGenericDataTypesText;

		tinyxml2::XMLElement*	findOrCreateElement( const char* pName );
	};
}

#endif // TIKI_PACKAGE_EDITOR_WIDGET_HPP_INCLUDED
