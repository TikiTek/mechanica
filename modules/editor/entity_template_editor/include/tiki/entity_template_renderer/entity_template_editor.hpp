#pragma once

#include "tiki/editor_interface/base_editor.hpp"

#include "tiki/entity_template_renderer/transform2d_component_view.hpp"

namespace tiki
{
	class GenericDataEditor;

	class EntitytemplateEditor : public BaseEditor
	{
	public:

									EntitytemplateEditor( GenericDataEditor* pGenericDataEditor );
		virtual						~EntitytemplateEditor();

	private:

		GenericDataEditor*			m_pGenericDataEditor;

		Transform2dComponentView	m_transformView;
	};
}