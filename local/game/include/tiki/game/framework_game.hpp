#pragma once
#ifndef TIKI_FRAMEWORK_GAME_HPP_INCLUDED__
#define TIKI_FRAMEWORK_GAME_HPP_INCLUDED__

namespace tiki
{
	class GameFlowSystem;
	class ResourceRequestPool;

	namespace framework
	{
		GameFlowSystem&			getGameFlowSystem();

		ResourceRequestPool&	getResourceRequestPool();
	}
}

#endif // TIKI_FRAMEWORK_GAME_HPP_INCLUDED__
