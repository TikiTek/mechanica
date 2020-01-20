#ifndef __TIKI_CAMERAPARAMETER_HPP_INCLUDED__
#define __TIKI_CAMERAPARAMETER_HPP_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

#ifdef __cplusplus
#	include "tiki/math/camera.hpp"
#	include "tiki/math/projection.hpp"
#endif

TIKI_GRAPHICS_NAMESPACE_BEGIN

struct CameraParameter
{
	GraphicsMatrix44	inverseViewProjection;
	float4				param0; // x: n*f, y: f-n,	z: f,		w: 2*n
	float4				param1; // x: top, y: left,	z: bottom,	w: right
};

#ifdef __cplusplus

CameraParameter fillCameraParameter( const Camera& camera )
{
	CameraParameter data;

	Matrix44 inverseViewProjection;
	matrix::invert( inverseViewProjection, camera.getViewProjectionMatrix() );
	createGraphicsMatrix44( data.inverseViewProjection, inverseViewProjection );

	const Matrix44& projection = camera.getProjection().getMatrix();
	const float nearPlane	= camera.getProjection().getNearPlane();
	const float farPlane	= camera.getProjection().getFarPlane();

	const float bottom	= nearPlane * (projection.y.z - 1.0f) / projection.y.y;
	const float top		= nearPlane * (projection.y.z + 1.0f) / projection.y.y;
	const float left	= nearPlane * (projection.x.z - 1.0f) / projection.x.x;
	const float right	= nearPlane * (projection.x.z + 1.0f) / projection.x.x;

	createFloat4( data.param0, nearPlane * farPlane, farPlane - nearPlane, farPlane, 2.0f * nearPlane );
	createFloat4( data.param1, right-left, right+left, top-bottom, top+bottom );

	return data;
}

#endif

TIKI_GRAPHICS_NAMESPACE_END

#endif // __TIKI_CAMERAPARAMETER_HPP_INCLUDED__
