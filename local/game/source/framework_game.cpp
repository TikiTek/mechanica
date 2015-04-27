
#include "tiki/game/framework_game.hpp"

#include "tiki/game/game.hpp"

namespace tiki
{
	GameFlowSystem& framework::getGameFlowSystem()
	{
		return static_cast<Game&>(framework::getGame()).getGameFlowSystem();
	}
}