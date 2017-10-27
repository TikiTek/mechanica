#pragma once
#ifndef TIKI_GENERIC_DATA_EDITOR_HPP_INCLUDED
#define TIKI_GENERIC_DATA_EDITOR_HPP_INCLUDED

#include "tiki/editor_interface/ifileeditor.hpp"

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
		virtual bool							saveEditable( IEditable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void							closeEditable( IEditable* pEditable ) TIKI_OVERRIDE_FINAL;

		virtual QString							getFileTypeName() const TIKI_OVERRIDE_FINAL;
		virtual QString							getFileExtension() const TIKI_OVERRIDE_FINAL;
		virtual QIcon							getEditableIcon() const TIKI_OVERRIDE_FINAL;

	private:

		IEditorInterface*			m_pInterface;

		QIcon						m_icon;

		GenericDataTypeCollection	m_typeCollection;
	};
}

#endif // TIKI_GENERIC_DATA_EDITOR_HPP_INCLUDED
