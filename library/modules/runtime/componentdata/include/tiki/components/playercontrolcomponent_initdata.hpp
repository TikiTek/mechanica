#pragma once
#ifndef __TIKI_PLAYERCONTROLCOMPONENT_INITDATA_HPP_INCLUDED__
#define __TIKI_PLAYERCONTROLCOMPONENT_INITDATA_HPP_INCLUDED__

#include "tiki/base/reflection.hpp"

namespace tiki
{
	TIKI_REFLECTION_STRUCT(
		PlayerControlComponentInitData,
		TIKI_REFLECTION_FIELD( float, speed )
	);
}

#endif // __TIKI_PLAYERCONTROLCOMPONENT_INITDATA_HPP_INCLUDED__
