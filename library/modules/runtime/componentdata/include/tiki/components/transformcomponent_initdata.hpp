#pragma once
#ifndef __TIKI_STATICMODELCOMPONENT_INITDATA_HPP_INCLUDED__
#define __TIKI_STATICMODELCOMPONENT_INITDATA_HPP_INCLUDED__

#include "tiki/base/reflection.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/math/quaternion.hpp"

namespace tiki
{
	struct TransformComponentInitData
	{
		Vector3		position;
		Quaternion	rotation;
	};
}

#endif // __TIKI_STATICMODELCOMPONENT_INITDATA_HPP_INCLUDED__
