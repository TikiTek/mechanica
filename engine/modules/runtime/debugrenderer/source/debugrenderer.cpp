#include "tiki/debugrenderer/debugrenderer.hpp"

#include "tiki/base/basicstring.hpp"
#include "tiki/base/zoneallocator.hpp"
#include "tiki/container/staticarray.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/resource/resourcemanager.hpp"

#include "debugrenderer_types.hpp"

#include <stdarg.h>

namespace tiki
{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	class DebugRenderer
	{
		TIKI_NONCOPYABLE_CLASS( DebugRenderer );

	public:

								DebugRenderer();
								~DebugRenderer();

		void					create( ResourceManager& resourceManager );
		void					dispose( ResourceManager& resourceManager );

		template<class T> T*	allocateCommand2D( uint extraMemory = 0u );
		template<class T> T*	allocateCommand3D( uint extraMemory = 0u );

		void					flush( const ImmediateRenderer& renderer, const Camera& camera, const RenderTarget* pRenderTarget );

	private:

		struct CommandList
		{
			CommandList()
			{
				clear();
			}

			void clear()
			{
				pFirstCommand	= nullptr;
				pLastCommand	= nullptr;
			}

			DebugRenderCommand*	pFirstCommand;
			DebugRenderCommand*	pLastCommand;
		};

		ZoneAllocator			m_data;

		CommandList				m_list2D;
		CommandList				m_list3D;

		const Font*				m_pFont;

		DebugRenderCommand*		allocateCommand( CommandList& list, uint size, DebugRenderCommandType type );

		void					flushList( const CommandList& list, const ImmediateRenderer& renderer, const Camera& camera );

		void					flushDrawLines( const ImmediateRenderer& renderer, const Vector3* pPoints, uint pointCount, bool drawAsStrip, Color color );

		void					flushDrawLineRay( const ImmediateRenderer& renderer, const DebugRenderLineRayCommand& command );
		void					flushDrawLineBox( const ImmediateRenderer& renderer, const DebugRenderLineBoxCommand& command );
		void					flushDrawLineAxisAlignedBox( const ImmediateRenderer& renderer, const AxisAlignedBox& box, Color color );
		void					flushDrawLineAxes( const ImmediateRenderer& renderer, const DebugRenderLineAxesCommand& command );
		void					flushDrawLineGrid( const ImmediateRenderer& renderer, const DebugRenderLineGridCommand& command );
		void					flushDrawLineCircle( const ImmediateRenderer& renderer, const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color );
		void					flushDrawLineSphere( const ImmediateRenderer& renderer, const DebugRenderLineSphereCommand& command );
		void					flushDrawLineFrustum( const ImmediateRenderer& renderer, const DebugRenderLineFrustumCommand& command );

		void					flushDrawSolidBox( const ImmediateRenderer& renderer, const DebugRenderSolidBoxCommand& command );
		void					flushDrawSolidAxes( const ImmediateRenderer& renderer, float lineLength, float lineOffset, const Matrix43& worldMatrix );
		void					flushDrawSolidTriangle( const ImmediateRenderer& renderer, const DebugRenderSolidTriangleCommand& command );
		void					flushDrawSolidCircle( const ImmediateRenderer& renderer, const DebugRenderSolidCircleCommand& command );
		void					flushDrawSolidRectangle( const ImmediateRenderer& renderer, const DebugRenderSolidRectangleCommand& command );

		void					flushDrawText( const ImmediateRenderer& renderer, const DebugRenderTextCommand& command );
		void					flushDrawText3D( const ImmediateRenderer& renderer, const DebugRenderText3DCommand& command, const Camera& camera );

		void					flushSetOption( const ImmediateRenderer& renderer, const DebugRenderSetOptionCommand& command );
		
	};

	static DebugRenderer s_debugRenderer;

	DebugRenderer::DebugRenderer()
	{
		m_pFont = nullptr;
	}

	DebugRenderer::~DebugRenderer()
	{
		TIKI_ASSERT( m_pFont == nullptr );
	}

	void DebugRenderer::create( ResourceManager& resourceManager )
	{
		m_data.create( 1u * 1024u * 1024u );

		m_list2D.clear();
		m_list3D.clear();

		m_pFont = resourceManager.loadResource< Font >( "debug.font" );
	}
	
	void DebugRenderer::dispose( ResourceManager& resourceManager )
	{
		resourceManager.unloadResource( m_pFont );
		m_pFont = nullptr;

		m_list2D.clear();
		m_list3D.clear();

		m_data.dispose();
	}

	template<class T>
	T* DebugRenderer::allocateCommand2D( uint extraMemory /*= 0u*/ )
	{
		const uint sizeInBytes = sizeof( T ) + extraMemory;
		return (T*)allocateCommand( m_list2D, sizeInBytes, T::CommandType );
	}

	template<class T>
	T* DebugRenderer::allocateCommand3D( uint extraMemory /*= 0u*/ )
	{
		const uint sizeInBytes = sizeof( T ) + extraMemory;
		return (T*)allocateCommand( m_list3D, sizeInBytes, T::CommandType );
	}

	void DebugRenderer::flush( const ImmediateRenderer& renderer, const Camera& camera, const RenderTarget* pRenderTarget )
	{
		renderer.beginRenderPass( pRenderTarget, &camera );
		flushList( m_list3D, renderer, camera );
		renderer.endRenderPass();

		renderer.beginRenderPass( pRenderTarget );
		flushList( m_list2D, renderer, camera );
		renderer.endRenderPass();

		m_list2D.clear();
		m_list3D.clear();

		m_data.clear();
	}

	DebugRenderCommand* DebugRenderer::allocateCommand( CommandList& list, uint size, DebugRenderCommandType type )
	{
		DebugRenderCommand* pCommand = (DebugRenderCommand*)m_data.allocate( size );

		if( pCommand )
		{
			if( list.pFirstCommand == nullptr )
			{
				list.pFirstCommand = pCommand;
				list.pLastCommand = pCommand;
			}
			else
			{
				list.pLastCommand->pNext = pCommand;
				list.pLastCommand = pCommand;
			}

			pCommand->pNext = nullptr;
			pCommand->type = type;
		}
		else
		{
			TIKI_TRACE_WARNING( "[debugrenderer] Can't draw debug command. Out of memory.\n" );
		}

		return pCommand;
	}
	
	void DebugRenderer::flushList( const CommandList& list, const ImmediateRenderer& renderer, const Camera& camera )
	{
		DebugRenderCommand* pCommand = list.pFirstCommand;
		while( pCommand != nullptr )
		{
			switch( pCommand->type )
			{
			case DebugRenderCommandType_DrawLines:
				{
					const DebugRenderLinesCommand& command = *(const DebugRenderLinesCommand*)pCommand;
					flushDrawLines( renderer, command.aPoints, command.pointCount, false, command.color );
				}
				break;

			case DebugRenderCommandType_DrawLineRay:
				{
					const DebugRenderLineRayCommand& command = *(const DebugRenderLineRayCommand*)pCommand;
					flushDrawLineRay( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawLineBox:
				{
					const DebugRenderLineBoxCommand& command = *(const DebugRenderLineBoxCommand*)pCommand;
					flushDrawLineBox( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawLineAxisAlignedBox:
				{
					const DebugRenderLineAxisAlignedBoxCommand& command = *(const DebugRenderLineAxisAlignedBoxCommand*)pCommand;
					flushDrawLineAxisAlignedBox( renderer, command.box, command.color );
				}
				break;

			case DebugRenderCommandType_DrawLineAxes:
				{
					const DebugRenderLineAxesCommand& command = *(const DebugRenderLineAxesCommand*)pCommand;
					flushDrawLineAxes( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawLineGrid:
				{
					const DebugRenderLineGridCommand& command = *(const DebugRenderLineGridCommand*)pCommand;
					flushDrawLineGrid( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawLineCircle:
				{
					const DebugRenderLineCircleCommand& command = *(const DebugRenderLineCircleCommand*)pCommand;
					flushDrawLineCircle( renderer, command.center, command.radius, command.normal, command.tangent, command.color );
				}
				break;

			case DebugRenderCommandType_DrawLineSphere:
				{
					const DebugRenderLineSphereCommand& command = *(const DebugRenderLineSphereCommand*)pCommand;
					flushDrawLineSphere( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawLineFrustum:
				{
					const DebugRenderLineFrustumCommand& command = *(const DebugRenderLineFrustumCommand*)pCommand;
					flushDrawLineFrustum( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawSolidBox:
				{
					const DebugRenderSolidBoxCommand& command = *(const DebugRenderSolidBoxCommand*)pCommand;
					flushDrawSolidBox( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawSolidAxes:
				{
					const DebugRenderSolidAxesCommand& command = *(const DebugRenderSolidAxesCommand*)pCommand;
					flushDrawSolidAxes( renderer, command.lineLength, command.lineOffset, command.worldMatrix );
				}
				break;

			case DebugRenderCommandType_DrawSolidTriangle:
				{
					const DebugRenderSolidTriangleCommand& command = *(const DebugRenderSolidTriangleCommand*)pCommand;
					flushDrawSolidTriangle( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawSolidCircle:
				{
					const DebugRenderSolidCircleCommand& command = *(const DebugRenderSolidCircleCommand*)pCommand;
					flushDrawSolidCircle( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawSolidRectangle:
				{
					const DebugRenderSolidRectangleCommand& command = *(const DebugRenderSolidRectangleCommand*)pCommand;
					flushDrawSolidRectangle( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawText:
				{
					const DebugRenderTextCommand& command = *(const DebugRenderTextCommand*)pCommand;
					flushDrawText( renderer, command );
				}
				break;

			case DebugRenderCommandType_DrawText3D:
				{
					const DebugRenderText3DCommand& command = *(const DebugRenderText3DCommand*)pCommand;
					flushDrawText3D( renderer, command, camera );
				}
				break;

			case DebugRenderCommandType_SetOption:
				{
					const DebugRenderSetOptionCommand& command = *(const DebugRenderSetOptionCommand*)pCommand;
					flushSetOption( renderer, command );
				}
				break;

			default:
				TIKI_TRACE_ERROR( "[debugrenderer] Invalid debug render command.\n" );
				break;
			}

			pCommand = pCommand->pNext;
		}
	}

	void DebugRenderer::flushDrawLines( const ImmediateRenderer& renderer, const Vector3* pPoints, uint pointCount, bool drawAsStrip, Color color )
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

	void DebugRenderer::flushDrawLineRay( const ImmediateRenderer& renderer, const DebugRenderLineRayCommand& command )
	{
		Vector3 scaledDir = command.ray.direction;
		vector::scale( scaledDir, command.length );

		Vector3 end = command.ray.origin;
		vector::add( end, scaledDir );

		const Vector3 points[] = { command.ray.origin, end };
		flushDrawLines( renderer, points, TIKI_COUNT( points ), false, command.color );

		AxisAlignedBox originBox;
		originBox.createFromCenterExtends( command.ray.origin, vector::create( 0.5f, 0.5f, 0.5f ) );
		flushDrawLineAxisAlignedBox( renderer, originBox, command.color );
	}

	void DebugRenderer::flushDrawLineBox( const ImmediateRenderer& renderer, const DebugRenderLineBoxCommand& command )
	{
		renderer.setPrimitiveTopology( PrimitiveTopology_LineList );
		renderer.setShaderMode( ImmediateShaderMode_Color );

		Vector3 points[ 8 ];
		command.box.getVertices( &points[ 0 ] );

		StaticArray< ImmediateVertex > vertices;
		renderer.beginImmediateGeometry( vertices, 24 );

		// draw lower rect
		createFloat3( vertices[ 0 ].position, points[ 0 ].x, points[ 0 ].y, points[ 0 ].z );
		createFloat3( vertices[ 1 ].position, points[ 1 ].x, points[ 1 ].y, points[ 1 ].z );

		createFloat3( vertices[ 2 ].position, points[ 1 ].x, points[ 1 ].y, points[ 1 ].z );
		createFloat3( vertices[ 3 ].position, points[ 2 ].x, points[ 2 ].y, points[ 2 ].z );

		createFloat3( vertices[ 4 ].position, points[ 2 ].x, points[ 2 ].y, points[ 2 ].z );
		createFloat3( vertices[ 5 ].position, points[ 3 ].x, points[ 3 ].y, points[ 3 ].z );

		createFloat3( vertices[ 6 ].position, points[ 3 ].x, points[ 3 ].y, points[ 3 ].z );
		createFloat3( vertices[ 7 ].position, points[ 0 ].x, points[ 0 ].y, points[ 0 ].z );

		// draw upper rect
		createFloat3( vertices[ 8 ].position, points[ 4 ].x, points[ 4 ].y, points[ 4 ].z );
		createFloat3( vertices[ 9 ].position, points[ 5 ].x, points[ 5 ].y, points[ 5 ].z );

		createFloat3( vertices[ 10 ].position, points[ 5 ].x, points[ 5 ].y, points[ 5 ].z );
		createFloat3( vertices[ 11 ].position, points[ 6 ].x, points[ 6 ].y, points[ 6 ].z );

		createFloat3( vertices[ 12 ].position, points[ 6 ].x, points[ 6 ].y, points[ 6 ].z );
		createFloat3( vertices[ 13 ].position, points[ 7 ].x, points[ 7 ].y, points[ 7 ].z );

		createFloat3( vertices[ 14 ].position, points[ 7 ].x, points[ 7 ].y, points[ 7 ].z );
		createFloat3( vertices[ 15 ].position, points[ 4 ].x, points[ 4 ].y, points[ 4 ].z );
		
		// draw vertical lines
		createFloat3( vertices[ 16 ].position, points[ 0 ].x, points[ 0 ].y, points[ 0 ].z );
		createFloat3( vertices[ 17 ].position, points[ 4 ].x, points[ 4 ].y, points[ 4 ].z );

		createFloat3( vertices[ 18 ].position, points[ 1 ].x, points[ 1 ].y, points[ 1 ].z );
		createFloat3( vertices[ 19 ].position, points[ 5 ].x, points[ 5 ].y, points[ 5 ].z );

		createFloat3( vertices[ 20 ].position, points[ 2 ].x, points[ 2 ].y, points[ 2 ].z );
		createFloat3( vertices[ 21 ].position, points[ 6 ].x, points[ 6 ].y, points[ 6 ].z );

		createFloat3( vertices[ 22 ].position, points[ 3 ].x, points[ 3 ].y, points[ 3 ].z );
		createFloat3( vertices[ 23 ].position, points[ 7 ].x, points[ 7 ].y, points[ 7 ].z );

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

	void DebugRenderer::flushDrawLineAxisAlignedBox( const ImmediateRenderer& renderer, const AxisAlignedBox& box, Color color )
	{
		renderer.setPrimitiveTopology( PrimitiveTopology_LineList );
		renderer.setShaderMode( ImmediateShaderMode_Color );

		Vector3 points[ AxisAlignedBoxVertices_Count ];
		box.getVertices( points );

		StaticArray< ImmediateVertex > vertices;
		renderer.beginImmediateGeometry( vertices, 24 );

		// set color and uv
		for( uint i = 0u; i < vertices.getCount(); ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color	= color;
			current.u		= 0u;
			current.v		= 0u;
		}

		// draw lower rect
		vector::toFloat( vertices[ 0 ].position, points[ AxisAlignedBoxVertices_XMinYMinZMin ] );
		vector::toFloat( vertices[ 1 ].position, points[ AxisAlignedBoxVertices_XMaxYMinZMin ] );

		vector::toFloat( vertices[ 2 ].position, points[ AxisAlignedBoxVertices_XMaxYMinZMin ] );
		vector::toFloat( vertices[ 3 ].position, points[ AxisAlignedBoxVertices_XMaxYMaxZMin ] );

		vector::toFloat( vertices[ 4 ].position, points[ AxisAlignedBoxVertices_XMaxYMaxZMin ] );
		vector::toFloat( vertices[ 5 ].position, points[ AxisAlignedBoxVertices_XMinYMaxZMin ] );

		vector::toFloat( vertices[ 6 ].position, points[ AxisAlignedBoxVertices_XMinYMaxZMin ] );
		vector::toFloat( vertices[ 7 ].position, points[ AxisAlignedBoxVertices_XMinYMinZMin ] );

		// draw upper rect
		vector::toFloat( vertices[ 8 ].position, points[ AxisAlignedBoxVertices_XMinYMinZMax ] );
		vector::toFloat( vertices[ 9 ].position, points[ AxisAlignedBoxVertices_XMaxYMinZMax ] );

		vector::toFloat( vertices[ 10 ].position, points[ AxisAlignedBoxVertices_XMaxYMinZMax ] );
		vector::toFloat( vertices[ 11 ].position, points[ AxisAlignedBoxVertices_XMaxYMaxZMax ] );

		vector::toFloat( vertices[ 12 ].position, points[ AxisAlignedBoxVertices_XMaxYMaxZMax ] );
		vector::toFloat( vertices[ 13 ].position, points[ AxisAlignedBoxVertices_XMinYMaxZMax ] );

		vector::toFloat( vertices[ 14 ].position, points[ AxisAlignedBoxVertices_XMinYMaxZMax ] );
		vector::toFloat( vertices[ 15 ].position, points[ AxisAlignedBoxVertices_XMinYMinZMax ] );
		
		// draw vertical lines
		vector::toFloat( vertices[ 16 ].position, points[ AxisAlignedBoxVertices_XMinYMinZMin ] );
		vector::toFloat( vertices[ 17 ].position, points[ AxisAlignedBoxVertices_XMinYMinZMax ] );

		vector::toFloat( vertices[ 18 ].position, points[ AxisAlignedBoxVertices_XMaxYMinZMin ] );
		vector::toFloat( vertices[ 19 ].position, points[ AxisAlignedBoxVertices_XMaxYMinZMax ] );

		vector::toFloat( vertices[ 20 ].position, points[ AxisAlignedBoxVertices_XMaxYMaxZMin ] );
		vector::toFloat( vertices[ 21 ].position, points[ AxisAlignedBoxVertices_XMaxYMaxZMax ] );

		vector::toFloat( vertices[ 22 ].position, points[ AxisAlignedBoxVertices_XMinYMaxZMin ] );
		vector::toFloat( vertices[ 23 ].position, points[ AxisAlignedBoxVertices_XMinYMaxZMax ] );

		renderer.endImmediateGeometry( vertices );
	}

	void DebugRenderer::flushDrawLineAxes( const ImmediateRenderer& renderer, const DebugRenderLineAxesCommand& command )
	{

	}

	void DebugRenderer::flushDrawLineGrid( const ImmediateRenderer& renderer, const DebugRenderLineGridCommand& command )
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

	void DebugRenderer::flushDrawLineCircle( const ImmediateRenderer& renderer, const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color )
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

	void DebugRenderer::flushDrawLineSphere( const ImmediateRenderer& renderer, const DebugRenderLineSphereCommand& command )
	{
		flushDrawLineCircle( renderer, command.center, command.radius, Vector3::unitX, Vector3::unitY, command.color );
		flushDrawLineCircle( renderer, command.center, command.radius, Vector3::unitX, Vector3::unitZ, command.color );
		flushDrawLineCircle( renderer, command.center, command.radius, Vector3::unitZ, Vector3::unitY, command.color );
	}

	void DebugRenderer::flushDrawLineFrustum( const ImmediateRenderer& renderer, const DebugRenderLineFrustumCommand& command )
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

	void DebugRenderer::flushDrawSolidBox( const ImmediateRenderer& renderer, const DebugRenderSolidBoxCommand& command )
	{

	}

	void DebugRenderer::flushDrawSolidAxes( const ImmediateRenderer& renderer, float lineLength, float lineOffset, const Matrix43& worldMatrix )
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

	void DebugRenderer::flushDrawSolidTriangle( const ImmediateRenderer& renderer, const DebugRenderSolidTriangleCommand& command )
	{
		renderer.setPrimitiveTopology( PrimitiveTopology_TriangleList );
		renderer.setShaderMode( ImmediateShaderMode_Color );

		StaticArray< ImmediateVertex > vertices;
		renderer.beginImmediateGeometry( vertices, command.points.getCount() );

		for( uint i = 0u; i < command.points.getCount(); ++i )
		{
			ImmediateVertex& vertex = vertices[ i ];

			vector::toFloat( vertex.position, command.points[ i ] );
			vertex.u		= 0u;
			vertex.v		= 0u;
			vertex.color	= command.color;
		}

		renderer.endImmediateGeometry( vertices );
	}

	void DebugRenderer::flushDrawSolidCircle( const ImmediateRenderer& renderer, const DebugRenderSolidCircleCommand& command )
	{
		renderer.setPrimitiveTopology( PrimitiveTopology_TriangleList );
		renderer.setShaderMode( ImmediateShaderMode_Color );

		const uint verticesPerCircle = TIKI_MAX( 16u, command.radius * 8u );
		const uint vertexCount = verticesPerCircle + 1u;

		StaticArray< ImmediateVertex > vertices;
		renderer.beginImmediateGeometry( vertices, vertexCount );

		Vector3 scaleAxe1 = command.normal;
		vector::scale( scaleAxe1, command.radius );

		Vector3 scaleAxe2 = command.tangent;
		vector::scale( scaleAxe2, command.radius );

		uint i = 0u;
		for( ; i < verticesPerCircle; i++ )
		{
			Vector3 vt;
			vector::set( vt, scaleAxe1.x, scaleAxe1.y, scaleAxe1.z );
			vector::scale( vt, f32::sin( (f32::twoPi / verticesPerCircle) * i ) );

			Vector3 vtySin;
			vector::set( vtySin, scaleAxe2.x, scaleAxe2.y, scaleAxe2.z );
			vector::scale( vtySin, f32::cos( (f32::twoPi / verticesPerCircle) * i ) );

			vector::add( vt, vtySin );
			vector::add( vt, command.center );

			vector::toFloat( vertices[ i ].position, vt );
		}

		// add last vertex from end to start
		vertices[ i++ ].position = vertices[ 0 ].position;

		// set color and uv
		TIKI_ASSERT( i == vertexCount );
		for( uint i = 0u; i < vertexCount; ++i )
		{
			ImmediateVertex& current = vertices[ i ];
			current.color	= command.color;
			current.u		= 0u;
			current.v		= 0u;
		}

		renderer.endImmediateGeometry( vertices );
	}

	void DebugRenderer::flushDrawSolidRectangle( const ImmediateRenderer& renderer, const DebugRenderSolidRectangleCommand& command )
	{
		renderer.setPrimitiveTopology( PrimitiveTopology_TriangleList );
		renderer.setShaderMode( ImmediateShaderMode_Color );

		static const Vector3 s_rectanglePoints[] =
		{ 
			{ -0.5f,  0.5f, 0.0f },
			{ -0.5f, -0.5f, 0.0f },
			{  0.5f, -0.5f, 0.0f },
			{ -0.5f,  0.5f, 0.0f },
			{  0.5f, -0.5f, 0.0f },
			{  0.5f,  0.5f, 0.0f },
		};

		StaticArray< ImmediateVertex > vertices;
		renderer.beginImmediateGeometry( vertices, TIKI_COUNT( s_rectanglePoints ) );

		Vector3 scaleAxe1 = command.normal;
		vector::scale( scaleAxe1, command.extends.x );

		Vector3 scaleAxe2 = command.tangent;
		vector::scale( scaleAxe2, command.extends.y );
		
		for( uint i = 0u; i < TIKI_COUNT( s_rectanglePoints ); ++i )
		{
			ImmediateVertex& vertex = vertices[ i ];

			Vector3 vtx = scaleAxe1;
			vector::scale( vtx, s_rectanglePoints[ i ].x );

			Vector3 vty = scaleAxe2;
			vector::scale( vty, s_rectanglePoints[ i ].y );

			Vector3 vt;
			vector::add( vt, vtx, vty );
			vector::add( vt, command.center );

			vector::toFloat( vertex.position, vt );
			vertex.color	= command.color;
			vertex.u		= 0u;
			vertex.v		= 0u;
		}

		renderer.endImmediateGeometry( vertices );
	}

	void DebugRenderer::flushDrawText( const ImmediateRenderer& renderer, const DebugRenderTextCommand& command )
	{
		renderer.drawText( command.position, *m_pFont, command.text, command.color );
	}

	void DebugRenderer::flushDrawText3D( const ImmediateRenderer& renderer, const DebugRenderText3DCommand& command, const Camera& camera )
	{
		const Projection& projection = camera.getProjection();

		Vector3 viewPosition = command.position;
		matrix::transform( viewPosition, camera.getViewMatrix() );

		vector::scale( viewPosition, 1.0f / viewPosition.z );

		Vector4 clipPosition;
		vector::set( clipPosition, viewPosition, 1.0f );
		matrix::transform( clipPosition, projection.getMatrix() );

		Vector2 screenPosition =
		{
			0.5f + (clipPosition.x * 0.5f),
			0.5f - (clipPosition.y * 0.5f)
		};

		const Vector2 screenSize = vector::create( projection.getWidth(), projection.getHeight() );
		vector::mul( screenPosition, screenSize );
		vector::floor( screenPosition );

		renderer.drawText( screenPosition, *m_pFont, command.text, command.color );
	}

	void DebugRenderer::flushSetOption( const ImmediateRenderer& renderer, const DebugRenderSetOptionCommand& command )
	{

	}
	
	void debugrenderer::drawLine( const Vector3& start, const Vector3& end, Color color /*= TIKI_COLOR_WHITE */ )
	{
		const Vector3 aPoints[] = { start, end };
		debugrenderer::drawLines( aPoints, TIKI_COUNT( aPoints ), color );
	}

	void debugrenderer::drawLines( const Vector3* pPoints, uint capacity, Color color /*= TIKI_COLOR_WHITE */ )
	{
		TIKI_ASSERT( pPoints != nullptr );

		DebugRenderLinesCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderLinesCommand >( sizeof( Vector3 ) * capacity );
		if( pCommand != nullptr )
		{
			pCommand->color			= color;
			pCommand->pointCount	= capacity;
			memory::copy( pCommand->aPoints, pPoints, sizeof( Vector3 ) * capacity );
		}
	}

	void debugrenderer::drawLineRay( const Ray& ray, float length /*= 100.0f*/, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderLineRayCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderLineRayCommand >();
		if( pCommand != nullptr )
		{
			pCommand->ray			= ray;
			pCommand->length		= length;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawLineBox( const Box& box, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderLineBoxCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderLineBoxCommand >();
		if( pCommand != nullptr )
		{
			pCommand->box			= box;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawLineAxisAlignedBox( const AxisAlignedBox& axisAlignedBox, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderLineAxisAlignedBoxCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderLineAxisAlignedBoxCommand >();
		if( pCommand != nullptr )
		{
			pCommand->box			= axisAlignedBox;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawLineAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix )
	{
		DebugRenderLineAxesCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderLineAxesCommand >();
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

		DebugRenderLineGridCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderLineGridCommand >();
		if( pCommand != nullptr )
		{
			pCommand->gridSpace		= gridSpacing;
			pCommand->gridSize		= gridSize;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawLineCircle( const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderLineCircleCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderLineCircleCommand >();
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
		DebugRenderLineSphereCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderLineSphereCommand >();
		if( pCommand != nullptr )
		{
			pCommand->center		= center;
			pCommand->radius		= radius;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawLineFrustum( const Frustum& frustum, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderLineFrustumCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderLineFrustumCommand >();
		if( pCommand != nullptr )
		{
			pCommand->frustum		= frustum;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawSolidBox( const Box& box, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderSolidBoxCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderSolidBoxCommand >();
		if( pCommand != nullptr )
		{
			pCommand->box			= box;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawSolidAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix )
	{
		DebugRenderSolidAxesCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderSolidAxesCommand >();
		if( pCommand != nullptr )
		{
			pCommand->lineLength	= lineLength;
			pCommand->lineOffset	= lineOffset;
			pCommand->worldMatrix	= worldMatrix;
		}
	}

	void debugrenderer::drawSolidTriangle( const Vector3& point1, const Vector3& point2, const Vector3& point3, Color color /* = TIKI_COLOR_WHITE */ )
	{
		DebugRenderSolidTriangleCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderSolidTriangleCommand >();
		if( pCommand != nullptr )
		{
			pCommand->points[ 0u ]	= point1;
			pCommand->points[ 1u ]	= point2;
			pCommand->points[ 2u ]	= point3;
			pCommand->color	= color;
		}
	}

	void debugrenderer::drawSolidCircle( const Vector3& center, float radius, const Vector3& normal, const Vector3& tangent, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderSolidCircleCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderSolidCircleCommand >();
		if( pCommand != nullptr )
		{
			pCommand->center		= center;
			pCommand->radius		= radius;
			pCommand->normal		= normal;
			pCommand->tangent		= tangent;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawSolidRectangle( const Vector3& center, const Vector2& extends, const Vector3& normal, const Vector3& tangent, Color color /*= TIKI_COLOR_WHITE */ )
	{
		DebugRenderSolidRectangleCommand* pCommand = s_debugRenderer.allocateCommand3D< DebugRenderSolidRectangleCommand >();
		if( pCommand != nullptr )
		{
			pCommand->center		= center;
			pCommand->extends		= extends;
			pCommand->normal		= normal;
			pCommand->tangent		= tangent;
			pCommand->color			= color;
		}
	}

	void debugrenderer::drawText( const Vector2& position, Color color, const char* pTextFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pTextFormat );
		const string text = formatStringArgs( pTextFormat, argptr );
		va_end( argptr );

		DebugRenderTextCommand* pCommand = s_debugRenderer.allocateCommand2D< DebugRenderTextCommand >( text.getLength() + 1u );
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

		DebugRenderText3DCommand* pCommand = s_debugRenderer.allocateCommand2D< DebugRenderText3DCommand >( text.getLength() + 1u );
		if( pCommand != nullptr )
		{
			pCommand->position		= position;
			pCommand->color			= color;
			copyString( pCommand->text, text.getLength() + 1u, text.cStr() );
		}
	}

	void debugrenderer::initialize( ResourceManager& resourceManager )
	{
		s_debugRenderer.create( resourceManager );
	}

	void debugrenderer::shutdown( ResourceManager& resourceManager )
	{
		s_debugRenderer.dispose( resourceManager );
	}

	void debugrenderer::flush( const ImmediateRenderer& renderer, const Camera& camera, const RenderTarget* pRenderTarget /* = nullptr */ )
	{
		s_debugRenderer.flush( renderer, camera, pRenderTarget );
	}
#endif
}
