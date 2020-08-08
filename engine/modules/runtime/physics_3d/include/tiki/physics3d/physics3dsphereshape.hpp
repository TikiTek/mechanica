#pragma once
#ifndef TIKI_PHYSICS3DSPHERESHAPE_HPP_INCLUDED
#define TIKI_PHYSICS3DSPHERESHAPE_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/physics3d/physics3dshape.hpp"

#include "BulletCollision/CollisionShapes/btSphereShape.h"

namespace tiki
{
	class Physics3dSphereShape : public Physics3dShape
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dSphereShape );

	public:

						Physics3dSphereShape();
		virtual			~Physics3dSphereShape();

		void			create( float radius );
		void			dispose();

		virtual void*	getNativeShape() const;

	private:

		btSphereShape	m_shape;
	};
}

#endif // TIKI_PHYSICS3DSPHERESHAPE_HPP_INCLUDED
