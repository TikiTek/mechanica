#include "tiki/base/platform.hpp"

#include "tiki/genericdataeditor/genericdataeditor.hpp"
#include "tiki/packageeditor/packageeditor.hpp"

#include "editor.hpp"

#include <QApplication>

namespace tiki
{
	int mainEntryPoint()
	{
		QApplication application( platform::getArgumentCount(), platform::getArgumentPointer() );

		EditorWindow window;
		window.show();

		Editor editor( &window );

		PackageEditor packageEditor( &editor );

		editor.registerFileEditor( &packageEditor );

		return application.exec();
	}
}
