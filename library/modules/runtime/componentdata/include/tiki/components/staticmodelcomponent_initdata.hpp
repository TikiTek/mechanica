#pragma once
#ifndef __TIKI_STATICMODELCOMPONENT_INITDATA_HPP_INCLUDED__
#define __TIKI_STATICMODELCOMPONENT_INITDATA_HPP_INCLUDED__

#include "tiki/base/reflection.hpp"

namespace tiki
{
	class Model;

	struct StaticModelComponentInitData
	{
		const Model*	pModel;
	};
}

#endif // __TIKI_STATICMODELCOMPONENT_INITDATA_HPP_INCLUDED__
