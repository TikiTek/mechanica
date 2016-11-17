#include "tiki/genericdataeditor/genericdataeditorwidget.hpp"

#include <QHBoxLayout>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QComboBox>

namespace tiki
{
	GenericDataEditorWidget::GenericDataEditorWidget()
	{
		m_pTreeModel = new QStandardItemModel();
		m_pTreeModel->setHorizontalHeaderLabels( { "Key", "Value" } );

		m_pTreeView = new QTreeView();
		m_pTreeView->setModel( m_pTreeModel );

		m_pLayout = new QHBoxLayout();
		m_pLayout->addWidget( m_pTreeView );
		setLayout( m_pLayout );
	}

	GenericDataEditorWidget::~GenericDataEditorWidget()
	{
		delete m_pTreeRootItem;
		delete m_pTreeModel;
		delete m_pTreeView;
	}

	bool GenericDataEditorWidget::openFile( IFile* pFile )
	{
		QFileInfo fileInfo( pFile->getFileName() );

		m_pTreeRootItem = new QStandardItem( fileInfo.baseName() );

		m_pTreeModel->appendRow( m_pTreeRootItem );


		m_pTreeView->setIndexWidget( m_pTreeModel->indexFromItem( m_pTreeRootItem ), new QComboBox() );

		return true;
	}

	bool GenericDataEditorWidget::saveFile()
	{
		return true;
	}

	void GenericDataEditorWidget::closeFile()
	{

	}
}