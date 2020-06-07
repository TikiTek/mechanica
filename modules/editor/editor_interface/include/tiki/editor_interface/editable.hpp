#pragma once
#ifndef TIKI_IEDITABLE_HPP_INCLUDED
#define TIKI_IEDITABLE_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class BaseEditor;
	class EditableFile;
	struct InputEvent;

	class Editable : NonCopyable
	{
	public:

		virtual							~Editable() { }

		const DynamicString&			getTitle() const { return m_title; }
		BaseEditor&						getEditor() const { return m_editor; }
		bool							isDirty() const { return m_isDirty; }

		virtual void					update( float deltaTime ) { }
		virtual void					render( GraphicsContext& graphicsContext ) { }
		virtual void					doUi() = 0;

		virtual bool					processToolInputEvent( const InputEvent& inputEvent ) { return false; }

		virtual EditableFile*			asFile() { return nullptr; }

	protected:

		bool							m_isDirty;

										Editable( const DynamicString& title, BaseEditor& editor );

	private:

		DynamicString					m_title;
		BaseEditor&						m_editor;
	};
}

#endif // TIKI_IEDITABLE_HPP_INCLUDED
