#pragma once

#include "tiki/editor_interface/editable_file.hpp"

#include "tiki/tool_generic_data/generic_data_document.hpp"

namespace tiki
{
	class GenericDataArray;
	class GenericDataEditor;

	class GenericDataFile : public EditableFile
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataFile );

	public:

									GenericDataFile( const Path& fileName, GenericDataEditor& genericDataEditor, GenericDataRenderer& renderer );
		virtual						~GenericDataFile();

		bool						load();
		bool						save();

		virtual void				update( float deltaTime ) TIKI_OVERRIDE_FINAL;
		virtual void				render( GraphicsContext& graphicsContext ) TIKI_OVERRIDE_FINAL;
		virtual void				doUi() TIKI_OVERRIDE_FINAL;

		virtual bool				processToolInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE_FINAL;

	private:

		GenericDataEditor&			m_genericDataEditor;
		GenericDataRenderer&		m_renderer;
		GenericDataRendererState	m_rendererState;
		AxisAlignedRectangle		m_rendererRectangle;
		bool						m_hasRendererFocus;

		GenericDataDocument			m_document;
		GenericDataObject*			m_pSelectedObject;

		ToolImage					m_tagsIcon;
		ToolImage					m_valueCreateIcon;
		ToolImage					m_valueRemoveIcon;

		void						doObjectUi( GenericDataObject* pObject, bool readOnly );
		void						doArrayUi( GenericDataArray* pArray, bool readOnly );
		GenericDataValue*			doElementUi( const string& name, GenericDataValue* pValue, const GenericDataType* pType, bool readOnly );
		void						doCreateObjectUi( GenericDataValue* pValue );
		void						doValueUi( GenericDataValue* pValue, bool readOnly );
		void						doEnumUi( GenericDataValue* pValue, bool readOnly );
		void						doValueTypeUi( GenericDataValue* pValue, bool readOnly );
		void						doValueTagUi( GenericDataTag* pTag, const GenericDataType* pTargetType, bool fixedTag, bool readOnly );
		void						doResourceUi( GenericDataValue* pValue, bool readOnly );

		DynamicString				getValuePreview( const GenericDataValue* pValue );
	};
}
