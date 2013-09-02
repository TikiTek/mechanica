#pragma once
#ifndef TIKI_GAMEFRAMEWORK_HPP
#define TIKI_GAMEFRAMEWORK_HPP

#include "tiki/base/types.hpp"

#include "tiki/framework/framework.hpp"

#include "frameworkdata.hpp"

namespace tiki
{
	class	GpuContext;

	struct GameFrameworkParamters
	{
		GameFrameworkParamters()
		{
			screenWidth			= 900;
			screenHeight		= 600;
			fullScreen			= false;
			useHardwareRenderer	= true;

			pTitle				= "TikiEngine 3.0";

			pGamebuildPath		= "../gamebuild/";
		}

		uint		screenWidth;
		uint		screenHeight;
		bool		fullScreen;
		bool		useHardwareRenderer;

		cstring		pTitle;

		cstring		pGamebuildPath;
	};

	class GameFramework
	{
		friend int				framework::run();
		friend FrameworkData&	framework::getFrameworkData();

	protected:

									GameFramework();
		virtual						~GameFramework();

		int							run();

		virtual void				fillParameters( GameFrameworkParamters* pParams ) = 0;
		virtual void				initialize() = 0;
		virtual void				shutdown() = 0;

		virtual void				update() = 0;
		virtual void				render( GraphicsContext* pGraphicsContext ) const = 0;

	private:

		GameFrameworkParamters		m_parameters;
		FrameworkData				m_frameworkData;
	
		bool						internInitialize();
		void						internShutdown();

		bool						frame();

	};

	GameFramework& getGameFramework();
}

#endif // TIKI_GAMEFRAMEWORK_HPP
