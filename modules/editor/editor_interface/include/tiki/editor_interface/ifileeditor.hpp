#pragma once
#ifndef TIKI_IFILEEDITOR_HPP_INCLUDED
#define TIKI_IFILEEDITOR_HPP_INCLUDED

#include "tiki/editor_interface/ieditor.hpp"

#include <QString>

class QWidget;

namespace tiki
{
	class IFile;

	class IFileEditor : public IEditor
	{
	public:

		virtual QWidget*			openFile( IFile* pFile ) TIKI_PURE;

		virtual QString				getFileTypeName() const TIKI_PURE;
		virtual QString				getFileExtension() const TIKI_PURE;

		virtual IFileEditor*		asFileEditor() TIKI_OVERRIDE_FINAL { return this; }

	protected:

		virtual						~IFileEditor() { }
	};
}

#endif // TIKI_IFILEEDITOR_HPP_INCLUDED
