#pragma once

#include "tiki/converter_editor/converter_editor_ribbon.hpp"
#include "tiki/editor_interface/base_editor.hpp"
#include "tiki/tool_application/tool_image.hpp"

namespace tiki
{
	class AssetConverterInterface;
	class EditorInterface;

	class ConverterEditor : public BaseEditor
	{
		TIKI_NONCOPYABLE_CLASS( ConverterEditor );

	public:

									ConverterEditor( EditorInterface& editor, AssetConverterInterface* pAssetConverter );
		virtual						~ConverterEditor();

	private:

		AssetConverterInterface*	m_pConverter;

		ConverterEditorRibbon		m_ribbon;

		void						convertAll();
		void						showConverter();
		void						showLastResult();
	};
}
