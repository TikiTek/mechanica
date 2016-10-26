#include "tiki/base/platform.hpp"

#include "editorwindow.hpp"

#include <QApplication>

namespace tiki
{
	int mainEntryPoint()
	{
		QApplication application( platform::getArgumentCount(), platform::getArgumentPointer() );

		EditorWindow window;
		window.show();

		return application.exec();
	}
}
