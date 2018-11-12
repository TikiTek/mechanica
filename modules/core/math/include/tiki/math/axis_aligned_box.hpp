#pragma once

#include "tiki/math/vector.hpp"

namespace tiki
{
	enum AxisAlignedBoxVertices
	{
		//		   7-------6
		//		  /|      /|
		//		 / |     / |
		//		3--+----2  |
		//      |  4----+--5
		//		| /	    | /
		//		|/      |/
		//		0-------1

		AxisAlignedBoxVertices_XMinYMinZMin,
		AxisAlignedBoxVertices_XMaxYMinZMin,
		AxisAlignedBoxVertices_XMaxYMaxZMin,
		AxisAlignedBoxVertices_XMinYMaxZMin,
		AxisAlignedBoxVertices_XMinYMinZMax,
		AxisAlignedBoxVertices_XMaxYMinZMax,
		AxisAlignedBoxVertices_XMaxYMaxZMax,
		AxisAlignedBoxVertices_XMinYMaxZMax,

		AxisAlignedBoxVertices_Count
	};

	struct AxisAlignedBox
	{
	public:

					AxisAlignedBox();

		void		createFromCenterExtends( const Vector3& center, const Vector3& extents );
		void		createFromMinMax( const Vector3& _min, const Vector3& _max );
		void		createFromMinMax( float minX, float minY, float minZ, float maxX, float maxY, float maxZ );

		void		getVertices( Vector3 aVertices[ AxisAlignedBoxVertices_Count ] ) const;

		void		translate( const Vector3& translation );
		void		extend( const Vector3& _extents );

		bool		contains( const Vector3& point ) const;

		Vector3		getCenter() const;
		Vector3		getSize() const;

		Vector3		min;
		Vector3		max;
	};
}
