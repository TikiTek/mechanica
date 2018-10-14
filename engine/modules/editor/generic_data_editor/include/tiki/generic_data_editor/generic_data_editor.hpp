#pragma once

#include "tiki/editor_interface/file_editor.hpp"

#include "tiki/toolgenericdata/generic_data_type_collection.hpp"

namespace tiki
{
	class GenericDataEditor : public FileEditor
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataEditor );

	public:

												GenericDataEditor( EditorInterface* pInterface );
		virtual									~GenericDataEditor();

		virtual EditableFile*					openFile( const Path& fileName ) TIKI_OVERRIDE_FINAL;
		virtual bool							saveEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void							closeEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;

	private:

		GenericDataTypeCollection	m_typeCollection;
	};
}
