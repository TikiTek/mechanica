#pragma once
#ifndef TIKI_RENDERVIEW_HPP_INCLUDED
#define TIKI_RENDERVIEW_HPP_INCLUDED

#include "tiki/math/camera.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class RenderScene;
	class RenderTarget;

	struct RenderViewParameters
	{
		RenderViewParameters()
		{
			cameraPosition		= Vector3::zero;
			cameraRotation		= Quaternion::identity;
			cameraFieldOfView	= f32::piOver4;
			cameraNearPLane		= 0.001f;
			cameraFarPLane		= 1000.0f;
		}

		Vector3		cameraPosition;
		Quaternion	cameraRotation;
		float		cameraFieldOfView;
		float		cameraNearPLane;
		float		cameraFarPLane;
	};

	class RenderView
	{
		TIKI_NONCOPYABLE_CLASS( RenderView );
		friend class RenderScene;

	public:

							RenderView();
							~RenderView();

		bool				isCreated() const;

		const RenderScene&	getScene() const;

		float				getNearPlane() const { return m_nearPlane; }
		float				getFarPlane() const { return m_farPlane; }

		Camera&				getCamera();
		const Camera&		getCamera() const;

		void				prepareForRenderTarget( const RenderTarget& renderTarget );

	private: // friend

		void				create( const RenderScene& scene, const RenderViewParameters& parameters );
		void				dispose();

	private:

		const RenderScene*	m_pScene;

		float				m_fieldOfView;
		float				m_nearPlane;
		float				m_farPlane;
		Camera				m_camera;
	};
}

#endif // TIKI_RENDERVIEW_HPP_INCLUDED
