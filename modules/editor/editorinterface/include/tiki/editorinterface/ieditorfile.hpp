#pragma once
#ifndef TIKI_IEDITORFILE_HPP_INCLUDED
#define TIKI_IEDITORFILE_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class IEditorFile
	{
	public:

		virtual						~IEditorFile() { }

		virtual const QString&		getFileName() const TIKI_PURE;
		virtual const IFileEditor*	getFileEditor() const TIKI_PURE;
		virtual QWidget*			getEditWidget() const TIKI_PURE;

		virtual bool				isDirty() const TIKI_PURE;
		virtual void				markAsDirty() TIKI_PURE;
	};
}

#endif // TIKI_EDITORFILE_HPP_INCLUDED
