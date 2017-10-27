#include "tiki/base/platform.hpp"

#include "editor.hpp"

#include <QApplication>

namespace tiki
{
	int mainEntryPoint()
	{
		QApplication application( platform::getArgumentCount(), platform::getArgumentPointer() );
		QApplication::setApplicationName( "editor" );
		QApplication::setApplicationVersion( "0.2.0" );

		EditorWindow window;
		window.show();

		Editor editor( &window );

		const StaticArray< const char* >& arguments = platform::getArguments();
		for( uint i = 1u; i < arguments.getCount(); ++i )
		{
			editor.openFile( arguments[ i ] );
		}

		return application.exec();
	}
}
