#pragma once
#ifndef TIKI_QTMAINWINDOW_HPP_INCLUDED
#define TIKI_QTMAINWINDOW_HPP_INCLUDED

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>

namespace tiki
{
	class QtRibbonTab;

	class QtMainWindow : public QMainWindow
	{
		Q_OBJECT

	public:

						QtMainWindow();
						~QtMainWindow();

		void			addRibbonTab( QtRibbonTab* pRibbonTab );
		void			removeRibbonTab( QtRibbonTab* pRibbonTab );

		void			openFileTab( const QString& title, QWidget* pWidget );

	signals:

		void			fileClosed( QWidget* pWidget );

	private:

		QWidget*		m_pMainWidget;
		QVBoxLayout*	m_pMainLayout;

		QTabWidget*		m_pRibbonTabs;

		QTabWidget*		m_pFileTabs;
	};
}

#endif // TIKI_QTMAINWINDOW_HPP_INCLUDED
