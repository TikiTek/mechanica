#pragma once
#ifndef __TIKI_SKINNEDMODELCOMPONENT_INITDATA_HPP_INCLUDED__
#define __TIKI_SKINNEDMODELCOMPONENT_INITDATA_HPP_INCLUDED__

#include "tiki/base/reflection.hpp"
#include "tiki/graphics/graphics_references.hpp"

namespace tiki
{
	TIKI_REFLECTION_STRUCT(
		SkinnedModelComponentInitData,
		TIKI_REFLECTION_FIELD( ModelReference, model )
	);
}

#endif // __TIKI_SKINNEDMODELCOMPONENT_INITDATA_HPP_INCLUDED__
