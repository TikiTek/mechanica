#pragma once
#ifndef __TIKI_FRAMEWORKFACTORIES_HPP_INCLUDED__
#define __TIKI_FRAMEWORKFACTORIES_HPP_INCLUDED__

namespace tiki
{
	class FrameworkFactories
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( FrameworkFactories );

	public:

		void	create( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem );
		void	dispose( ResourceManager& resourceManager );

	};
}

#endif // __TIKI_FRAMEWORKFACTORIES_HPP_INCLUDED__
