#ifndef TIKI_GRAPHICSTYPES_HPP__INCLUDED
#define TIKI_GRAPHICSTYPES_HPP__INCLUDED

#if __cplusplus
#include "tiki/math/matrix.hpp"

namespace tiki
{
#endif
	struct GraphicsMatrix44
	{
		float	data[ 16 ];
	};

#if __cplusplus
	GraphicsMatrix44 createGraphicsMatrix44( const Matrix& input )
	{
		GraphicsMatrix44 mtx;

		mtx.data[  0u ] = input.n[  0u ];
		mtx.data[  1u ] = input.n[  1u ];
		mtx.data[  2u ] = input.n[  2u ];
		mtx.data[  3u ] = input.n[  3u ];
		mtx.data[  4u ] = input.n[  4u ];
		mtx.data[  5u ] = input.n[  5u ];
		mtx.data[  6u ] = input.n[  6u ];
		mtx.data[  7u ] = input.n[  7u ];
		mtx.data[  8u ] = input.n[  8u ];
		mtx.data[  9u ] = input.n[  9u ];
		mtx.data[ 10u ] = input.n[ 10u ];
		mtx.data[ 11u ] = input.n[ 11u ];
		mtx.data[ 12u ] = input.n[ 12u ];
		mtx.data[ 13u ] = input.n[ 13u ];
		mtx.data[ 14u ] = input.n[ 14u ];
		mtx.data[ 15u ] = input.n[ 15u ];

		return mtx;
	}
}
#endif

#endif // TIKI_GRAPHICSTYPES_HPP__INCLUDED
