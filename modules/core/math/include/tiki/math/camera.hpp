#pragma once
#ifndef __TIKI_CAMERA_HPP_INCLUDED__
#define __TIKI_CAMERA_HPP_INCLUDED__

#include "tiki/math/frustum.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/math/quaternion.hpp"

namespace tiki
{
	struct Quaternion;
	struct Ray;

	class Camera
	{
		TIKI_NONCOPYABLE_CLASS( Camera );

	public:

		Camera();
		~Camera();

		void				create( const Camera& copy );
		void				create( const Vector3& position = Vector3::zero, const Quaternion& rotation = Quaternion::identity, const Projection* pProjection = nullptr, const Vector3& upVector = Vector3::unitY );

		const Projection&	getProjection() const { return m_projection; }
		void				setProjection( const Projection& projection );

		const Vector3&		getPosition() const	{ return m_position; }
		const Quaternion&	getRotation() const	{ return m_rotation; }

		void				setTransform( const Vector3& position, const Quaternion& rotation );

		const Matrix43&		getWorldMatrix() const			{ return m_world; }
		const Matrix43&		getViewMatrix() const			{ return m_view; }
		const Matrix44&		getViewProjectionMatrix() const	{ return m_viewProjection; }

		const Frustum&		getFrustum() const;
		void				getCameraRay( Ray& ray, float mousePosX, float mousePosY, float width, float height ) const;

	private:

		Vector3		m_upVector;
		Projection	m_projection;

		Vector3		m_position;
		Quaternion	m_rotation;

		Matrix43	m_world;
		Matrix43	m_view;
		Matrix44	m_viewProjection;

		mutable bool		m_isFrustumValid;
		mutable Frustum		m_frustum;

		void		createMatrix();

	};
}

#endif // __TIKI_CAMERA_HPP_INCLUDED__
