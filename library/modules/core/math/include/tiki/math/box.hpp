#pragma once
#ifndef TIKI_BOX_HPP__
#define TIKI_BOX_HPP__

#include "tiki/math/vector.hpp"

namespace tiki
{
	class Box
	{
		TIKI_NONCOPYABLE_CLASS( Box );

	public:
				Box( const Vector3& center, const Vector3& extents );

		void	getVertices( Vector3* pVertices ) const;

	public:
		Vector3		m_Center;
		Vector3		m_Extents;   
		Vector3		m_Axis[ 3 ];
	};

}

#endif // TIKI_BOX_HPP__
