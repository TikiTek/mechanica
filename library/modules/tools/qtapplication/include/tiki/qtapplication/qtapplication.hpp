#pragma once
#ifndef TIKI_QTAPPLICATION_HPP__INCLUDED
#define TIKI_QTAPPLICATION_HPP__INCLUDED

#include "QtWidgets/QApplication.h"

namespace tiki
{
	class QtApplication : public QApplication
	{
		Q_OBJECT

	public:

						QtApplication( int argc, char** argv );
		virtual			~QtApplication();

	protected:

		virtual int		runInternal();

	};
}

#endif // TIKI_QTAPPLICATION_HPP__INCLUDED
