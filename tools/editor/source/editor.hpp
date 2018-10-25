#pragma once

#include "tiki/editor_interface/editor_interface.hpp"

#include "tiki/toolapplication/tool_message_box.hpp"
#include "tiki/toolproject/project.hpp"

#include "editor_file_browser_ui.hpp"

namespace tiki
{
	class AssetConverterInterface;
	class ConverterEditor;
	class GenericDataEditor;
	class PackageEditor;
	class PacketManager;
	class EditorRibbon;

	class Editor : public EditorInterface
	{
		TIKI_NONCOPYABLE_CLASS( Editor );

	public:

										Editor();
		virtual							~Editor();

		bool							create();
		void							dispose();

		void							doUi();

		virtual EditableFile*			openFile( const Path& fileName ) TIKI_OVERRIDE_FINAL;
		virtual void					openEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void					saveEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void					closeEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void					closeAll() TIKI_OVERRIDE_FINAL;

		virtual void					registerFileEditor( FileEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual void					unregisterFileEditor( FileEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual FileEditor*				findEditorForFile( const Path& fileName ) const;

		virtual void					addGlobalRibbon( EditorRibbon* pTab ) TIKI_OVERRIDE_FINAL;
		virtual void					removeGlobalRibbon( EditorRibbon* pTab ) TIKI_OVERRIDE_FINAL;

		//virtual void					addGlobalDockWidget( QDockWidget* pWidget, Qt::DockWidgetArea area ) TIKI_OVERRIDE_FINAL;
		//virtual void					removeGlobalDockWidget( QDockWidget* pWidget ) TIKI_OVERRIDE_FINAL;

		virtual Project&				getProject() TIKI_OVERRIDE_FINAL;

		virtual const Path&				getProjectPath() const TIKI_OVERRIDE_FINAL;
		virtual const Path&				getContentPath() const TIKI_OVERRIDE_FINAL;
		virtual const Path&				getPackagePath() const TIKI_OVERRIDE_FINAL;

		//virtual QWidget*				getDialogParent() const TIKI_OVERRIDE_FINAL;
		//virtual QString				getDialogTitle() const TIKI_OVERRIDE_FINAL;

	private:

		Project						m_project;
		AssetConverterInterface*	m_pAssetConverter;

		Path						m_projectPath;
		Path						m_contentPath;
		Path						m_packagePath;

		List< BaseEditor* >			m_editors;
		List< EditorRibbon* >		m_ribbons;
		EditorRibbon*				m_pCurrentRibbon;

		PackageEditor*				m_pPackageEditor;
		GenericDataEditor*			m_pGenericDataEditor;
		ConverterEditor*			m_pConverterEditor;

		List< Editable* >			m_editables;
		Editable*					m_pCurrentEditable;

		EditorFileBrowserUi			m_fileBrowserUi;
		ToolMessageBox				m_messageBox;

		//QShortcut					m_openShortcut;
		//QShortcut					m_saveShortcut;
		//QShortcut					m_closeShortcut;

		void						setProjectPathes();
		void						setPackagePath();

		void						doRibbonUi();
		void						doBrowserUi();
		void						doEditableUi();
	};
}
