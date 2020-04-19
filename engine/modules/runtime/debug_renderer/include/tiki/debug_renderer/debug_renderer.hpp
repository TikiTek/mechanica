#pragma once

#include "tiki/graphics/color.hpp"

namespace tiki
{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	class Camera;
	class Frustum;
	class ImmediateRenderer;
	class RenderTarget;
	class ResourceManager;
	struct AxisAlignedBox;
	struct Box;
	struct Circle;
	struct Line2;
	struct Line3;
	struct Matrix43;
	struct Ray3;
	struct Vector2;
	struct Vector3;

	namespace debugrenderer
	{
		void	initialize( ResourceManager& resourceManager );
		void	shutdown( ResourceManager& resourceManager );

		void	drawLine( const Line2& line, Color color = TIKI_COLOR_WHITE );
		void	drawLine( const Vector2& start, const Vector2& end, Color color = TIKI_COLOR_WHITE );
		void	drawLines( const Vector2* pPoints, uint capacity, Color color = TIKI_COLOR_WHITE );

		void	drawLine( const Line3& line, Color color = TIKI_COLOR_WHITE );
		void	drawLine( const Vector3& start, const Vector3& end, Color color = TIKI_COLOR_WHITE );
		void	drawLines( const Vector3* pPoints, uint capacity, Color color = TIKI_COLOR_WHITE );

		void	drawLineRay( const Ray3& ray, float length = 100.0f, Color color = TIKI_COLOR_WHITE );
		void	drawLineBox( const Box& box, Color color = TIKI_COLOR_WHITE );
		void	drawLineAxisAlignedBox( const AxisAlignedBox& axisAlignedBox, Color color = TIKI_COLOR_WHITE );
		void	drawLineAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix );
		void	drawLineGrid( float gridSpacing = 5.0f, float gridSize = 10.0f, Color color = TIKI_COLOR_WHITE );
		void	drawLineCircle( const Circle& circle, Color color = TIKI_COLOR_WHITE );
		void	drawLineCircle( const Vector2& center, float radius, Color color = TIKI_COLOR_WHITE );
		void	drawLineCircle( const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color = TIKI_COLOR_WHITE );
		void	drawLineSphere( const Vector3& center, float radius, Color color = TIKI_COLOR_WHITE );
		void	drawLineFrustum( const Frustum& frustum, Color color = TIKI_COLOR_WHITE );

		void	drawSolidBox( const Box& box, Color color = TIKI_COLOR_WHITE );
		void	drawSolidAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix );
		void	drawSolidTriangle( const Vector3& point1, const Vector3& point2, const Vector3& point3, Color color = TIKI_COLOR_WHITE );
		void	drawSolidCircle( const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color = TIKI_COLOR_WHITE );
		void	drawSolidRectangle( const Vector3& center, const Vector2& extends, const Vector3& normal, const Vector3& tangent, Color color = TIKI_COLOR_WHITE );

		void	drawText( const Vector2& position, Color color, const char* pTextFormat, ... );
		void	drawText3D( const Vector3& position, Color color, const char* pTextFormat, ... );

		void	flush( const ImmediateRenderer& renderer, const Camera& camera, const RenderTarget* pRenderTarget = nullptr );
	}
#endif
}
