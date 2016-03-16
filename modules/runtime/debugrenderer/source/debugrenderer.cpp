#include "tiki/debugrenderer/debugrenderer.hpp"

#include "tiki/base/basicstring.hpp"
#include "tiki/base/zoneallocator.hpp"
#include "tiki/container/staticarray.hpp"
#include "tiki/graphics/immediaterenderer.hpp"

#include "debugrenderer_types.hpp"

#include <stdarg.h>

namespace tiki
{
	static DebugRenderCommand* s_pDebugRendererFirstCommand = nullptr;
	static DebugRenderCommand* s_pDebugRendererLastCommand = nullptr;
	static ZoneAllocator s_debugRendererData;

	namespace debugrenderer
	{
		template<class T>
		T*		allocateCommand( uint extraMemory = 0u );

		void	flushDrawLines( ImmediateRenderer& renderer, const Vector3* pPoints, uint pointCount, Color color );

		void	flushDrawLineRay( ImmediateRenderer& renderer, const DebugRenderLineRayCommand& command );
		void	flushDrawLineBox( ImmediateRenderer& renderer, const DebugRenderLineBoxCommand& command );
		void	flushDrawLineAxes( ImmediateRenderer& renderer, const DebugRenderLineAxesCommand& command );
		void	flushDrawLineGrid( ImmediateRenderer& renderer, const DebugRenderLineGridCommand& command );
		void	flushDrawLineCircle( ImmediateRenderer& renderer, const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color );
		void	flushDrawLineSphere( ImmediateRenderer& renderer, const DebugRenderLineSphereCommand& command );

		void	flushDrawSolidBox( ImmediateRenderer& renderer, const DebugRenderSolidBoxCommand& command );
		void	flushDrawSolidAxes( ImmediateRenderer& renderer, float lineLength, float lineOffset, const Matrix43& worldMatrix );

		void	flushDrawText( ImmediateRenderer& renderer, const DebugRenderTextCommand& command );
		void	flushDrawText3D( ImmediateRenderer& renderer, const DebugRenderText3DCommand& command );
	}

	template<class T>
	T* debugrenderer::allocateCommand( uint extraMemory /*= 0u*/ )
	{
		const uint sizeInBytes = sizeof( T ) + extraMemory;
		T* pCommand = (T*)s_debugRendererData.allocate( sizeInBytes );

		if( pCommand )
		{
			if( s_pDebugRendererFirstCommand == nullptr )
			{
				s_pDebugRendererFirstCommand = pCommand;
				s_pDebugRendererLastCommand = pCommand;
			}
			else
			{
				s_pDebugRendererFirstCommand->pNext = pCommand;
				s_pDebugRendererLastCommand = pCommand;
			}

			pCommand->pNext = nullptr;
			pCommand->type	= T::CommandType;
		}
		
		return pCommand;
	}

	void debugrenderer::flushDrawLines( ImmediateRenderer& renderer, const Vector3* pPoints, uint pointCount, Color color )
	{
		renderer.setPrimitiveTopology( PrimitiveTopology_LineList );
		renderer.setShaderMode( ImmediateShaderMode_Color );

		StaticArray< ImmediateVertex > vertices;
		renderer.beginImmediateGeometry( vertices, pointCount );

		for( uint i = 0u; i < pointCount; ++i )
		{
			ImmediateVertex& targetVertex	= vertices[ i ];
			const Vector3& sourcePoint		= pPoints[ i ];

			createFloat3( targetVertex.position, sourcePoint.x, sourcePoint.y, sourcePoint.z );
			targetVertex.u		= 0u;
			targetVertex.v		= 0u;
			targetVertex.color	= color;
		}

		renderer.endImmediateGeometry( vertices );
	}

	void debugrenderer::flushDrawLineRay( ImmediateRenderer& renderer, const DebugRenderLineRayCommand& command )
	{
		Vector3 scaledDir = command.ray.direction;
		vector::scale( scaledDir, command.length );

		Vector3 end = command.ray.origin;
		vector::add( end, scaledDir );

		const Vector3 points[] = { command.ray.origin, end };
		debugrenderer::flushDrawLines( renderer, points, TIKI_COUNT( points ), command.color );
	}

	void debugrenderer::flushDrawLineBox( ImmediateRenderer& renderer, const DebugRenderLineBoxCommand& command )
	{
		renderer.setPrimitiveTopology( PrimitiveTopology_LineList );
		renderer.setShaderMode( ImmediateShaderMode_Color );

		Vector3 boxVertices[ 8 ];
		command.box.getVertices( &boxVertices[ 0 ] );

		StaticArray< ImmediateVertex > vertices;
		renderer.beginImmediateGeometry( vertices, 24 );

		// draw lower rect
		createFloat3( vertices[ 0 ].position, boxVertices[ 0 ].x, boxVertices[ 0 ].y, boxVertices[ 0 ].z );
		createFloat3( vertices[ 1 ].position, boxVertices[ 1 ].x, boxVertices[ 1 ].y, boxVertices[ 1 ].z );

		createFloat3( vertices[ 2 ].position, boxVertices[ 1 ].x, boxVertices[ 1 ].y, boxVertices[ 1 ].z );
		createFloat3( vertices[ 3 ].position, boxVertices[ 2 ].x, boxVertices[ 2 ].y, boxVertices[ 2 ].z );

		createFloat3( vertices[ 4 ].position, boxVertices[ 2 ].x, boxVertices[ 2 ].y, boxVertices[ 2 ].z );
		createFloat3( vertices[ 5 ].position, boxVertices[ 3 ].x, boxVertices[ 3 ].y, boxVertices[ 3 ].z );

		createFloat3( vertices[ 6 ].position, boxVertices[ 3 ].x, boxVertices[ 3 ].y, boxVertices[ 3 ].z );
		createFloat3( vertices[ 7 ].position, boxVertices[ 0 ].x, boxVertices[ 0 ].y, boxVertices[ 0 ].z );

		// draw upper rect
		createFloat3( vertices[ 8 ].position, boxVertices[ 4 ].x, boxVertices[ 4 ].y, boxVertices[ 4 ].z );
		createFloat3( vertices[ 9 ].position, boxVertices[ 5 ].x, boxVertices[ 5 ].y, boxVertices[ 5 ].z );

		createFloat3( vertices[ 10 ].position, boxVertices[ 5 ].x, boxVertices[ 5 ].y, boxVertices[ 5 ].z );
		createFloat3( vertices[ 11 ].position, boxVertices[ 6 ].x, boxVertices[ 6 ].y, boxVertices[ 6 ].z );

		createFloat3( vertices[ 12 ].position, boxVertices[ 6 ].x, boxVertices[ 6 ].y, boxVertices[ 6 ].z );
		createFloat3( vertices[ 13 ].position, boxVertices[ 7 ].x, boxVertices[ 7 ].y, boxVertices[ 7 ].z );

		createFloat3( vertices[ 14 ].position, boxVertices[ 7 ].x, boxVertices[ 7 ].y, boxVertices[ 7 ].z );
		createFloat3( vertices[ 15 ].position, boxVertices[ 4 ].x, boxVertices[ 4 ].y, boxVertices[ 4 ].z );


		// draw vertical lines
		createFloat3( vertices[ 16 ].position, boxVertices[ 0 ].x, boxVertices[ 0 ].y, boxVertices[ 0 ].z );
		createFloat3( vertices[ 17 ].position, boxVertices[ 4 ].x, boxVertices[ 4 ].y, boxVertices[ 4 ].z );

		createFloat3( vertices[ 18 ].position, boxVertices[ 1 ].x, boxVertices[ 1 ].y, boxVertices[ 1 ].z );
		createFloat3( vertices[ 19 ].position, boxVertices[ 5 ].x, boxVertices[ 5 ].y, boxVertices[ 5 ].z );

		createFloat3( vertices[ 20 ].position, boxVertices[ 2 ].x, boxVertices[ 2 ].y, boxVertices[ 2 ].z );
		createFloat3( vertices[ 21 ].position, boxVertices[ 6 ].x, boxVertices[ 6 ].y, boxVertices[ 6 ].z );

		createFloat3( vertices[ 22 ].position, boxVertices[ 3 ].x, boxVertices[ 3 ].y, boxVertices[ 3 ].z );
		createFloat3( vertices[ 23 ].position, boxVertices[ 7 ].x, boxVertices[ 7 ].y, boxVertices[ 7 ].z );

		// set color and uv
		for( uint i = 0u; i < vertices.getCount(); ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color	= command.color;
			current.u		= 0u;
			current.v		= 0u;
		}

		renderer.endImmediateGeometry( vertices );
	}

	void debugrenderer::flushDrawLineAxes( ImmediateRenderer& renderer, const DebugRenderLineAxesCommand& command )
	{

	}

	void debugrenderer::flushDrawLineGrid( ImmediateRenderer& renderer, const DebugRenderLineGridCommand& command )
	{
		renderer.setPrimitiveTopology( PrimitiveTopology_LineList );
		renderer.setShaderMode( ImmediateShaderMode_Color );

		const float gridRatio = command.gridSize / command.gridSpace;
		const uint vertexCount = (uint(gridRatio) * 8) + 4;

		StaticArray< ImmediateVertex > vertices;
		renderer.beginImmediateGeometry( vertices, vertexCount );

		// fill array
		uint idx = 0;
		for( int i = 1; i < int( gridRatio ) + 1; i++ )
		{
			createFloat3( vertices[ idx++ ].position, (float)(i * command.gridSpace), 0.0f, (float)(command.gridSize) );
			createFloat3( vertices[ idx++ ].position, (float)(i * command.gridSpace), 0.0f, (float)(-command.gridSize) );
			createFloat3( vertices[ idx++ ].position, (float)(-i * command.gridSpace), 0.0f, (float)(command.gridSize) );
			createFloat3( vertices[ idx++ ].position, (float)(-i * command.gridSpace), 0.0f, (float)(-command.gridSize) );

			createFloat3( vertices[ idx++ ].position, (float)(command.gridSize), 0.0f, (float)(i * command.gridSpace) );
			createFloat3( vertices[ idx++ ].position, (float)(-command.gridSize), 0.0f, (float)(i * command.gridSpace) );
			createFloat3( vertices[ idx++ ].position, (float)(command.gridSize), 0.0f, (float)(-i * command.gridSpace) );
			createFloat3( vertices[ idx++ ].position, (float)(-command.gridSize), 0.0f, (float)(-i * command.gridSpace) );
		}

		// set color and uv
		for( uint i = 0u; i < idx; ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color = command.color;
			current.u = 0u;
			current.v = 0u;
		}

		// add highlights
		createFloat3( vertices[ idx ].position, 0.0f, 0.0f, (float)(-command.gridSize) );
		vertices[ idx++ ].color = TIKI_COLOR_GREEN;
		createFloat3( vertices[ idx ].position, 0.0f, 0.0f, (float)(command.gridSize) );
		vertices[ idx++ ].color = TIKI_COLOR_GREEN;
		createFloat3( vertices[ idx ].position, (float)(command.gridSize), 0.0f, 0.0f );
		vertices[ idx++ ].color = TIKI_COLOR_GREEN;
		createFloat3( vertices[ idx ].position, (float)(-command.gridSize), 0.0f, 0.0f );
		vertices[ idx++ ].color = TIKI_COLOR_GREEN;

		renderer.endImmediateGeometry( vertices );
	}

	void debugrenderer::flushDrawLineCircle( ImmediateRenderer& renderer, const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color )
	{
		renderer.setPrimitiveTopology( PrimitiveTopology_LineList );
		renderer.setShaderMode( ImmediateShaderMode_Color );

		uint verticesPercircle = 30;
		uint vertexCount = verticesPercircle + 1;

		StaticArray< ImmediateVertex > vertices;
		renderer.beginImmediateGeometry( vertices, vertexCount );

		Vector3 scaleAxe1;
		vector::set( scaleAxe1, normal.x * radius, normal.y * radius, normal.z * radius );

		Vector3 scaleAxe2;
		vector::set( scaleAxe2, tangent.x * radius, tangent.y * radius, tangent.z * radius );

		uint idx = 0u;
		for( uint i = 0u; i < verticesPercircle; i++ )
		{
			Vector3 vt;
			vector::set( vt, scaleAxe1.x, scaleAxe1.y, scaleAxe1.z );
			vector::scale( vt, f32::sin( (f32::twoPi / verticesPercircle) * i ) );

			Vector3 vtySin;
			vector::set( vtySin, scaleAxe2.x, scaleAxe2.y, scaleAxe2.z );
			vector::scale( vtySin, f32::cos( (f32::twoPi / verticesPercircle) * i ) );

			vector::add( vt, vtySin );
			vector::add( vt, center );

			createFloat3( vertices[ idx++ ].position, vt.x, vt.y, vt.z );
		}

		// add last vertex from end to start
		createFloat3( vertices[ idx++ ].position, vertices[ 0 ].position.x, vertices[ 0 ].position.y, vertices[ 0 ].position.z );

		// set color and uv
		TIKI_ASSERT( idx == vertexCount );
		for( uint i = 0u; i < vertexCount; ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color = color;
			current.u = 0u;
			current.v = 0u;
		}

		renderer.endImmediateGeometry( vertices );
	}

	void debugrenderer::flushDrawLineSphere( ImmediateRenderer& renderer, const DebugRenderLineSphereCommand& command )
	{
		debugrenderer::flushDrawLineCircle( renderer, command.center, command.radius, Vector3::unitX, Vector3::unitY, command.color );
		debugrenderer::flushDrawLineCircle( renderer, command.center, command.radius, Vector3::unitX, Vector3::unitZ, command.color );
		debugrenderer::flushDrawLineCircle( renderer, command.center, command.radius, Vector3::unitZ, Vector3::unitY, command.color );
	}

	void debugrenderer::flushDrawSolidAxes( ImmediateRenderer& renderer, float lineLength, float lineOffset, const Matrix43& worldMatrix )
	{
		renderer.setPrimitiveTopology( PrimitiveTopology_LineList );
		renderer.setShaderMode( ImmediateShaderMode_Color );

		StaticArray< ImmediateVertex > vertices;
		renderer.beginImmediateGeometry( vertices, 14u * 3u );

		float halfLineOffset	= lineOffset * 0.5f;
		float quarterLineLength = lineLength * 0.25f;
		float halfLineLength	= lineLength * 0.5f;

		uint idx = 0;

		// -- X Axis -- 
		createFloat3( vertices[ idx++ ].position, halfLineOffset, 0, 0 );
		createFloat3( vertices[ idx++ ].position, lineLength, 0, 0 );
		createFloat3( vertices[ idx++ ].position, lineOffset, 0, 0 );
		createFloat3( vertices[ idx++ ].position, lineOffset, lineOffset, 0 );
		createFloat3( vertices[ idx++ ].position, lineOffset, 0, 0 );
		createFloat3( vertices[ idx++ ].position, lineOffset, 0, lineOffset );

		// arrow
		createFloat3( vertices[ idx++ ].position, lineLength, 0, 0 );
		createFloat3( vertices[ idx++ ].position, lineLength - quarterLineLength, 0.0f, quarterLineLength );
		createFloat3( vertices[ idx++ ].position, lineLength, 0, 0 );
		createFloat3( vertices[ idx++ ].position, lineLength - quarterLineLength, 0.0f, -quarterLineLength );

		createFloat3( vertices[ idx++ ].position, lineLength, 0, 0 );
		createFloat3( vertices[ idx++ ].position, lineLength - quarterLineLength, quarterLineLength, 0.0f );
		createFloat3( vertices[ idx++ ].position, lineLength, 0, 0 );
		createFloat3( vertices[ idx++ ].position, lineLength - quarterLineLength, -quarterLineLength, 0.0f );


		// set color
		for( uint i = 0u; i < idx; ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color = TIKI_COLOR_RED;
		}

		uint lastIdx = idx;

		// -- Y Axis -- 
		createFloat3( vertices[ idx++ ].position, 0, halfLineOffset, 0 );
		createFloat3( vertices[ idx++ ].position, 0, lineLength, 0 );
		createFloat3( vertices[ idx++ ].position, 0, lineOffset, 0 );
		createFloat3( vertices[ idx++ ].position, lineOffset, lineOffset, 0 );
		createFloat3( vertices[ idx++ ].position, 0, lineOffset, 0 );
		createFloat3( vertices[ idx++ ].position, 0, lineOffset, lineOffset );

		// arrow
		createFloat3( vertices[ idx++ ].position, 0, lineLength, 0 );
		createFloat3( vertices[ idx++ ].position, quarterLineLength, lineLength - quarterLineLength, 0 );
		createFloat3( vertices[ idx++ ].position, 0, lineLength, 0 );
		createFloat3( vertices[ idx++ ].position, -quarterLineLength, lineLength - quarterLineLength, 0 );

		createFloat3( vertices[ idx++ ].position, 0, lineLength, 0 );
		createFloat3( vertices[ idx++ ].position, 0, lineLength - quarterLineLength, quarterLineLength );
		createFloat3( vertices[ idx++ ].position, 0, lineLength, 0 );
		createFloat3( vertices[ idx++ ].position, 0, lineLength - quarterLineLength, -quarterLineLength );

		// set color
		for( uint i = lastIdx + 1; i < idx; ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color = TIKI_COLOR_GREEN;
		}

		lastIdx = idx;


		// -- Z Axis --
		createFloat3( vertices[ idx++ ].position, 0, 0, halfLineOffset );
		createFloat3( vertices[ idx++ ].position, 0, 0, lineLength );
		createFloat3( vertices[ idx++ ].position, 0, 0, lineOffset );
		createFloat3( vertices[ idx++ ].position, lineOffset, 0, lineOffset );
		createFloat3( vertices[ idx++ ].position, 0, 0, lineOffset );
		createFloat3( vertices[ idx++ ].position, 0, lineOffset, lineOffset );

		// arrow
		createFloat3( vertices[ idx++ ].position, 0, 0, lineLength );
		createFloat3( vertices[ idx++ ].position, 0, quarterLineLength, lineLength - quarterLineLength );
		createFloat3( vertices[ idx++ ].position, 0, 0, lineLength );
		createFloat3( vertices[ idx++ ].position, 0, -quarterLineLength, lineLength - quarterLineLength );

		createFloat3( vertices[ idx++ ].position, 0, 0, lineLength );
		createFloat3( vertices[ idx++ ].position, quarterLineLength, 0, lineLength - quarterLineLength );
		createFloat3( vertices[ idx++ ].position, 0, 0, lineLength );
		createFloat3( vertices[ idx++ ].position, -quarterLineLength, 0, lineLength - quarterLineLength );

		TIKI_ASSERT( vertices.getCount() == idx );

		// set color
		for( uint i = lastIdx + 1; i < vertices.getCount(); ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color = TIKI_COLOR_BLUE;
		}


		// todo: set immediate renderer world matrix?
		for( uint i = 0; i < vertices.getCount(); ++i )
		{
			Vector3 transformedPos;
			vector::set( transformedPos, vertices[ i ].position.x, vertices[ i ].position.y, vertices[ i ].position.z );

			matrix::transform( transformedPos, worldMatrix );

			vertices[ i ].position.x = transformedPos.x;
			vertices[ i ].position.y = transformedPos.y;
			vertices[ i ].position.z = transformedPos.z;
		}

		renderer.endImmediateGeometry( vertices );
	}

	void debugrenderer::initialize( ResourceManager& resourceManager )
	{
		s_pDebugRendererFirstCommand = nullptr;
		s_pDebugRendererLastCommand = nullptr;
		s_debugRendererData.create( 1u * 1024u * 1024u );
	}

	void debugrenderer::shutdown( ResourceManager& resourceManager )
	{
		s_pDebugRendererFirstCommand = nullptr;
		s_pDebugRendererLastCommand = nullptr;
		s_debugRendererData.dispose();
	}

	void debugrenderer::drawLines( const Vector3* pPoints, uint capacity, Color color /*= TIKI_COLOR_WHITE */ )
	{
		TIKI_ASSERT( pPoints != nullptr );

		DebugRenderLinesCommand* pCommand = debugrenderer::allocateCommand< DebugRenderLinesCommand >( sizeof( Vector3 ) * capacity );
		if( pCommand != nullptr )
		{
			pCommand->color			= color;
			pCommand->pointCount	= capacity;
			memory::copy( pCommand->aPoints, pPoints, sizeof( Vector3 ) * capacity );
		}
	}

	void debugrenderer::drawLineRay( const Ray& ray, float length /*= 100.0f*/, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderLineRayCommand* pCommand = debugrenderer::allocateCommand< DebugRenderLineRayCommand >();
		if( pCommand != nullptr )
		{
			pCommand->ray			= ray;
			pCommand->length		= length;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawLineBox( const Box& box, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderLineBoxCommand* pCommand = debugrenderer::allocateCommand< DebugRenderLineBoxCommand >();
		if( pCommand != nullptr )
		{
			pCommand->box			= box;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawLineAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix )
	{
		DebugRenderLineAxesCommand* pCommand = debugrenderer::allocateCommand< DebugRenderLineAxesCommand >();
		if( pCommand != nullptr )
		{
			pCommand->lineLength	= lineLength;
			pCommand->lineOffset	= lineOffset;
			pCommand->worldMatrix	= worldMatrix;
		}
	}

	void debugrenderer::drawLineGrid( float gridSpacing /*= 5.0f*/, float gridSize /*= 10.0f*/, Color color /*= TIKI_COLOR_WHITE */ )
	{
		if( gridSpacing == 0 )
		{
			return;
		}

		DebugRenderLineGridCommand* pCommand = debugrenderer::allocateCommand< DebugRenderLineGridCommand >();
		if( pCommand != nullptr )
		{
			pCommand->gridSpace		= gridSpacing;
			pCommand->gridSize		= gridSize;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawLineCircle( const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderLineCircleCommand* pCommand = debugrenderer::allocateCommand< DebugRenderLineCircleCommand >();
		if( pCommand != nullptr )
		{
			pCommand->center		= center;
			pCommand->radius		= radius;
			pCommand->normal		= normal;
			pCommand->tangent		= tangent;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawLineSphere( const Vector3& center, float radius, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderLineSphereCommand* pCommand = debugrenderer::allocateCommand< DebugRenderLineSphereCommand >();
		if( pCommand != nullptr )
		{
			pCommand->center		= center;
			pCommand->radius		= radius;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawSolidBox( const Box& box, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderSolidBoxCommand* pCommand = debugrenderer::allocateCommand< DebugRenderSolidBoxCommand >();
		if( pCommand != nullptr )
		{
			pCommand->box			= box;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawSolidAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix )
	{
		DebugRenderSolidAxesCommand* pCommand = debugrenderer::allocateCommand< DebugRenderSolidAxesCommand >();
		if( pCommand != nullptr )
		{
			pCommand->lineLength	= lineLength;
			pCommand->lineOffset	= lineOffset;
			pCommand->worldMatrix	= worldMatrix;
		}
	}

	void debugrenderer::drawText( const Vector2& position, Color color, const char* pTextFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pTextFormat );
		const string text = formatStringArgs( pTextFormat, argptr );
		va_end( argptr );

		DebugRenderTextCommand* pCommand = debugrenderer::allocateCommand< DebugRenderTextCommand >( text.getLength() + 1u );
		if( pCommand != nullptr )
		{
			pCommand->position		= position;
			pCommand->color			= color;
			copyString( pCommand->text, text.getLength() + 1u, text.cStr() );
		}
	}

	void debugrenderer::drawText3D( const Vector3& position, Color color, const char* pTextFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pTextFormat );
		const string text = formatStringArgs( pTextFormat, argptr );
		va_end( argptr );

		DebugRenderText3DCommand* pCommand = debugrenderer::allocateCommand< DebugRenderText3DCommand >( text.getLength() + 1u );
		if( pCommand != nullptr )
		{
			pCommand->position		= position;
			pCommand->color			= color;
			copyString( pCommand->text, text.getLength() + 1u, text.cStr() );
		}
	}

	void debugrenderer::flush( ImmediateRenderer& renderer )
	{

	}
}
