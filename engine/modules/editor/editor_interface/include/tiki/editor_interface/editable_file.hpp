#pragma once

#include "tiki/editor_interface/editable.hpp"

#include "tiki/base/path.hpp"

namespace tiki
{
	class FileEditor;

	class EditableFile : public Editable
	{
	public:

		const Path&					getFileName() const { return m_filename; }

		virtual EditableFile*		asFile() TIKI_OVERRIDE_FINAL { return this; }

	protected:

									EditableFile( const Path& filename, BaseEditor& editor );

	private:

		Path						m_filename;
	};
}
