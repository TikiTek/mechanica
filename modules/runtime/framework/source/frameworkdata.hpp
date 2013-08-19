#ifndef TIKI_FRAMEWORKDATA_HPP__
#define TIKI_FRAMEWORKDATA_HPP__

#include "tiki/framework/frameworkfactories.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/graphics/graphicsystem.hpp"
#include "tiki/resource/resourcemanager.hpp"
#include "tiki/physic/physicsystem.hpp"
#include "tiki/component/componentmanager.hpp"
#include "tiki/input/inputsystem.hpp"
#include "tiki/gamecore/timesystem.hpp"
#include "tiki/events/eventsystem.hpp"

namespace tiki
{
	struct FrameworkData
	{
		MainWindow			mainWindow;
		GraphicsSystem		graphicSystem;
		ResourceManager		resourceManager;
		PhysicSystem		physicSystem;
		ComponentManager	componentManager;
		InputSystem			inputSystem;
		TimeSystem			timeSystem;
		EventSystem			eventSystem;

		FrameworkFactories	factories;
	};

	namespace framework
	{
		FrameworkData&	getFrameworkData();
	}
}

#endif // TIKI_FRAMEWORKDATA_HPP__
