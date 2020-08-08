#pragma once

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

		virtual b2Body*	getNativeObject() const = 0;

	protected:

		uint			m_materialId;
	};
}
