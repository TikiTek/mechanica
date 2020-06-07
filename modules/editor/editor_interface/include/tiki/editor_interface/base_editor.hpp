#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"
#include "tiki/editor_interface/editor_ribbon.hpp"

namespace tiki
{
	class Editable;
	class EditorInterface;
	class FileEditor;
	class GraphicsContext;

	class BaseEditor
	{
	public:

		virtual Editable*				openEditable( const DynamicString& title ) { return nullptr; }
		virtual bool					saveEditable( Editable& editable ) { return false; }
		virtual void					closeEditable( Editable& editable ) { }

		EditorInterface&				getInterface() { return m_editor; }
		const List< EditorRibbon* >&	getEditableRibbons() const { return m_editableTabs; }

		virtual void					doUi() { }

		virtual FileEditor*				asFileEditor() { return nullptr; }

	protected:

		EditorInterface&				m_editor;

		List< EditorRibbon* >			m_editableTabs;

										BaseEditor( EditorInterface& editor );
		virtual							~BaseEditor() { }
	};
}
