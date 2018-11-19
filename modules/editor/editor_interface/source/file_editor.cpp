#include "tiki/editor_interface/file_editor.hpp"

namespace tiki
{
	FileEditor::FileEditor( EditorInterface& editor, ConstMemoryBlock fileIconData, const DynamicString& fileTypeName, const DynamicString& fileExtension )
		: BaseEditor( editor )
		, m_fileTypeName( fileTypeName )
		, m_fileExtension( fileExtension )
	{
		TIKI_VERIFY( m_fileIcon.createFromMemory( fileIconData ) );
	}
}