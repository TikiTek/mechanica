#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>

namespace Ui
{
	class MainWindow;
}

namespace tiki
{
	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:

		explicit		MainWindow( QWidget* pParent = nullptr );
		~MainWindow();

	private:

		Ui::MainWindow*	m_pUi;

		QWidget			m_pMainWidget;
		QVBoxLayout*	m_pMainLayout;

		QTabWidget*		m_pRibbonTabs;

		QTabWidget*		m_pMainTabs;
	};
}

#endif // MAINWINDOW_H
