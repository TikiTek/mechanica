#pragma once
#ifndef TIKI_IEDITABLE_HPP_INCLUDED
#define TIKI_IEDITABLE_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class IEditor;
	class IFile;

	class IEditable
	{
	public:

		virtual					~IEditable() { }

		virtual const QString&	getTitle() const TIKI_PURE;
		virtual IEditor*		getEditor() const TIKI_PURE;
		virtual QWidget*		getEditWidget() const TIKI_PURE;

		virtual bool			isDirty() const TIKI_PURE;
		virtual void			markAsDirty() TIKI_PURE;

		virtual IFile*			asFile() { return nullptr; }
	};
}

#endif // TIKI_IEDITABLE_HPP_INCLUDED
