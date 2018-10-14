#pragma once

#include "tiki/editor_interface/base_editor.hpp"
#include "tiki/toolapplication/tool_image.hpp"

namespace tiki
{
	class AssetConverterInterface;
	class EditorInterface;

	class ConverterEditor : public BaseEditor
	{
		TIKI_NONCOPYABLE_CLASS( ConverterEditor );

	public:

									ConverterEditor( EditorInterface* pInterface, AssetConverterInterface* pAssetConverter );
		virtual						~ConverterEditor();

		virtual bool				saveEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void				closeEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;

	private:

		AssetConverterInterface*	m_pConverter;

		void						convertAll();
		void						showConverter();
		void						showLastResult();
	};
}
