
#include "tiki/base/structs.hpp"
#include "tiki/math/vector3.hpp"
#include "tiki/math/vector4.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE void Color::fromVector3( const Vector3& source, const float alpha )
	{
		r = source.x;
		g = source.y;
		b = source.z;
		a = alpha;
	}

	TIKI_FORCE_INLINE void Color::fromVector4( const Vector4& source )
	{
		r = source.x;
		g = source.y;
		b = source.z;
		a = source.w;
	}

	TIKI_FORCE_INLINE void Color::toVector3( Vector3& target ) const
	{
		target.x = r;
		target.y = g;
		target.z = b;
	}

	TIKI_FORCE_INLINE void Color::toVector4( Vector4& target ) const
	{
		target.x = r;
		target.y = g;
		target.z = b;
		target.w = a;
	}

	TIKI_FORCE_INLINE void Color::toFloat3( float3& target ) const
	{
		target.x = r;
		target.y = g;
		target.z = b;
	}

	TIKI_FORCE_INLINE void Color::toFloat4( float4& target ) const
	{
		target.x = r;
		target.y = g;
		target.z = b;
		target.w = a;
	}
}
