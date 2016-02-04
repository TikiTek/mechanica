#pragma once
#ifndef __TIKI_ENTITYTEMPLATE_HPP_INCLUDED__
#define __TIKI_ENTITYTEMPLATE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/staticarray.hpp"

namespace tiki
{
	struct EntityTemplateComponent
	{
		crc32		typeCrc;
		const void*	pInitData;
	};

	struct EntityTemplate
	{
		StaticArray< const EntityTemplateComponent > components;
	};
}

#endif // __TIKI_ENTITYTEMPLATE_HPP_INCLUDED__
