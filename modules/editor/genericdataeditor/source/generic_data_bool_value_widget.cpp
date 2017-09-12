#include "generic_data_bool_value_widget.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/editorinterface/ifile.hpp"
#include "tiki/toolgenericdata/generic_data_value.hpp"

#include <QCheckBox>
#include <QHBoxLayout>

namespace tiki
{
	GenericDataBoolValueWidget::GenericDataBoolValueWidget( IFile* pFile, GenericDataValue* pValue )
		: m_pFile( pFile )
		, m_pValue( pValue )
	{
		m_pCheckBox = new QCheckBox();

		m_pLayout = new QHBoxLayout();
		m_pLayout->addWidget( m_pCheckBox );
		m_pLayout->setMargin( 0 );

		applyValue();

		setLayout( m_pLayout );

		connect( m_pCheckBox, &QCheckBox::stateChanged, this, &GenericDataBoolValueWidget::onChanged );
	}

	GenericDataBoolValueWidget::~GenericDataBoolValueWidget()
	{
		delete m_pLayout;
		delete m_pCheckBox;
	}

	void GenericDataBoolValueWidget::onChanged( int state )
	{
		const bool checked = (state != 0u);

		bool value = false;
		if( m_pValue->getBoolean( value ) && value != checked )
		{
			m_pValue->setBoolean( checked );
			m_pFile->markAsDirty();
		}
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
