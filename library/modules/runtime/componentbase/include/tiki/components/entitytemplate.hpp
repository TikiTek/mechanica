#pragma once
#ifndef __TIKI_ENTITYTEMPLATE_HPP_INCLUDED__
#define __TIKI_ENTITYTEMPLATE_HPP_INCLUDED__

#include "tiki/base/reflection.hpp"

namespace tiki
{
	TIKI_REFLECTION_STRUCT(
		EntityTemplateComponent,
		TIKI_REFLECTION_FIELD( crc32, typeCrc )
		TIKI_REFLECTION_FIELD( void*, pInitData )
	);

	TIKI_REFLECTION_STRUCT(
		EntityTemplate,
		TIKI_REFLECTION_FIELD( uint32, componentCount )
		TIKI_REFLECTION_FIELD( EntityTemplateComponent*, pComponents )
	);
}

#endif // __TIKI_ENTITYTEMPLATE_HPP_INCLUDED__
