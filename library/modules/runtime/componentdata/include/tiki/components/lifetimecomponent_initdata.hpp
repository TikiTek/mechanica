#pragma once
#ifndef __TIKI_LIFETIMECOMPONENT_INITDATA_HPP_INCLUDED__
#define __TIKI_LIFETIMECOMPONENT_INITDATA_HPP_INCLUDED__

#include "tiki/base/reflection.hpp"

namespace tiki
{
	TIKI_REFLECTION_STRUCT(
		LifeTimeComponentInitData,
		TIKI_REFLECTION_FIELD( float, lifeTimeInSeconds )
	);
}

#endif // __TIKI_LIFETIMECOMPONENT_INITDATA_HPP_INCLUDED__
