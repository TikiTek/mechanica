#pragma once
#ifndef TIKI_QTRIBBONTAB_HPP_INCLUDED
#define TIKI_QTRIBBONTAB_HPP_INCLUDED

#include <QHBoxLayout>
#include <QWidget>

namespace tiki
{
	class QtRibbonButton;

	class QtRibbonTab : public QWidget
	{
		Q_OBJECT

	public:

						QtRibbonTab( const QString& title );
						~QtRibbonTab();

		const QString&	getTitle() const { return m_title; }

		QtRibbonButton*	addButton( const QString& text, const QIcon& icon );
		QtRibbonButton*	addButton( const QString& text, const QIcon* pIcon = nullptr );
		void			removeButton( QtRibbonButton* pButton );

	private:

		QString			m_title;

		QHBoxLayout*	m_pLayout;
	};
}

#endif // TIKI_QTRIBBONTAB_HPP_INCLUDED
