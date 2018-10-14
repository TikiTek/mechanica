#pragma once

#include "tiki/editor_interface/base_editor.hpp"

namespace tiki
{
	class EditableFile;

	class FileEditor : public BaseEditor
	{
	public:

		virtual EditableFile*		openFile( const Path& fileName ) TIKI_PURE;

		string						getFileTypeName() const { return m_fileTypeName; }
		string						getFileExtension() const { return m_fileExtension; }

		virtual FileEditor*			asFileEditor() TIKI_OVERRIDE_FINAL { return this; }

	protected:

									FileEditor( EditorInterface* pInterface, const char* pIconPath, const DynamicString& fileTypeName, const DynamicString& fileExtension );
		virtual						~FileEditor() { }

	private:

		DynamicString				m_fileTypeName;
		DynamicString				m_fileExtension;
	};
}
