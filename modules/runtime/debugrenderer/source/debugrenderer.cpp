#include "tiki/debugrenderer/debugrenderer.hpp"

#include "tiki/base/basicstring.hpp"
#include "tiki/base/zoneallocator.hpp"
#include "tiki/container/staticarray.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/math/axisalignedbox.hpp"

#include "debugrenderer_types.hpp"

#include <stdarg.h>

namespace tiki
{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	static DebugRenderCommand* s_pDebugRendererFirstCommand = nullptr;
	static DebugRenderCommand* s_pDebugRendererLastCommand = nullptr;
	static ZoneAllocator s_debugRendererData;

	namespace debugrenderer
	{
		template<class T>
		T*		allocateCommand( uint extraMemory = 0u );

		void	flushDrawLines( const ImmediateRenderer& renderer, const Vector3* pPoints, uint pointCount, bool drawAsStrip, Color color );

		void	flushDrawLineRay( const ImmediateRenderer& renderer, const DebugRenderLineRayCommand& command );
		void	flushDrawLineBox( const ImmediateRenderer& renderer, const DebugRenderLineBoxCommand& command );
		void	flushDrawLineAxes( const ImmediateRenderer& renderer, const DebugRenderLineAxesCommand& command );
		void	flushDrawLineGrid( const ImmediateRenderer& renderer, const DebugRenderLineGridCommand& command );
		void	flushDrawLineCircle( const ImmediateRenderer& renderer, const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color );
		void	flushDrawLineSphere( const ImmediateRenderer& renderer, const DebugRenderLineSphereCommand& command );
		void	flushDrawLineFrustum( const ImmediateRenderer& renderer, const DebugRenderLineFrustumCommand& command );

		void	flushDrawSolidBox( const ImmediateRenderer& renderer, const DebugRenderSolidBoxCommand& command );
		void	flushDrawSolidAxes( const ImmediateRenderer& renderer, float lineLength, float lineOffset, const Matrix43& worldMatrix );

		void	flushDrawText( const ImmediateRenderer& renderer, const DebugRenderTextCommand& command );
		void	flushDrawText3D( const ImmediateRenderer& renderer, const DebugRenderText3DCommand& command );

		void	flushSetOption( const ImmediateRenderer& renderer, const DebugRenderSetOptionCommand& command );
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
				s_pDebugRendererLastCommand->pNext = pCommand;
				s_pDebugRendererLastCommand = pCommand;
			}

			pCommand->pNext = nullptr;
			pCommand->type	= T::CommandType;
		}
		else
		{
			TIKI_TRACE_WARNING( "[debugrenderer] Can't draw debug commaand. Out of memory.\n" );
		}
		
		return pCommand;
	}

	void debugrenderer::flushDrawLines( const ImmediateRenderer& renderer, const Vector3* pPoints, uint pointCount, bool drawAsStrip, Color color )
	{
		renderer.setPrimitiveTopology( drawAsStrip ? PrimitiveTopology_LineStrip : PrimitiveTopology_LineList );
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

	void debugrenderer::flushDrawLineRay( const ImmediateRenderer& renderer, const DebugRenderLineRayCommand& command )
	{
		Vector3 scaledDir = command.ray.direction;
		vector::scale( scaledDir, command.length );

		Vector3 end = command.ray.origin;
		vector::add( end, scaledDir );

		const Vector3 points[] = { command.ray.origin, end };
		debugrenderer::flushDrawLines( renderer, points, TIKI_COUNT( points ), false, command.color );
	}

	void debugrenderer::flushDrawLineBox( const ImmediateRenderer& renderer, const DebugRenderLineBoxCommand& command )
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

	void debugrenderer::flushDrawLineAxes( const ImmediateRenderer& renderer, const DebugRenderLineAxesCommand& command )
	{

	}

	void debugrenderer::flushDrawLineGrid( const ImmediateRenderer& renderer, const DebugRenderLineGridCommand& command )
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

	void debugrenderer::flushDrawLineCircle( const ImmediateRenderer& renderer, const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color )
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

	void debugrenderer::flushDrawLineSphere( const ImmediateRenderer& renderer, const DebugRenderLineSphereCommand& command )
	{
		debugrenderer::flushDrawLineCircle( renderer, command.center, command.radius, Vector3::unitX, Vector3::unitY, command.color );
		debugrenderer::flushDrawLineCircle( renderer, command.center, command.radius, Vector3::unitX, Vector3::unitZ, command.color );
		debugrenderer::flushDrawLineCircle( renderer, command.center, command.radius, Vector3::unitZ, Vector3::unitY, command.color );
	}

	void debugrenderer::flushDrawLineFrustum( const ImmediateRenderer& renderer, const DebugRenderLineFrustumCommand& command )
	{
		Vector3 cornerPoints[ 8u ];
		if( !command.frustum.getCorners( cornerPoints ) )
		{
			TIKI_TRACE_ERROR( "[debugrenderer] Could not draw frustum.\n" );
		}

		Vector3	nearPoints[] = { 
			cornerPoints[ FrustumCorner_NearRightBottom ],
			cornerPoints[ FrustumCorner_NearRightTop ],
			cornerPoints[ FrustumCorner_NearLeftTop ],
			cornerPoints[ FrustumCorner_NearLeftBottom ]
		};

		Vector3	farPoints[] = {
			cornerPoints[ FrustumCorner_FarRightBottom ],
			cornerPoints[ FrustumCorner_FarRightTop ],
			cornerPoints[ FrustumCorner_FarLeftTop ],
			cornerPoints[ FrustumCorner_FarLeftBottom ]
		};

		Vector3	topLeft[] = { cornerPoints[ FrustumCorner_NearLeftTop ], cornerPoints[ FrustumCorner_FarLeftTop ] };
		Vector3	topRight[] = { cornerPoints[ FrustumCorner_NearRightTop ], cornerPoints[ FrustumCorner_FarRightTop ] };
		Vector3	bottomLeft[] = { cornerPoints[ FrustumCorner_NearLeftBottom ], cornerPoints[ FrustumCorner_FarLeftBottom ] };
		Vector3	bottomRight[] = { cornerPoints[ FrustumCorner_NearRightBottom ], cornerPoints[ FrustumCorner_FarRightBottom ] };

		flushDrawLines( renderer, nearPoints, TIKI_COUNT( nearPoints ), true, command.color );
		flushDrawLines( renderer, farPoints, TIKI_COUNT( farPoints ), true, command.color );
		flushDrawLines( renderer, topLeft, TIKI_COUNT( topLeft ), true, command.color );
		flushDrawLines( renderer, topRight, TIKI_COUNT( topRight ), true, command.color );
		flushDrawLines( renderer, bottomLeft, TIKI_COUNT( bottomLeft ), true, command.color );
		flushDrawLines( renderer, bottomRight, TIKI_COUNT( bottomRight ), true, command.color );
	}

	void debugrenderer::flushDrawSolidBox( const ImmediateRenderer& renderer, const DebugRenderSolidBoxCommand& command )
	{

	}

	void debugrenderer::flushDrawSolidAxes( const ImmediateRenderer& renderer, float lineLength, float lineOffset, const Matrix43& worldMatrix )
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

	void debugrenderer::flushDrawText( const ImmediateRenderer& renderer, const DebugRenderTextCommand& command )
	{

	}

	void debugrenderer::flushDrawText3D( const ImmediateRenderer& renderer, const DebugRenderText3DCommand& command )
	{

	}

	void debugrenderer::flushSetOption( const ImmediateRenderer& renderer, const DebugRenderSetOptionCommand& command )
	{

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

	void debugrenderer::drawLine( const Vector3& start, const Vector3& end, Color color /*= TIKI_COLOR_WHITE */ )
	{
		const Vector3 aPoints[] = { start, end };
		debugrenderer::drawLines( aPoints, TIKI_COUNT( aPoints ), color );
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

	void debugrenderer::drawLineAxisAlignedBox( const AxisAlignedBox& axisAlignedBox, Color color /*= TIKI_COLOR_WHITE */ )
	{
		Box box;
		box.create( axisAlignedBox.min, axisAlignedBox.max );

		debugrenderer::drawLineBox( box, color );
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

	void debugrenderer::drawLineFrustum( const Frustum& frustum, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderLineFrustumCommand* pCommand = debugrenderer::allocateCommand< DebugRenderLineFrustumCommand >();
		if( pCommand != nullptr )
		{
			pCommand->frustum		= frustum;
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

	void debugrenderer::flush( const ImmediateRenderer& renderer, const Camera& camera, const RenderTarget* pRenderTarget /* = nullptr */ )
	{
		renderer.beginRenderPass( pRenderTarget, &camera );

		DebugRenderCommand* pCommand = s_pDebugRendererFirstCommand;
		while( pCommand != nullptr )
		{
			switch( pCommand->type )
			{
			case DebugRenderCommandType_DrawLines:
				{
					const DebugRenderLinesCommand& command = *(const DebugRenderLinesCommand*)pCommand;
					debugrenderer::flushDrawLines( renderer, command.aPoints, command.pointCount, false, command.color );
				}
				break;

			case DebugRenderCommandType_DrawLineRay:
				{
					const DebugRenderLineRayCommand& command = *(const DebugRenderLineRayCommand*)pCommand;
					debugrenderer::flushDrawLineRay( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawLineBox:
				{
					const DebugRenderLineBoxCommand& command = *(const DebugRenderLineBoxCommand*)pCommand;
					debugrenderer::flushDrawLineBox( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawLineAxes:
				{
					const DebugRenderLineAxesCommand& command = *(const DebugRenderLineAxesCommand*)pCommand;
					debugrenderer::flushDrawLineAxes( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawLineGrid:
				{
					const DebugRenderLineGridCommand& command = *(const DebugRenderLineGridCommand*)pCommand;
					debugrenderer::flushDrawLineGrid( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawLineCircle:
				{
					const DebugRenderLineCircleCommand& command = *(const DebugRenderLineCircleCommand*)pCommand;
					debugrenderer::flushDrawLineCircle( renderer, command.center, command.radius, command.normal, command.tangent, command.color );
				}
				break;

			case DebugRenderCommandType_DrawLineSphere:
				{
					const DebugRenderLineSphereCommand& command = *(const DebugRenderLineSphereCommand*)pCommand;
					debugrenderer::flushDrawLineSphere( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawLineFrustum:
				{
					const DebugRenderLineFrustumCommand& command = *(const DebugRenderLineFrustumCommand*)pCommand;
					debugrenderer::flushDrawLineFrustum( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawSolidBox:
				{
					const DebugRenderSolidBoxCommand& command = *(const DebugRenderSolidBoxCommand*)pCommand;
					debugrenderer::flushDrawSolidBox( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawSolidAxes:
				{
					const DebugRenderSolidAxesCommand& command = *(const DebugRenderSolidAxesCommand*)pCommand;
					debugrenderer::flushDrawSolidAxes( renderer, command.lineLength, command.lineOffset, command.worldMatrix );
				}
				break;

			case DebugRenderCommandType_DrawText:
				{
					const DebugRenderTextCommand& command = *(const DebugRenderTextCommand*)pCommand;
					debugrenderer::flushDrawText( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawText3D:
				{
					const DebugRenderText3DCommand& command = *(const DebugRenderText3DCommand*)pCommand;
					debugrenderer::flushDrawText3D( renderer, command );
				}
				break;

			case DebugRenderCommandType_SetOption:
				{
					const DebugRenderSetOptionCommand& command = *(const DebugRenderSetOptionCommand*)pCommand;
					debugrenderer::flushSetOption( renderer, command );
				}
				break;

			default:
				TIKI_TRACE_ERROR( "[debugrenderer] Invalid debug render command.\n" );
				break;
			}

			pCommand = pCommand->pNext;
		}

		renderer.endRenderPass();

		s_pDebugRendererFirstCommand = nullptr;
		s_pDebugRendererLastCommand = nullptr;
		s_debugRendererData.clear();
	}
#endif
}
