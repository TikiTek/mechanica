#pragma once

#include "tiki/editor_interface/editable_file.hpp"

#include "tiki/toolgenericdata/generic_data_document.hpp"

namespace tiki
{
	class GenericDataArray;
	class GenericDataEditor;

	class GenericDataFile : public EditableFile
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataFile );

	public:

								GenericDataFile( const Path& fileName, GenericDataEditor& genericDataEditor );
		virtual					~GenericDataFile();

		bool					load();
		bool					save();

		virtual void			doUi() TIKI_OVERRIDE_FINAL;

	private:

		GenericDataEditor&		m_genericDataEditor;

		GenericDataDocument		m_document;
		GenericDataObject*		m_pSelectedObject;

		ToolImage				m_tagsIcon;
		ToolImage				m_valueCreateIcon;
		ToolImage				m_valueRemoveIcon;

		void					doObjectUi( GenericDataObject* pObject, bool readOnly );
		void					doArrayUi( GenericDataArray* pArray, bool readOnly );
		GenericDataValue*		doElementUi( const string& name, GenericDataValue* pValue, const GenericDataType* pType, bool readOnly );
		void					doValueUi( GenericDataValue* pValue, bool readOnly );
		void					doEnumUi( GenericDataValue* pValue, bool readOnly );
		void					doValueTypeUi( GenericDataValue* pValue, bool readOnly );
		void					doValueTagUi( GenericDataTag* pTag, const GenericDataType* pTargetType, bool fixedTag, bool readOnly );
		void					doResourceUi( GenericDataValue* pValue, bool readOnly );
	};
}
