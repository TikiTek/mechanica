#include "tiki/qtapplication/qtribbontab.hpp"

namespace tiki
{
	QtRibbonTab::QtRibbonTab( const QString& title )
		: m_title( title )
	{
		m_pLayout = new QHBoxLayout();
		m_pLayout->setAlignment( Qt::AlignLeft );

		setLayout( m_pLayout );
	}

	QtRibbonTab::~QtRibbonTab()
	{
	}

	QtRibbonButton* QtRibbonTab::addButton( const QString& text, QIcon* pIcon /* = nullptr */ )
	{
		QtRibbonButton* pButton = new QtRibbonButton( text, pIcon );
		m_pLayout->addWidget( pButton );
		return pButton;
	}

	void QtRibbonTab::removeButton( QtRibbonButton* pButton )
	{
		m_pLayout->removeWidget( pButton );
	}
}