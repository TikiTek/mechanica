#ifndef TIKI_GRAPHICSTYPES_HPP__INCLUDED
#define TIKI_GRAPHICSTYPES_HPP__INCLUDED

#if __cplusplus
#include "tiki/base/inline.hpp"
#include "tiki/math/matrix.hpp"

#define TIKI_GRAPHICS_NAMESPACE_BEGIN namespace tiki {
#define TIKI_GRAPHICS_NAMESPACE_END }

namespace tiki
{
	struct GraphicsMatrix44
	{
		float	data[ 16 ];
	};

	TIKI_FORCE_INLINE void createGraphicsMatrix44( GraphicsMatrix44& mtx, const Matrix43& input )
	{
		mtx.data[  0u ] = input.rot.x.x;
		mtx.data[  1u ] = input.rot.y.x;
		mtx.data[  2u ] = input.rot.z.x;
		mtx.data[  3u ] = input.pos.x;
		mtx.data[  4u ] = input.rot.x.y;
		mtx.data[  5u ] = input.rot.y.y;
		mtx.data[  6u ] = input.rot.z.y;
		mtx.data[  7u ] = input.pos.y;
		mtx.data[  8u ] = input.rot.x.z;
		mtx.data[  9u ] = input.rot.y.z;
		mtx.data[ 10u ] = input.rot.z.z;
		mtx.data[ 11u ] = input.pos.y;
		mtx.data[ 12u ] = 0.0f;
		mtx.data[ 13u ] = 0.0f;
		mtx.data[ 14u ] = 0.0f;
		mtx.data[ 15u ] = 1.0f;
	}

	TIKI_FORCE_INLINE void createGraphicsMatrix44( GraphicsMatrix44& mtx, const Matrix44& input )
	{
		mtx.data[  0u ] = input.x.x;
		mtx.data[  1u ] = input.y.x;
		mtx.data[  2u ] = input.z.x;
		mtx.data[  3u ] = input.w.x;
		mtx.data[  4u ] = input.x.y;
		mtx.data[  5u ] = input.y.y;
		mtx.data[  6u ] = input.z.y;
		mtx.data[  7u ] = input.w.y;
		mtx.data[  8u ] = input.x.z;
		mtx.data[  9u ] = input.y.z;
		mtx.data[ 10u ] = input.z.z;
		mtx.data[ 11u ] = input.w.z;
		mtx.data[ 12u ] = input.x.w;
		mtx.data[ 13u ] = input.y.w;
		mtx.data[ 14u ] = input.z.w;
		mtx.data[ 15u ] = input.w.w;
	}
}

#else

#define TIKI_GRAPHICS_NAMESPACE_BEGIN
#define TIKI_GRAPHICS_NAMESPACE_END

typedef float4x4 GraphicsMatrix44;

#endif

#endif // TIKI_GRAPHICSTYPES_HPP__INCLUDED
