#pragma once
#ifndef TIKI_GAMEFRAMEWORK_HPP
#define TIKI_GAMEFRAMEWORK_HPP

#include "tiki/base/types.hpp"

#include "tiki/framework/framework.hpp"

#include "../../../source/frameworkdata.hpp"

namespace tiki
{
	class GameFramework;

	namespace framework
	{
		GameFramework& getGame();
	}

	struct GameFrameworkParamters
	{
		GameFrameworkParamters()
		{
			screenWidth			= 900;
			screenHeight		= 600;
			fullScreen			= false;
			graphicsMode		= GraphicsRendererMode_Hardware;

			pWindowTitle		= "TikiEngine 3.0";

			pGamebuildPath		= "../../../../../gamebuild/";
		}

		uint					screenWidth;
		uint					screenHeight;
		bool					fullScreen;
		GraphicsRendererMode	graphicsMode;

		cstring					pWindowTitle;

		cstring					pGamebuildPath;

	};

	class GameFramework
	{
		friend int				framework::run();
		friend FrameworkData&	framework::getFrameworkData();

	protected:

									GameFramework();
		virtual						~GameFramework();

		int							run();

		virtual void				fillParameters( GameFrameworkParamters& parameters ) = 0;
		virtual bool				initialize() = 0;
		virtual void				shutdown() = 0;

		virtual void				update() = 0;
		virtual void				render( GraphicsContext& graphicsContext ) const = 0;

		virtual bool				processInputEvent( const InputEvent& inputEvent ) = 0;

	private:
		
		bool						m_isInitialized;
		
		GameFrameworkParamters		m_parameters;
		FrameworkData				m_frameworkData;
	
		bool						internInitialize();
		void						internShutdown();

		bool						frame();

	};
}

#endif // TIKI_GAMEFRAMEWORK_HPP
