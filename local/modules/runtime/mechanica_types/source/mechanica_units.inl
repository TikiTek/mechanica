#pragma once

namespace tiki
{
	float mech::toUnitSize( float value )
	{
		return value * DrawToUnitFactor;
	}

	Vector2 mech::toUnitSize( const Vector2& value )
	{
		return Vector2::scale( value, DrawToUnitFactor );
	}

	float mech::toDrawSize( float value )
	{
		return value * UnitToDrawFactor;
	}

	Vector2 mech::toDrawSize( const Vector2& value )
	{
		return Vector2::scale( value, UnitToDrawFactor );
	}
}