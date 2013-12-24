
#include "tiki/gamestates/teststate.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/stockvertex.hpp"
#include "tiki/graphicsresources/model.hpp"
#include "tiki/graphicsresources/shaderset.hpp"
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
					m_pModel		= framework::getResourceManager().loadResource< Model >( "replaceme_cube.model" );

					m_pInputBinding	= framework::getGraphicsSystem().createVertexInputBinding(
						m_pShaderSet->getShader( ShaderType_VertexShader, 0u ),
						framework::getGraphicsSystem().getStockVertexFormat( StockVertexFormat_Pos2 )
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
				framework::getResourceManager().unloadResource< Model >( m_pModel );

				framework::getGraphicsSystem().disposeVertexInputBinding( m_pInputBinding );

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

		graphicsContext.setVertexInputBinding( m_pInputBinding );

		StockVertexPos2* pVertices = static_cast< StockVertexPos2* >( graphicsContext.beginImmediateGeometry( sizeof( StockVertexPos2 ), 3u ) );
		pVertices[ 0u ].position.x = -0.5f;
		pVertices[ 0u ].position.y = -0.5f;
		pVertices[ 1u ].position.x =  0.0f;
		pVertices[ 1u ].position.y =  0.5f;
		pVertices[ 2u ].position.x =  0.5f;
		pVertices[ 2u ].position.y = -0.5f;
		graphicsContext.endImmediateGeometry();
	}
}