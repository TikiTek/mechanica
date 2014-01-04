
#include "tiki/gamestates/teststate.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/gamestates/applicationstate.hpp"
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
#include "tiki/renderer/renderercontext.hpp"
#include "tiki/resource/resourcemanager.hpp"
#include "tiki/framework/inputsystem.hpp"

namespace tiki
{
	struct FallbackVertex
	{
		float3	position;
		float4	color;
		float2	texCoord;
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
					m_pModel		= framework::getResourceManager().loadResource< Model >( "unit_cube.model" );
					m_pShaderSet	= framework::getResourceManager().loadResource< ShaderSet >( "immediate.shader" );
					m_pTexture		= framework::getResourceManager().loadResource< Texture >( "checker.texture" );
					TIKI_ASSERT( m_pModel != nullptr );
					TIKI_ASSERT( m_pShaderSet != nullptr );
					TIKI_ASSERT( m_pTexture != nullptr );

					//VertexAttribute attributes[] =
					//{
					//	{ VertexSementic_Position,	0u, VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
					//	{ VertexSementic_TexCoord,	0u, VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
					//	{ VertexSementic_Color,		0u, VertexAttributeFormat_x32y32z32w32_float,	0u, VertexInputType_PerVertex }
					//};
					//m_pVertexFormat = framework::getGraphicsSystem().createVertexFormat( attributes, TIKI_COUNT( attributes ) );

					m_pInputBinding	= framework::getGraphicsSystem().createVertexInputBinding(
						m_pShaderSet->getShader( ShaderType_VertexShader, 0u ),
						framework::getGraphicsSystem().getStockVertexFormat( StockVertexFormat_Pos2Tex2 ) //m_pModel->getGeometryByIndex( 0u ).getVertexFormat()
					);

					m_pSampler = framework::getGraphicsSystem().createSamplerState(
						AddressMode_Clamp,
						AddressMode_Clamp,
						AddressMode_Clamp,
						FilterMode_Linear,
						FilterMode_Linear
					);

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

				framework::getResourceManager().unloadResource< Texture >( m_pTexture );
				framework::getResourceManager().unloadResource< ShaderSet >( m_pShaderSet );
				framework::getResourceManager().unloadResource< Model >( m_pModel );

				framework::getGraphicsSystem().disposeVertexInputBinding( m_pInputBinding );
				//framework::getGraphicsSystem().disposeVertexFormat( m_pVertexFormat );
				
				framework::getGraphicsSystem().disposeSamplerState( m_pSampler );
				m_pSampler = nullptr;

				return TransitionState_Finish;
			}
			//break;

		case TestStateTransitionSteps_SetRendererValues:
			{
				m_pGameRenderer = &m_pParentState->getGameRenderer();

				if ( isCreating )
				{
					FrameData& frameData = m_pGameRenderer->getFrameData();
					RendererContext& rendererContext = m_pGameRenderer->getRendererContext();

					Projection projection;
					projection.createPerspective(
						(float)rendererContext.rendererWidth / (float)rendererContext.rendererHeight,
						f32::piOver4,
						0.001f,
						100.0f
					);

					const Vector3 cameraPosition = { 0.0f, 0.0f, 5.0f };
					frameData.mainCamera.create( cameraPosition, Quaternion::identity, projection );

					m_fallbackRenderEffect.create( m_pGameRenderer->getRendererContext() );

					m_pGameRenderer->registerRenderEffect( &m_fallbackRenderEffect );

					return TransitionState_Finish;
				}
				else
				{
					m_pGameRenderer->unregisterRenderEffect( &m_fallbackRenderEffect );
					m_fallbackRenderEffect.dispose();

					return TransitionState_Finish;
				}
			}
			//break;

		default:
			break;
		}

		TIKI_BREAK( "no result\n" );
		return TransitionState_Error;
	}

	void TestState::update()
	{
		FrameData& frameData = m_pGameRenderer->getFrameData();
		InputSystem& inputSystem = framework::getInputSystem();

		Vector3 cameraPosition		= frameData.mainCamera.getPosition();
		Quaternion cameraRotation	= frameData.mainCamera.getRotation();

		// move camera
		{
			Vector3 cameraForward;
			Vector3 cameraRight;
			quaternion::getForward( cameraForward, cameraRotation );
			quaternion::getRight( cameraRight, cameraRotation );

			Vector3 cameraBackward	= cameraForward;
			Vector3 cameraLeft		= cameraRight;
			vector::negate( cameraBackward );
			vector::negate( cameraLeft );

			const float timeDelta = (float)framework::getFrameTimer().getElapsedTime();
			vector::scale( cameraForward,	( inputSystem.isKeyDown( KEY_W ) ? 1.0f : 0.0f ) * timeDelta );
			vector::scale( cameraBackward,	( inputSystem.isKeyDown( KEY_S ) ? 1.0f : 0.0f ) * timeDelta );
			vector::scale( cameraLeft,		( inputSystem.isKeyDown( KEY_A ) ? 1.0f : 0.0f ) * timeDelta );
			vector::scale( cameraRight,		( inputSystem.isKeyDown( KEY_D ) ? 1.0f : 0.0f ) * timeDelta );

			vector::add( cameraPosition, cameraForward );
			vector::add( cameraPosition, cameraBackward );
			vector::add( cameraPosition, cameraLeft );
			vector::add( cameraPosition, cameraRight );
		}

		// rotate camera
		{
			const Vector2& mouseDelta = inputSystem.getMouseDeltaNormalized();

			Quaternion cameraRotationDelta;
			quaternion::fromYawPitchRoll( cameraRotationDelta, mouseDelta.x, -mouseDelta.y, 0.0f );

			quaternion::mul( cameraRotation, cameraRotationDelta );
		}

		frameData.mainCamera.setTransform( cameraPosition, cameraRotation );

		Matrix43 mtx;
		matrix::clear( mtx );
		matrix::createRotationY( mtx.rot, (float)framework::getFrameTimer().getTotalTime() );

		m_pGameRenderer->queueModel( m_pModel, &mtx );
	}

	void TestState::render( GraphicsContext& graphicsContext ) const
	{
		m_pGameRenderer->render( graphicsContext );

		//const Vector3 scale = { 0.5f, 1.5f, 0.5f };
		//Matrix33 scaleMtx;
		//matrix::createScale( scaleMtx, scale );

		Matrix43 mtx;
		matrix::clear( mtx );
		matrix::createRotationZ( mtx.rot, (float)framework::getFrameTimer().getTotalTime() );
		////matrix::mul( mtx.rot, scaleMtx );
		////vector::set( mtx.pos, 0.0f, 0.0f, 2.5f );

		Vector3 pos[] = {
			{ -1.0f, -3.0f, 0.0f }, // left bottom
			{ -1.0f,  1.0f, 0.0f },	// mid top
			{  3.0f,  1.0f, 0.0f }  // right bottom
		};

		//for (uint i = 0u; i < TIKI_COUNT( pos ); ++i)
		//{
		//	matrix::transform( pos[ i ], mtx );
		//} 

		//Projection proj;
		//proj.createPerspective( 800.0f / 600.0f, f32::piOver4, 0.001f, 100.0f );
		////proj.createOrthographic( 8.0f, 6.0f, 0.001f, 100.0f );

		//const Vector3 camPos = { 0.0f, 0.0f, 5.0f };

		//Camera cam;
		//cam.create( camPos, Quaternion::identity, proj );

		//Matrix44 mvp;
		//matrix::set( mvp, mtx );
		//matrix::mul( mvp, cam.getViewProjectionMatrix() );
		//matrix::transpose( mvp );

		//Matrix43 mv = mtx;
		//matrix::mul( mtx, cam.getViewMatrix() );

		graphicsContext.beginRenderPass( graphicsContext.getBackBuffer() );
		graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_GRAY );

		graphicsContext.setPixelShaderTexture( 0u, m_pGameRenderer->getGeometryBuffer() + 0u );
		//graphicsContext.setPixelShaderTexture( 0u, &m_pTexture->getTextureData() );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSampler );

		graphicsContext.setVertexInputBinding( m_pInputBinding );

		graphicsContext.setVertexShader( m_pShaderSet->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 0u ) );

		StockVertexPos2Tex2* pVertices = static_cast< StockVertexPos2Tex2* >( graphicsContext.beginImmediateGeometry( sizeof( StockVertexPos2Tex2 ), 3u ) );
		
		pVertices[ 0u ].position.x = pos[ 0u ].x;
		pVertices[ 0u ].position.y = pos[ 0u ].y;
		//pVertices[ 0u ].position.z = pos[ 0u ].z;
		pVertices[ 0u ].texCoord.x = 0.0f;
		pVertices[ 0u ].texCoord.y = 2.0f;

		pVertices[ 1u ].position.x = pos[ 1u ].x;
		pVertices[ 1u ].position.y = pos[ 1u ].y;
		//pVertices[ 1u ].position.z = pos[ 1u ].z;
		pVertices[ 1u ].texCoord.x = 0.0f;
		pVertices[ 1u ].texCoord.y = 0.0f;

		pVertices[ 2u ].position.x = pos[ 2u ].x;
		pVertices[ 2u ].position.y = pos[ 2u ].y;
		//pVertices[ 2u ].position.z = pos[ 2u ].z;
		pVertices[ 2u ].texCoord.x = 2.0f;
		pVertices[ 2u ].texCoord.y = 0.0f;

		//for (uint i = 0u; i < 3u; ++i)
		//{
		//	createFloat4( pVertices[ i ].color, 1.0f, 1.0f, 1.0f, 1.0f );
		//}
		
		graphicsContext.endImmediateGeometry();

		graphicsContext.endRenderPass();
	}
}