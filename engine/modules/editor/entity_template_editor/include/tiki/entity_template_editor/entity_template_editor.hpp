#pragma once

#include "tiki/editor_interface/base_editor.hpp"

#include "tiki/entity_template_editor/transform2d_component_view.hpp"

namespace tiki
{
	class GenericDataEditor;

	class EntitytemplateEditor : public BaseEditor
	{
	public:

									EntitytemplateEditor( EditorInterface& editor, GenericDataEditor& genericDataEditor );
		virtual						~EntitytemplateEditor();

	private:

		GenericDataEditor&			m_genericDataEditor;

		Transform2dComponentView	m_transformView;
	};
}