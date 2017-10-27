#pragma once
#ifndef TIKI_IFILE_HPP_INCLUDED
#define TIKI_IFILE_HPP_INCLUDED

#include "tiki/editor_interface/ieditable.hpp"

namespace tiki
{
	class IFileEditor;

	class IFile : public IEditable

	{
	public:

		virtual						~IFile() { }

		virtual const QString&		getFileName() const TIKI_PURE;

		virtual IFile*				asFile() TIKI_OVERRIDE_FINAL { return this; }
	};
}

#endif // TIKI_IFILE_HPP_INCLUDED
