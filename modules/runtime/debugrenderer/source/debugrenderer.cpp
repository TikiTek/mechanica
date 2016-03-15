#include "tiki/debugrenderer/debugrenderer.hpp"

#include "tiki/container/staticarray.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/math/box.hpp"
#include "tiki/math/ray.hpp"

namespace tiki
{


	void debugrenderer::drawLines( const Vector3* pPoints, uint capacity, Color color /*= TIKI_COLOR_WHITE */ )
	{
		TIKI_ASSERT( pPoints != nullptr );
		TIKI_ASSERT( m_pContext != nullptr );

		m_pContext->setPrimitiveTopology( PrimitiveTopology_LineList );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		StaticArray< ImmediateVertex > vertices;
		m_pContext->beginImmediateGeometry( vertices, capacity );

		for( uint i = 0u; i < capacity; ++i )
		{
			ImmediateVertex& targetVertex	= vertices[ i ];
			const Vector3& sourcePoint		= pPoints[ i ];

			createFloat3( targetVertex.position, sourcePoint.x, sourcePoint.y, sourcePoint.z );
			targetVertex.u		= 0u;
			targetVertex.v		= 0u;
			targetVertex.color	= color;
		}

		vertices.dispose();

		m_pContext->endImmediateGeometry();
	}

	void debugrenderer::drawRay( const Ray& ray, float length /* = 100.0f */, Color color /*= TIKI_COLOR_WHITE */ )
	{
		Vector3 scaledDir = ray.direction;
		vector::scale( scaledDir, length );

		Vector3 end = ray.origin;
		vector::add( end, scaledDir );

		const Vector3 points[] ={ ray.origin, end };
		drawLines( points, TIKI_COUNT( points ), color );
	}

	void debugrenderer::drawBox( const Box& box, Color color /*= TIKI_COLOR_WHITE */ )
	{
		m_pContext->setPrimitiveTopology( PrimitiveTopology_LineList );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		Vector3 boxVertices[ 8 ];
		box.getVertices( &boxVertices[ 0 ] );

		const uint vertexCount = 24;

		StaticArray< ImmediateVertex > vertices;
		m_pContext->beginImmediateGeometry( vertices, vertexCount );

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
		for( uint i = 0u; i < vertexCount; ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color	= color;
			current.u		= 0u;
			current.v		= 0u;
		}

		vertices.dispose();

		m_pContext->endImmediateGeometry();
	}

	void debugrenderer::drawGrid( int gridSpacing /*= 10*/, int gridSize /*= 512*/, Color color /*= TIKI_COLOR_WHITE */ )
	{
		if( gridSpacing == 0 )
		{
			return;
		}

		m_pContext->setPrimitiveTopology( PrimitiveTopology_LineList );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		uint vertexCount = ((gridSize / gridSpacing) * 8) + 4;

		StaticArray< ImmediateVertex > vertices;
		m_pContext->beginImmediateGeometry( vertices, vertexCount );

		// fill array
		uint idx = 0;
		for( int i = 1; i < (gridSize / gridSpacing) + 1; i++ )
		{
			createFloat3( vertices[ idx++ ].position, (float)(i * gridSpacing), 0.0f, (float)(gridSize) );
			createFloat3( vertices[ idx++ ].position, (float)(i * gridSpacing), 0.0f, (float)(-gridSize) );
			createFloat3( vertices[ idx++ ].position, (float)(-i * gridSpacing), 0.0f, (float)(gridSize) );
			createFloat3( vertices[ idx++ ].position, (float)(-i * gridSpacing), 0.0f, (float)(-gridSize) );

			createFloat3( vertices[ idx++ ].position, (float)(gridSize), 0.0f, (float)(i * gridSpacing) );
			createFloat3( vertices[ idx++ ].position, (float)(-gridSize), 0.0f, (float)(i * gridSpacing) );
			createFloat3( vertices[ idx++ ].position, (float)(gridSize), 0.0f, (float)(-i * gridSpacing) );
			createFloat3( vertices[ idx++ ].position, (float)(-gridSize), 0.0f, (float)(-i * gridSpacing) );
		}

		// set color and uv
		for( uint i = 0u; i < idx; ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color = color;
			current.u = 0u;
			current.v = 0u;
		}

		// add highlights
		createFloat3( vertices[ idx ].position, 0.0f, 0.0f, (float)(-gridSize) );
		vertices[ idx++ ].color = TIKI_COLOR_GREEN;
		createFloat3( vertices[ idx ].position, 0.0f, 0.0f, (float)(gridSize) );
		vertices[ idx++ ].color = TIKI_COLOR_GREEN;
		createFloat3( vertices[ idx ].position, (float)(gridSize), 0.0f, 0.0f );
		vertices[ idx++ ].color = TIKI_COLOR_GREEN;
		createFloat3( vertices[ idx ].position, (float)(-gridSize), 0.0f, 0.0f );
		vertices[ idx++ ].color = TIKI_COLOR_GREEN;

		vertices.dispose();

		m_pContext->endImmediateGeometry();
	}

	void debugrenderer::drawAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix )
	{
		m_pContext->setPrimitiveTopology( PrimitiveTopology_LineList );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		uint vertexCount = 14 * 3;

		StaticArray< ImmediateVertex > vertices;
		m_pContext->beginImmediateGeometry( vertices, vertexCount );

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

		//TIKI_ASSERT( vertexCount == idx );

		// set color
		for( uint i = lastIdx + 1; i < vertexCount; ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color = TIKI_COLOR_BLUE;
		}


		// todo: set immediate renderer world matrix?
		for( uint i = 0; i < vertexCount; ++i )
		{
			Vector3 transformedPos;
			vector::set( transformedPos, vertices[ i ].position.x, vertices[ i ].position.y, vertices[ i ].position.z );

			matrix::transform( transformedPos, worldMatrix );

			vertices[ i ].position.x = transformedPos.x;
			vertices[ i ].position.y = transformedPos.y;
			vertices[ i ].position.z = transformedPos.z;
		}

		vertices.dispose();

		m_pContext->endImmediateGeometry();
	}


	void debugrenderer::drawCircle( const Vector3& center, float radius, const Vector3& axe1, const Vector3& axe2, Color color /*= TIKI_COLOR_WHITE */ )
	{
		m_pContext->setPrimitiveTopology( PrimitiveTopology_LineStrip );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		uint verticesPercircle = 30;
		uint vertexCount = verticesPercircle + 1;

		StaticArray< ImmediateVertex > vertices;
		m_pContext->beginImmediateGeometry( vertices, vertexCount );

		Vector3 scaleAxe1;
		vector::set( scaleAxe1, axe1.x * radius, axe1.y * radius, axe1.z * radius );

		Vector3 scaleAxe2;
		vector::set( scaleAxe2, axe2.x * radius, axe2.y * radius, axe2.z * radius );

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

		vertices.dispose();

		m_pContext->endImmediateGeometry();
	}

	void debugrenderer::drawSphere( const Vector3& center, float radius, Color color /*= TIKI_COLOR_WHITE */ )
	{
		debugrenderer::drawCircle( center, radius, Vector3::unitX, Vector3::unitY, color );
		debugrenderer::drawCircle( center, radius, Vector3::unitX, Vector3::unitZ, color );
		debugrenderer::drawCircle( center, radius, Vector3::unitZ, Vector3::unitY, color );
	}
}
