#include "tiki/qtapplication/qtmainwindow.hpp"

#include "tiki/qtapplication/qtribbontab.hpp"

namespace tiki
{
	QtMainWindow::QtMainWindow()
	{
		m_pRibbonTabs = new QTabWidget();
		//m_pRibbonTabs->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Maximum );
		m_pRibbonTabs->setFixedHeight( 110 );

		
		
		m_pFileTabs = new QTabWidget();
		m_pFileTabs->setTabsClosable( true );

		m_pMainLayout = new QVBoxLayout();
		m_pMainLayout->setContentsMargins( 2, 2, 2, 2 );

		m_pMainLayout->addWidget( m_pRibbonTabs );
		m_pMainLayout->addWidget( m_pFileTabs );

		m_pMainWidget = new QWidget();
		m_pMainWidget->setLayout( m_pMainLayout );

		setCentralWidget( m_pMainWidget );
	}

	QtMainWindow::~QtMainWindow()
	{
	}

	void QtMainWindow::addRibbonTab( QtRibbonTab* pRibbonTab )
	{
		m_pRibbonTabs->addTab( pRibbonTab, pRibbonTab->getTitle() );
	}

	void QtMainWindow::removeRibbonTab( QtRibbonTab* pRibbonTab )
	{
		m_pRibbonTabs->removeTab( m_pRibbonTabs->indexOf( pRibbonTab ) );
	}

	void QtMainWindow::openFileTab( const QString& title, QWidget* pWidget )
	{
		m_pFileTabs->addTab( pWidget, title );
	}
}