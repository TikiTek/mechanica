#pragma once
#ifndef TIKI_AXISALIGNEDBOX_HPP_INCLUDED
#define TIKI_AXISALIGNEDBOX_HPP_INCLUDED

#include "tiki/math/vector.hpp"

namespace tiki
{
	enum AxisAlignedBoxVertices
	{
		AxisAlignedBoxVertices_XMinYMinZMin,
		AxisAlignedBoxVertices_XMaxYMinZMin,
		AxisAlignedBoxVertices_XMaxYMaxZMin,
		AxisAlignedBoxVertices_XMaxYMaxZMax,
		AxisAlignedBoxVertices_XMinYMaxZMax,
		AxisAlignedBoxVertices_XMinYMinZMax,
		AxisAlignedBoxVertices_XMinYMaxZMin,
		AxisAlignedBoxVertices_XMaxYMinZMax,

		AxisAlignedBoxVertices_Count
	};

	struct AxisAlignedBox
	{
	public:

					AxisAlignedBox();
					
		void		createFromCenterExtends( const Vector3& _center, const Vector3& _extents );
		void		createFromMinMax( const Vector3& min, const Vector3& max);
		void		createFromMinMax( float minX, float minY, float minZ, float maxX, float maxY, float maxZ );

		void		getVertices( Vector3 aVertices[ AxisAlignedBoxVertices_Count ] ) const;

		void		translate( const Vector3& translation );
		void		extend( const Vector3& _extents );

		Vector3		center;
		Vector3		extents;
	};
}

#endif // TIKI_AXISALIGNEDBOX_HPP_INCLUDED
