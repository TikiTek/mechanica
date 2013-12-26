
#include "tiki/gamestates/teststate.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/stockvertex.hpp"
#include "tiki/graphicsresources/model.hpp"
#include "tiki/graphicsresources/shaderset.hpp"
#include "tiki/graphicsresources/texture.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
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
					m_pShaderSet	= framework::getResourceManager().loadResource< ShaderSet >( "immediate.shader" );
					m_pTexture		= framework::getResourceManager().loadResource< Texture >( "checker.texture" );
					m_pModel		= framework::getResourceManager().loadResource< Model >( "replaceme_cube.model" );

					m_pInputBinding	= framework::getGraphicsSystem().createVertexInputBinding(
						m_pShaderSet->getShader( ShaderType_VertexShader, 0u ),
						framework::getGraphicsSystem().getStockVertexFormat( StockVertexFormat_Pos2Tex2 )
					);

					m_pSampler = framework::getGraphicsSystem().createSamplerState(
						AddressMode_Clamp,
						AddressMode_Clamp,
						AddressMode_Clamp,
						FilterMode_Linear,
						FilterMode_Neares
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

				framework::getResourceManager().unloadResource< ShaderSet >( m_pShaderSet );
				framework::getResourceManager().unloadResource< Texture >( m_pTexture );
				framework::getResourceManager().unloadResource< Model >( m_pModel );

				framework::getGraphicsSystem().disposeVertexInputBinding( m_pInputBinding );

				framework::getGraphicsSystem().disposeSamplerState( m_pSampler );

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

		Matrix43 mtx;
		matrix::clear( mtx );
		matrix::createRotationY( mtx.rot, framework::getFrameTimer().getTotalTime() );

		Vector3 pos[] = {
			{ -0.5f, -0.5f, 0.0f },
			{  0.0f,  0.5f, 0.0f },
			{  0.5f, -0.5f, 0.0f }
		};

		for (uint i = 0u; i < TIKI_COUNT( pos ); ++i)
		{
			matrix::transform( pos[ i ], mtx );
		}

		StockVertexPos2Tex2* pVertices = static_cast< StockVertexPos2Tex2* >( graphicsContext.beginImmediateGeometry( sizeof( StockVertexPos2Tex2 ), 3u ) );
		
		pVertices[ 0u ].position.x = pos[ 0u ].x;
		pVertices[ 0u ].position.y = pos[ 0u ].y;
		pVertices[ 0u ].texCoord.x = 0.0f;
		pVertices[ 0u ].texCoord.y = 1.0f;

		pVertices[ 1u ].position.x = pos[ 1u ].x;
		pVertices[ 1u ].position.y = pos[ 1u ].y;
		pVertices[ 1u ].texCoord.x = 0.5f;
		pVertices[ 1u ].texCoord.y = 0.0f;

		pVertices[ 2u ].position.x = pos[ 2u ].x;
		pVertices[ 2u ].position.y = pos[ 2u ].y;
		pVertices[ 2u ].texCoord.x = 1.0f;
		pVertices[ 2u ].texCoord.y = 1.0f;
		
		graphicsContext.endImmediateGeometry();
	}
}