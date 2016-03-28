#include "tiki/math/axisalignedbox.hpp"

namespace tiki
{
	AxisAlignedBox::AxisAlignedBox()
	{
	}

	void AxisAlignedBox::createFromCenterExtends( const Vector3& center, const Vector3& extents )
	{
		Vector3 halfExtents = extents;
		vector::scale( halfExtents, 0.5f );

		vector::sub( min, center, halfExtents );
		vector::add( max, center, halfExtents );
	}

	void AxisAlignedBox::createFromMinMax( const Vector3& _min, const Vector3& _max )
	{
		min = _min;
		max = _max;
	}

	void AxisAlignedBox::createFromMinMax( float minX, float minY, float minZ, float maxX, float maxY, float maxZ )
	{
		vector::set( min,  minX, minY, minZ );
		vector::set( max, maxX, maxY, maxZ );
	}

	void AxisAlignedBox::getVertices( Vector3 aVertices[ AxisAlignedBoxVertices_Count ] ) const
	{
		vector::set( aVertices[ AxisAlignedBoxVertices_XMinYMinZMin ], min.x, min.y, min.z );
		vector::set( aVertices[ AxisAlignedBoxVertices_XMaxYMinZMin ], max.x, min.y, min.z );
		vector::set( aVertices[ AxisAlignedBoxVertices_XMaxYMaxZMin ], max.x, max.y, min.z );
		vector::set( aVertices[ AxisAlignedBoxVertices_XMinYMaxZMin ], min.x, max.y, min.z );
		vector::set( aVertices[ AxisAlignedBoxVertices_XMinYMinZMax ], min.x, min.y, max.z );
		vector::set( aVertices[ AxisAlignedBoxVertices_XMaxYMinZMax ], max.x, min.y, max.z );
		vector::set( aVertices[ AxisAlignedBoxVertices_XMaxYMaxZMax ], max.x, max.y, max.z );
		vector::set( aVertices[ AxisAlignedBoxVertices_XMinYMaxZMax ], min.x, max.y, max.z );
	}
	
	void AxisAlignedBox::translate( const Vector3& translation )
	{
		vector::add( min, translation );
		vector::add( max, translation );
	}

	void AxisAlignedBox::extend( const Vector3& _extents )
	{
		vector::sub( min, _extents );
		vector::add( max, _extents );
	}

	bool AxisAlignedBox::contains( const Vector3& point ) const
	{
		return min.x <= point.x && min.y <= point.y && min.z <= point.z &&
			max.x >= point.x && max.y >= point.y && max.z >= point.z;
	}

	Vector3 AxisAlignedBox::getSize() const
	{
		return vector::sub( Vector3(), max, min );
	}
}
