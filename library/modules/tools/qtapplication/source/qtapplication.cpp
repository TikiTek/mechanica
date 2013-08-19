
#include "tiki/qtapplication/qtapplication.hpp"

namespace tiki
{
	QtApplication::QtApplication( int argc, char** argv )
		: QApplication( argc, argv )
	{

	}

	QtApplication::~QtApplication()
	{

	}

	int QtApplication::runInternal()
	{
		return 0;
	}
}