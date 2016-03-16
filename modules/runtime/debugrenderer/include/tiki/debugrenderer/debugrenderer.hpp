#pragma once
#ifndef TIKI_DEBUGRENDERER_HPP_INCLUDED
#define TIKI_DEBUGRENDERER_HPP_INCLUDED

#include "tiki/graphics/color.hpp"

namespace tiki
{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	class ImmediateRenderer;
	class ResourceManager;
	struct Box;
	struct Matrix43;
	struct Ray;
	struct Vector2;
	struct Vector3;
	
	namespace debugrenderer
	{
		void	initialize( ResourceManager& resourceManager );
		void	shutdown( ResourceManager& resourceManager );

		void	drawLines( const Vector3* pPoints, uint capacity, Color color = TIKI_COLOR_WHITE );

		void	drawLineRay( const Ray& ray, float length = 100.0f, Color color = TIKI_COLOR_WHITE );
		void	drawLineBox( const Box& box, Color color = TIKI_COLOR_WHITE );
		void	drawLineAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix );
		void	drawLineGrid( float gridSpacing = 5.0f, float gridSize = 10.0f, Color color = TIKI_COLOR_WHITE );
		void	drawLineCircle( const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color = TIKI_COLOR_WHITE );
		void	drawLineSphere( const Vector3& center, float radius, Color color = TIKI_COLOR_WHITE );

		void	drawSolidBox( const Box& box, Color color = TIKI_COLOR_WHITE );
		void	drawSolidAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix );

		void	drawText( const Vector2& position, Color color, const char* pTextFormat, ... );
		void	drawText3D( const Vector3& position, Color color, const char* pTextFormat, ... );

		void	flush( ImmediateRenderer& renderer );
	}
#endif
}

#endif // TIKI_DEBUGRENDERER_HPP_INCLUDED
