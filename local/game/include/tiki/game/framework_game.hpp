#pragma once
#ifndef TIKI_FRAMEWORK_GAME_HPP_INCLUDED__
#define TIKI_FRAMEWORK_GAME_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	class GameFlowSystem;
	class ImmediateRenderer;
	class ResourceRequestPool;

	namespace framework
	{
		GameFlowSystem&			getGameFlowSystem();

		ResourceRequestPool&	getResourceRequestPool();

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		ImmediateRenderer&		getImmediateRenderer();
#endif
	}
}

#endif // TIKI_FRAMEWORK_GAME_HPP_INCLUDED__
