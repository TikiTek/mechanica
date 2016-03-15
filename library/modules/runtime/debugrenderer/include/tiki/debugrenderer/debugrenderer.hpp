#pragma once
#ifndef TIKI_DEBUGRENDERER_HPP_INCLUDED
#define TIKI_DEBUGRENDERER_HPP_INCLUDED

#include "tiki/graphics/color.hpp"

namespace tiki
{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	struct Box;
	struct Matrix43;
	struct Ray;
	struct Vector2;
	struct Vector3;
	
	namespace debugrenderer
	{
		void	drawLines( const Vector3* pPoints, uint capacity, Color color = TIKI_COLOR_WHITE );

		void	drawRay( const Ray& ray, float length = 100.0f, Color color = TIKI_COLOR_WHITE );
		void	drawBox( const Box& box, Color color = TIKI_COLOR_WHITE );
		void	drawGrid( int gridSpacing = 5, int gridSize = 10, Color color = TIKI_COLOR_WHITE );
		void	drawCircle( const Vector3& center, float radius, const Vector3& axe1, const Vector3& axe2, Color color = TIKI_COLOR_WHITE );
		void	drawSphere( const Vector3& center, float radius, Color color = TIKI_COLOR_WHITE );

		void	drawAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix );

		void	drawText( const Vector2& position, const char* pTextFormat, ... );
		void	drawText3D( const Vector3& position, const char* pTextFormat, ... );
	}
#endif
}

#endif // TIKI_DEBUGRENDERER_HPP_INCLUDED
