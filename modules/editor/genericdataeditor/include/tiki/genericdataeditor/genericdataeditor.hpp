#pragma once
#ifndef TIKI_GENERICDATAEDITOR_HPP_INCLUDED
#define TIKI_GENERICDATAEDITOR_HPP_INCLUDED

#include "tiki/editorinterface/ifileeditor.hpp"

#include "tiki/toolgenericdata/generic_data_type_collection.hpp"

#include <QIcon>

namespace tiki
{
	class IEditorInterface;

	class GenericDataEditor : public QObject, public IFileEditor
	{
		Q_OBJECT
		TIKI_NONCOPYABLE_CLASS( GenericDataEditor );

	public:

												GenericDataEditor( IEditorInterface* pInterface );
		virtual									~GenericDataEditor();

		virtual QWidget*						openFile( IFile* pFile ) TIKI_OVERRIDE_FINAL;
		virtual bool							saveFile( IFile* pFile ) TIKI_OVERRIDE_FINAL;
		virtual void							closeFile( IFile* pFile ) TIKI_OVERRIDE_FINAL;

		virtual QString							getFileTypeName() const TIKI_OVERRIDE_FINAL;
		virtual QString							getFileExtension() const TIKI_OVERRIDE_FINAL;
		virtual QIcon							getFileIcon() const TIKI_OVERRIDE_FINAL;

		virtual const QVector< QtRibbonTab* >&	getRibbonTabs() const TIKI_OVERRIDE_FINAL;
		virtual const QVector< QDockWidget* >&	getDockWidgets() const TIKI_OVERRIDE_FINAL;

	private:

		IEditorInterface*			m_pInterface;

		QIcon						m_icon;

		GenericDataTypeCollection	m_typeCollection;

		QVector< QtRibbonTab* >		m_tabs;
		QVector< QDockWidget* >		m_docks;
	};
}

#endif // TIKI_GENERICDATAEDITOR_HPP_INCLUDED
