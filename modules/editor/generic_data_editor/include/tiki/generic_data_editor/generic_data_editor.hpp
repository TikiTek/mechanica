#pragma once

#include "tiki/editor_interface/file_editor.hpp"

#include "tiki/tool_generic_data/generic_data_type_collection.hpp"

#include "../../../source/generic_data_renderer.hpp"

namespace tiki
{
	class GenericDataEditor : public FileEditor
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataEditor );

	public:

									GenericDataEditor( EditorInterface* pInterface, ResourceManager& resourceManager, GraphicsSystem& graphicsSystem );
		virtual						~GenericDataEditor();

		virtual EditableFile*		openFile( const Path& fileName ) TIKI_OVERRIDE_FINAL;
		virtual bool				saveEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void				closeEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;

		virtual void				update( float deltaTime ) TIKI_OVERRIDE_FINAL;
		virtual void				render( GraphicsContext& graphicsContext ) TIKI_OVERRIDE_FINAL;

		void						registerView( const GenericDataTypeStruct* pType, GenericDataView* pView );
		void						unregisterView( const GenericDataTypeStruct* pType );

		GenericDataTypeCollection&	getTypeCollection() { return m_typeCollection; }

	private:

		GenericDataTypeCollection	m_typeCollection;

		GenericDataRenderer			m_renderer;
	};
}
