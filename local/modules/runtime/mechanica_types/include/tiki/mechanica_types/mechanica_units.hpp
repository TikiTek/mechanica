#pragma once

#include "tiki/math/vector.hpp"

namespace tiki
{
	static constexpr float UnitToDrawFactor = 100.0f;
	static constexpr float DrawToUnitFactor = 1.0f / UnitToDrawFactor;

	namespace mech
	{
		inline float	toUnitSize( float value );
		inline Vector2	toUnitSize( const Vector2& value );

		inline float	toDrawSize( float value );
		inline Vector2	toDrawSize( const Vector2& value );
	}
}

#include "../../../source/mechanica_units.inl"