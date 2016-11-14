#include "tiki/qtapplication/qtmainwindow.hpp"

#include "tiki/qtapplication/qtribbontab.hpp"

#include <QToolBar>

namespace tiki
{
	QtMainWindow::QtMainWindow()
	{
		m_pRibbonTabs = new QTabWidget();
		m_pRibbonTabs->setFixedHeight( 110 );

		m_pToolBar = new QToolBar();
		m_pToolBar->setMovable( false );
		m_pToolBar->addWidget( m_pRibbonTabs );
		addToolBar( m_pToolBar );

		layout()->setContentsMargins( 3, 3, 3, 3 );
	}

	QtMainWindow::~QtMainWindow()
	{
		delete m_pRibbonTabs;
		delete m_pToolBar;
	}

	void QtMainWindow::addRibbonTab( QtRibbonTab* pRibbonTab )
	{
		m_pRibbonTabs->addTab( pRibbonTab, pRibbonTab->getTitle() );
	}

	void QtMainWindow::removeRibbonTab( QtRibbonTab* pRibbonTab )
	{
		m_pRibbonTabs->removeTab( m_pRibbonTabs->indexOf( pRibbonTab ) );
	}
}