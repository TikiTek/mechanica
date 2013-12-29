#pragma once
#ifndef __TIKI_CAMERA_HPP_INCLUDED__
#define __TIKI_CAMERA_HPP_INCLUDED__

#include "tiki/math/matrix.hpp"
#include "tiki/math/projection.hpp"

namespace tiki
{
	struct Quaternion;

	class Camera
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( Camera );

	public:

		void				create( const Vector3& position, const Quaternion& rotation, const Projection& projection, const Vector3& upVector = Vector3::unitY );

		void				setTransform( const Vector3& position, const Quaternion& rotation );

		const Matrix43&		getWorldMatrix() const			{ return m_world; }
		const Matrix43&		getViewMatrix() const			{ return m_view; }
		const Matrix44&		getViewProjectionMatrix() const	{ return m_viewProjection; }

		const Projection&	getProjection() const			{ return m_projection; }

		//const AABB&		getBoundingBox() const;
		//const Frustum&	getFrustum() const;

	private:

		Vector3		m_upVector;
		Projection	m_projection;

		Matrix43	m_world;
		Matrix43	m_view;
		Matrix44	m_viewProjection;

		void		createMatrix();

	};
}

#endif // __TIKI_CAMERA_HPP_INCLUDED__
