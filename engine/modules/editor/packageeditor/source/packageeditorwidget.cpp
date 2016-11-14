#include "tiki/packageeditor/packageeditorwidget.hpp"

namespace tiki
{
	PackageEditorWidget::PackageEditorWidget()
		: m_pFile( nullptr )
	{
	}

	PackageEditorWidget::~PackageEditorWidget()
	{
	}

	bool PackageEditorWidget::openPackage( IEditorFile* pFile )
	{
		m_pFile = pFile;

		return true;
	}

	void PackageEditorWidget::savePackage()
	{
	}
}
