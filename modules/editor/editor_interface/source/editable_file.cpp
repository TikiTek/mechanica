#include "tiki/editor_interface/editable_file.hpp"

namespace tiki
{
	EditableFile::EditableFile( const Path& filename, BaseEditor* pEditor )
		: Editable( filename.getBasename(), pEditor )
		, m_filename( filename )
	{
	}
}
