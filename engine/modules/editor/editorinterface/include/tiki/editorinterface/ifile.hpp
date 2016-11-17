#pragma once
#ifndef TIKI_IFILE_HPP_INCLUDED
#define TIKI_IFILE_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class IFile
	{
	public:

		virtual						~IFile() { }

		virtual const QString&		getFileName() const TIKI_PURE;
		virtual const IFileEditor*	getFileEditor() const TIKI_PURE;
		virtual QWidget*			getEditWidget() const TIKI_PURE;

		virtual bool				isDirty() const TIKI_PURE;
		virtual void				markAsDirty() TIKI_PURE;
	};
}

#endif // TIKI_IFILE_HPP_INCLUDED
