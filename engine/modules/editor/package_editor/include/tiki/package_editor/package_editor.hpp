#pragma once

#include "tiki/editor_interface/file_editor.hpp"

#include "tiki/package_editor/package_editor_ribbon.hpp"

namespace tiki
{
	class EditorInterface;
	class Package;
	class PackageFile;

	class PackageEditor : public FileEditor
	{
		friend class PackageEditorRibbon;
		TIKI_NONCOPYABLE_CLASS( PackageEditor );

	public:

								PackageEditor( EditorInterface* pInterface );
		virtual					~PackageEditor();

		virtual EditableFile*	openFile( const Path& fileName ) TIKI_OVERRIDE_FINAL;
		virtual bool			saveEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void			closeEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;

		virtual void			doUi() TIKI_OVERRIDE_FINAL;

		Package*				getPackage() const { return m_pPackage; }

	private:

		PackageEditorRibbon		m_ribbon;

		Package*				m_pPackage;
		PackageFile*			m_pPackageFile;

		bool					openPackage( Package& package );
		void					closePackage();
		void					editPackage();
	};
}
