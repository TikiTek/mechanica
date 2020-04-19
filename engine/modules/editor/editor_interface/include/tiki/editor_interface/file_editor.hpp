#pragma once

#include "tiki/editor_interface/base_editor.hpp"

#include "tiki/tool_application/tool_image.hpp"

namespace tiki
{
	class EditableFile;

	class FileEditor : public BaseEditor
	{
	public:

		virtual EditableFile*		openFile( const Path& fileName ) TIKI_PURE;

		DynamicString				getFileTypeName() const { return m_fileTypeName; }
		DynamicString				getFileExtension() const { return m_fileExtension; }

		const ToolImage&			getEditableIcon() const { return m_fileIcon; };

		virtual FileEditor*			asFileEditor() TIKI_OVERRIDE_FINAL { return this; }

	protected:

									FileEditor( EditorInterface& editor, ConstMemoryBlock fileIconData, const DynamicString& fileTypeName, const DynamicString& fileExtension );
		virtual						~FileEditor() { }

	private:

		DynamicString				m_fileTypeName;
		DynamicString				m_fileExtension;
		ToolImage					m_fileIcon;
	};
}
