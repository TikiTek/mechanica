#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextEdit>
#include <QDockWidget>

namespace tiki
{
	MainWindow::MainWindow( QWidget* pParent )
		: QMainWindow( pParent )
		, m_pUi( new Ui::MainWindow() )
	{
		
		//m_pUi->setupUi( this );

		QTextEdit* pTest4 = new QTextEdit();

		QVBoxLayout* pMainLayout = new QVBoxLayout();
		

		QDockWidget* pBla = new QDockWidget();
		pBla->setWidget( pTest4 );
		pBla->setWindowTitle( "Hello world" );

		QTextEdit* pTest3 = new QTextEdit();

		QTabWidget* pTest2 = new QTabWidget();
		pTest2->addTab( pTest3, "Bla " );

		pMainLayout->addWidget( pTest2 );

		m_pMainTabs = new QTabWidget();

		QTextEdit* pTest = new QTextEdit();
		m_pMainTabs->addTab( pTest, "Test" );

		pMainLayout->addWidget( m_pMainTabs );

		QWidget* pMainWidget = new QWidget();
		pMainWidget->setLayout( pMainLayout );

		//setLayout( pMainLayout );
		addDockWidget( Qt::LeftDockWidgetArea, pBla );

		setCentralWidget( pMainWidget );

		
		//saveGeometry();
		//saveState();
	}

	MainWindow::~MainWindow()
	{
		delete m_pUi;
	}
}