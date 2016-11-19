#include "genericdataboolvaluewidget.hpp"

#include <QCheckBox>
#include <QHBoxLayout>

namespace tiki
{
	GenericDataBoolValueWidget::GenericDataBoolValueWidget( GenericDataValue* pValue )
		: m_pValue( pValue )
	{
		m_pCheckBox = new QCheckBox();

		m_pLayout = new QHBoxLayout();
		m_pLayout->addWidget( m_pCheckBox );

		setLayout( m_pLayout );
	}

	GenericDataBoolValueWidget::~GenericDataBoolValueWidget()
	{
		delete m_pLayout;
		delete m_pCheckBox;
	}
}
