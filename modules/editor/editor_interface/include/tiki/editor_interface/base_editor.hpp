#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"
#include "tiki/editor_interface/editor_ribbon.hpp"
#include "tiki/tool_application/tool_image.hpp"

namespace tiki
{
	class Editable;
	class EditorInterface;
	class FileEditor;

	class BaseEditor
	{
	public:

		virtual Editable*				openEditable( const DynamicString& title ) { return nullptr; }
		virtual bool					saveEditable( Editable* pEditable ) { return false; }
		virtual void					closeEditable( Editable* pEditable ) { }

		virtual void					update( float deltaTime ) { }
		virtual void					render( GraphicsContext& graphicsContext ) { }

		const ToolImage&				getEditableIcon() const { return m_icon; };
		const List< EditorRibbon* >&	getEditableRibbons() const { return m_editableTabs; }

		virtual void					doUi() { }

		virtual FileEditor*				asFileEditor() { return nullptr; }

	protected:

		EditorInterface*				m_pInterface;

		ToolImage						m_icon;

		List< EditorRibbon* >			m_editableTabs;

										BaseEditor( EditorInterface* pInterface, ConstMemoryBlock iconData );
		virtual							~BaseEditor() { }
	};
}
