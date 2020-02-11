#pragma once

#include "tiki/base/timer.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	struct MechanicaUpdateContext
	{
		Vector2		renderUnitSize;
		GameTime	gameTime;
	};
}