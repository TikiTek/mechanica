#pragma once

#include "tiki/editor_interface/base_editor.hpp"

#include "tiki/entity_template_editor/entity_template_view.hpp"
#include "tiki/entity_template_editor/transform2d_component_view.hpp"
#include "tiki/entity_template_editor/sprite_component_view.hpp"

namespace tiki
{
	class GenericDataEditor;

	class EntityTemplateEditor : public BaseEditor
	{
	public:

									EntityTemplateEditor( EditorInterface& editor, GenericDataEditor& genericDataEditor );
		virtual						~EntityTemplateEditor();

	private:

		GenericDataEditor&			m_genericDataEditor;

		EntityTemplateView			m_entityTemplateVuew;
		Transform2dComponentView	m_transformView;
		SpriteComponentView			m_spriteView;
	};
}