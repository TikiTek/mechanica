#pragma once

#include "tiki/editor_interface/file_editor.hpp"

#include "tiki/tool_generic_data/generic_data_document_collection.hpp"
#include "tiki/tool_generic_data/generic_data_type_collection.hpp"

#include "../../../source/generic_data_renderer.hpp"

namespace tiki
{
	class GenericDataEditor : public FileEditor
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataEditor );

	public:

										GenericDataEditor( EditorInterface& editor, ResourceManager& resourceManager, GraphicsSystem& graphicsSystem );
		virtual							~GenericDataEditor();

		virtual EditableFile*			openFile( const Path& fileName ) TIKI_OVERRIDE_FINAL;
		virtual bool					saveEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void					closeEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;

		void							update();

		void							registerView( GenericDataView& view );
		void							unregisterView( GenericDataView& view );

		GenericDataTypeCollection&		getTypeCollection() { return m_typeCollection; }
		GenericDataDocumentCollection&	getDocumentCollection() { return m_documentCollection; }

	private:

		GenericDataTypeCollection		m_typeCollection;
		GenericDataDocumentCollection	m_documentCollection;

		GenericDataRenderer				m_renderer;
	};
}
