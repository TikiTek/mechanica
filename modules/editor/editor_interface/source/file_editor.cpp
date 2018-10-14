#include "tiki/editor_interface/file_editor.hpp"

namespace tiki
{
	FileEditor::FileEditor( EditorInterface* pInterface, const char* pIconPath, const DynamicString& fileTypeName, const DynamicString& fileExtension )
		: BaseEditor( pInterface, pIconPath )
		, m_fileTypeName( fileTypeName )
		, m_fileExtension( fileExtension )
	{
	}
}