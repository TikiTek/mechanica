#pragma once

#include "tiki/editor_interface/editable_file.hpp"

#include "tiki/tool_generic_data/generic_data_document.hpp"

namespace tiki
{
	class GenericDataEditor;
	class GenericDataIterator;

	class GenericDataFile : public EditableFile
	{
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

		void						doContainerUi( GenericDataIterator& iterator );
		void						doElementUi( GenericDataIterator& iterator, const DynamicString& name );
		void						doEnumUi( GenericDataIterator& iterator, const GenericDataValue* pValue, const GenericDataType* pType );
		void						doValueUi( GenericDataIterator& iterator, const GenericDataValue* pValue, const GenericDataType* pType );
		void						doValueTagUi( GenericDataIterator& iterator, const GenericDataTag* pTag, const GenericDataType* pTargetType, bool fixedTag );
		void						doPointerUi( GenericDataIterator& iterator, const GenericDataValue* pValue, const GenericDataType* pType );
		void						doReferenceUi( GenericDataIterator& iterator, const GenericDataValue* pValue, const GenericDataType* pType );

		DynamicString				getValuePreview( const GenericDataValue* pValue );
	};
}
