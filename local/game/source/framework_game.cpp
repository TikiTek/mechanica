
#include "tiki/game/framework_game.hpp"

#include "tiki/game/game.hpp"

namespace tiki
{
	GameFlowSystem& framework::getGameFlowSystem()
	{
		return static_cast< Game& >( framework::getGame() ).getGameFlowSystem();
	}

	ResourceRequestPool& framework::getResourceRequestPool()
	{
		return static_cast< Game& >( framework::getGame() ).getResourceRequestPool();
	}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	ImmediateRenderer& framework::getImmediateRenderer()
	{
		return static_cast< Game& >( framework::getGame() ).getImmediateRenderer();
	}
#endif
}