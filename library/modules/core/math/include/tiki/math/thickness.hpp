#pragma once
#ifndef TIKI_THICKNESS_HPP_INCLUDED__
#define TIKI_THICKNESS_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Thickness
	{
		float left;
		float top;
		float right;
		float bottom;

		TIKI_FORCE_INLINE Thickness()
			: left( 0.0f ), top( 0.0f ), right( 0.0f ), bottom( 0.0f )
		{
		}

		TIKI_FORCE_INLINE Thickness( float all )
			: left( all ), top( all ), right( all ), bottom( all )
		{
		}

		TIKI_FORCE_INLINE Thickness( float _leftRight, float _topBottom )
			: left( _leftRight ), top( _topBottom ), right( _leftRight ), bottom( _topBottom )
		{
		}

		TIKI_FORCE_INLINE Thickness( float _left, float _top, float _right, float _bottom )
			: left( _left ), top( _top ), right( _right ), bottom( _bottom )
		{
		}

		TIKI_FORCE_INLINE float getWidth() const
		{
			return left + right;
		}

		TIKI_FORCE_INLINE float getHeight() const
		{
			return top + bottom;
		}

		TIKI_FORCE_INLINE Vector2 getLeftTop() const
		{
			return vector::create( left, top );
		}

		TIKI_FORCE_INLINE Vector2 getWidthHeight() const
		{
			return vector::create( left + right, top + bottom );
		}
	};
}


#endif // TIKI_THICKNESS_HPP_INCLUDED__
