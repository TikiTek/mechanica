#pragma once

#include "tiki/editor_interface/editable_file.hpp"

#include <tinyxml2.h>

namespace tiki
{
	class PackageFile : public EditableFile
	{
		TIKI_NONCOPYABLE_CLASS( PackageFile );

	public:

		explicit				PackageFile( const Path& filename, PackageEditor* pEditor );
		virtual					~PackageFile();

		bool					load();
		bool					save();

		virtual void			doUi() TIKI_OVERRIDE_FINAL;

	private:

		tinyxml2::XMLDocument	m_document;

		tinyxml2::XMLElement*	findOrCreateElement( const char* pName );
	};
}
