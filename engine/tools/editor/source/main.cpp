#include "tiki/base/platform.hpp"

#include "mainwindow.h"

#include <QApplication>

namespace tiki
{
	int mainEntryPoint()
	{
		QApplication application( platform::getArgumentCount(), platform::getArgumentPointer() );

		MainWindow window;
		window.show();

		return application.exec();
	}
}
