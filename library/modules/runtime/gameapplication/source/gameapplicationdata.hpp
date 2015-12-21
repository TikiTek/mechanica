#pragma once
#ifndef TIKI_GAMEAPPLICATIONDATA_HPP_INCLUDED
#define TIKI_GAMEAPPLICATIONDATA_HPP_INCLUDED

#include "tiki/base/timer.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/debugguiwindows/debugguiwindows.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/input/inputsystem.hpp"
#include "tiki/io/gamebuildfilesystem.hpp"
#include "tiki/resource/resourcemanager.hpp"
#include "tiki/toollibraries/iwebinterrface.hpp"

namespace tiki
{
	struct GameApplicationkData
	{
		GameApplicationkData()
		{
#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
			pWebInterface = nullptr;
#endif
		}

		MainWindow			mainWindow;
		GraphicsSystem		graphicSystem;
		ResourceManager		resourceManager;
		InputSystem			inputSystem;
		Timer				frameTimer;

		GamebuildFileSystem	gamebuildFileSystem;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		DebugGui			debugGui;
		DebugGuiWindows		debugGuiWindows;
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		IWebInterface*		pWebInterface;
#endif
	};
}

#endif // TIKI_GAMEAPPLICATIONDATA_HPP_INCLUDED
