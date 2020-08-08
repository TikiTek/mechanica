#include "tiki/editor_interface/editable_file.hpp"

namespace tiki
{
	EditableFile::EditableFile( const Path& filename, BaseEditor& editor )
		: Editable( filename.getBasename(), editor )
		, m_filename( filename )
	{
	}
}
