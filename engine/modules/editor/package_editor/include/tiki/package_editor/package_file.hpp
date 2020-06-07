#pragma once

#include "tiki/editor_interface/editable_file.hpp"

#include <tinyxml2.h>

namespace tiki
{
	class PackageFile : public EditableFile
	{
		TIKI_NONCOPYABLE_CLASS( PackageFile );

	public:

		explicit				PackageFile( Package& package, PackageEditor& editor );
		virtual					~PackageFile();

		bool					load();
		bool					save();

		virtual void			doUi() TIKI_OVERRIDE_FINAL;

	private:

		Package&				m_package;

		char					m_desciption[ 128u ];
		char					m_author[ 128u ];
		char					m_genericData[ 128u ];
		char					m_genericTypes[ 128u ];
		char					m_assetTemplates[ 128u ];
	};
}
