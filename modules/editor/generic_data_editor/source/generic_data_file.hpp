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

								GenericDataFile( const Path& fileName, GenericDataEditor* pEditor );
		virtual					~GenericDataFile();

		bool					load();
		bool					save();

		virtual void			doUi() TIKI_OVERRIDE_FINAL;

	private:

		GenericDataDocument		m_document;

		void					doObjectUi( GenericDataObject* pObject );
		void					doArrayUi( GenericDataArray* pArray );
		void					doElementUi( const string& name, GenericDataValue* pValue );
		void					doValueUi( GenericDataValue* pValue );
		void					doEnumUi( GenericDataValue* pValue );
		void					doValueTypeUi( GenericDataValue* pValue );
		void					doResourceUi( GenericDataValue* pValue );
	};
}
