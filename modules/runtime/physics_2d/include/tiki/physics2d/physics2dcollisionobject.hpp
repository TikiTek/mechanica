#pragma once
#ifndef TIKI_PHYSICS2DCOLLISIONOBJECT_HPP_INCLUDED
#define TIKI_PHYSICS2DCOLLISIONOBJECT_HPP_INCLUDED

#include "tiki/base/types.hpp"

class b2Body;

namespace tiki
{
	class Physics2dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dCollisionObject );

	public:

						Physics2dCollisionObject()	{}
		virtual			~Physics2dCollisionObject()	{}

		uint			getMaterialId() const { return m_materialId; }
		void			setMaterialId( uint materialId ) { m_materialId = materialId; }

		virtual b2Body*	getNativeObject() const TIKI_PURE;

	protected:

		uint			m_materialId;
	};
}

#endif // TIKI_PHYSICS2DCOLLISIONOBJECT_HPP_INCLUDED
