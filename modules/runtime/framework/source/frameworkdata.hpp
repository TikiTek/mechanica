#ifndef TIKI_FRAMEWORKDATA_HPP__
#define TIKI_FRAMEWORKDATA_HPP__

#include "tiki/base/timer.hpp"
#include "tiki/framework/frameworkfactories.hpp"
#include "tiki/framework/inputsystem.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct FrameworkData
	{
		MainWindow			mainWindow;
		GraphicsSystem		graphicSystem;
		ResourceManager		resourceManager;
		InputSystem			inputSystem;
		Timer				frameTimer;

		FrameworkFactories	factories;
	};

	namespace framework
	{
		FrameworkData&	getFrameworkData();
	}
}

#endif // TIKI_FRAMEWORKDATA_HPP__
