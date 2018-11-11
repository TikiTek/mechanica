#pragma once
#ifndef __TIKI_COMPONENT_TYPES_HPP_INCLUDED__
#define __TIKI_COMPONENT_TYPES_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	typedef uint16 EntityId;
	typedef uint16 ComponentTypeId;

	enum
	{
		InvalidEntityId			= 0u,
		InvalidComponentTypeId	= 0xffffu
	};
}

#endif // __TIKI_COMPONENT_TYPES_HPP_INCLUDED__
