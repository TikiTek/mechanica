#include "genericdataboolvaluewidget.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

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

		applyValue();

		setLayout( m_pLayout );
	}

	GenericDataBoolValueWidget::~GenericDataBoolValueWidget()
	{
		delete m_pLayout;
		delete m_pCheckBox;
	}

	void GenericDataBoolValueWidget::applyValue()
	{
		bool value = false;
		if( m_pValue->getBoolean( value ) )
		{
			m_pCheckBox->setCheckState( value ? Qt::Checked : Qt::Unchecked );
		}
		else
		{
			TIKI_ASSERT( false );
		}
	}
}
