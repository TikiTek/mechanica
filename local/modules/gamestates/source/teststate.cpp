
#include "tiki/gamestates/teststate.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/stockvertex.hpp"
#include "tiki/graphicsbase/graphicstypes.hpp"
#include "tiki/graphicsbase/vertexattribute.hpp"
#include "tiki/graphicsresources/model.hpp"
#include "tiki/graphicsresources/shaderset.hpp"
#include "tiki/graphicsresources/texture.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct FallbackVertex
	{
		float3	position;
		float4	color;
		float2	texCoord;
	};
	
	struct FallbackVertexConstants
	{
		GraphicsMatrix44	mvpMatrix;
		GraphicsMatrix44	modelViewMatrix;
	};

	void TestState::create( ApplicationState* pParentState )
	{
		m_pParentState	= pParentState;
	}

	void TestState::dispose()
	{
		m_pParentState = nullptr;
	}

	TransitionState TestState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case TestStateTransitionSteps_LoadResources:
			if ( isCreating )
			{
				if ( isInital )
				{
					m_pShaderSet	= framework::getResourceManager().loadResource< ShaderSet >( "fallback.shader" );
					m_pTexture		= framework::getResourceManager().loadResource< Texture >( "checker.texture" );
					m_pModel		= framework::getResourceManager().loadResource< Model >( "unit_cube.model" );

					VertexAttribute attributes[] =
					{
						{ VertexSementic_Position,	0u, VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
						{ VertexSementic_TexCoord,	0u, VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
						{ VertexSementic_Color,		0u, VertexAttributeFormat_x32y32z32w32_float,	0u, VertexInputType_PerVertex }
					};
					m_pVertexFormat = framework::getGraphicsSystem().createVertexFormat( attributes, TIKI_COUNT( attributes ) );
					m_pInputBinding	= framework::getGraphicsSystem().createVertexInputBinding(
						m_pShaderSet->getShader( ShaderType_VertexShader, 0u ),
						m_pModel->getGeometryByIndex( 0u ).getVertexFormat()
					);

					m_pSampler = framework::getGraphicsSystem().createSamplerState(
						AddressMode_Clamp,
						AddressMode_Clamp,
						AddressMode_Clamp,
						FilterMode_Linear,
						FilterMode_Linear
					);

					m_vertexConstantBuffer.create( framework::getGraphicsSystem(), sizeof( FallbackVertexConstants ) );

					return TransitionState_Finish;
				}
				else
				{
					// TODO: make resource manager async

					return TransitionState_Error;
				}
			}
			else
			{
				TIKI_ASSERT( isInital );

				framework::getResourceManager().unloadResource< ShaderSet >( m_pShaderSet );
				framework::getResourceManager().unloadResource< Texture >( m_pTexture );
				framework::getResourceManager().unloadResource< Model >( m_pModel );

				framework::getGraphicsSystem().disposeVertexInputBinding( m_pInputBinding );
				framework::getGraphicsSystem().disposeVertexFormat( m_pVertexFormat );
				
				framework::getGraphicsSystem().disposeSamplerState( m_pSampler );

				m_vertexConstantBuffer.dispose( framework::getGraphicsSystem() );

				return TransitionState_Finish;
			}
			break;
		default:
			break;
		}

		TIKI_BREAK( "no result\n" );
		return TransitionState_Error;
	}

	void TestState::update()
	{
	}

	void TestState::render( GraphicsContext& graphicsContext ) const
	{
		graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_GRAY );

		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleList );

		graphicsContext.setVertexShader( m_pShaderSet->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.setPixelShaderTexture( 0u, &m_pTexture->getTextureData() );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSampler );

		graphicsContext.setVertexInputBinding( m_pInputBinding );

		const Vector3 scale = { 0.5f, 1.5f, 0.5f };
		Matrix33 scaleMtx;
		matrix::createScale( scaleMtx, scale );

		Matrix43 mtx;
		matrix::clear( mtx );
		matrix::createRotationY( mtx.rot, (float)framework::getFrameTimer().getTotalTime() );
		//matrix::mul( mtx.rot, scaleMtx );
		//vector::set( mtx.pos, 0.0f, 0.0f, 2.5f );

		Vector3 pos[] = {
			{ -0.5f, -0.5f, 0.0f },
			{  0.0f,  0.5f, 0.0f },
			{  0.5f, -0.5f, 0.0f }
		};

		FallbackVertexConstants* pVertexConstants = static_cast< FallbackVertexConstants* >( graphicsContext.mapBuffer( m_vertexConstantBuffer ) );

		Projection proj;
		proj.createPerspective( 800.0f / 600.0f, f32::piOver4, 0.001f, 100.0f );
		//proj.createOrthographic( 8.0f, 6.0f, 0.001f, 100.0f );

		const Vector3 camPos = { 0.0f, 0.0f, 5.0f };

		Camera cam;
		cam.create( camPos, Quaternion::identity, proj );

		Matrix44 mvp;
		matrix::set( mvp, mtx );
		matrix::mul( mvp, cam.getViewProjectionMatrix() );
		matrix::transpose( mvp );

		Matrix43 mv = mtx;
		matrix::mul( mtx, cam.getViewMatrix() );

		createGraphicsMatrix44( pVertexConstants->mvpMatrix, mvp );
		createGraphicsMatrix44( pVertexConstants->modelViewMatrix, mv );

		graphicsContext.unmapBuffer( m_vertexConstantBuffer );

		graphicsContext.setVertexShaderConstant( 0u, m_vertexConstantBuffer );

		const ModelGeometry& geometry = m_pModel->getGeometryByIndex( 0u );

		graphicsContext.setVertexBuffer( 0u, geometry.getVertexBuffer() );
		graphicsContext.setIndexBuffer( geometry.getIndexBuffer() );

		graphicsContext.drawIndexed( geometry.getIndexCount() );

		//FallbackVertex* pVertices = static_cast< FallbackVertex* >( graphicsContext.beginImmediateGeometry( sizeof( FallbackVertex ), 3u ) );
		//
		//pVertices[ 0u ].position.x = pos[ 0u ].x;
		//pVertices[ 0u ].position.y = pos[ 0u ].y;
		//pVertices[ 0u ].position.z = pos[ 0u ].z;
		//pVertices[ 0u ].texCoord.x = 0.0f;
		//pVertices[ 0u ].texCoord.y = 1.0f;

		//pVertices[ 1u ].position.x = pos[ 1u ].x;
		//pVertices[ 1u ].position.y = pos[ 1u ].y;
		//pVertices[ 1u ].position.z = pos[ 1u ].z;
		//pVertices[ 1u ].texCoord.x = 0.5f;
		//pVertices[ 1u ].texCoord.y = 0.0f;

		//pVertices[ 2u ].position.x = pos[ 2u ].x;
		//pVertices[ 2u ].position.y = pos[ 2u ].y;
		//pVertices[ 2u ].position.z = pos[ 2u ].z;
		//pVertices[ 2u ].texCoord.x = 1.0f;
		//pVertices[ 2u ].texCoord.y = 1.0f;

		//for (uint i = 0u; i < 3u; ++i)
		//{
		//	createFloat4( pVertices[ i ].color, 1.0f, 1.0f, 1.0f, 1.0f );
		//} 
		//
		//graphicsContext.endImmediateGeometry();
	}
}