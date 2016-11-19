#pragma once
#ifndef TIKI_GENERICDATABOOLWIDGET_HPP_INCLUDED
#define TIKI_GENERICDATABOOLWIDGET_HPP_INCLUDED

#include "tiki/base/types.hpp"

#include <QWidget>

class QHBoxLayout;
class QCheckBox;

namespace tiki
{
	class GenericDataValue;

	class GenericDataBoolValueWidget : public QWidget
	{
		Q_OBJECT
		TIKI_NONCOPYABLE_CLASS( GenericDataBoolValueWidget );

	public:

		explicit	GenericDataBoolValueWidget( GenericDataValue* pValue );
		virtual		~GenericDataBoolValueWidget();

	private:

		GenericDataValue*	m_pValue;

		QHBoxLayout*		m_pLayout;
		QCheckBox*			m_pCheckBox;
	};
}

#endif // TIKI_GENERICDATABOOLWIDGET_HPP_INCLUDED
