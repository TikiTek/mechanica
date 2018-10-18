#include "tiki/editor_interface/file_editor.hpp"

namespace tiki
{
	FileEditor::FileEditor( EditorInterface* pInterface, ConstMemoryBlock iconData, const DynamicString& fileTypeName, const DynamicString& fileExtension )
		: BaseEditor( pInterface, iconData )
		, m_fileTypeName( fileTypeName )
		, m_fileExtension( fileExtension )
	{
	}
}