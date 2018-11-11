#include "tiki/renderer/renderscene.hpp"

#include "tiki/renderer/renderview.hpp"

namespace tiki
{
	RenderScene::RenderScene()
	{
	}

	RenderScene::~RenderScene()
	{
	}

	bool RenderScene::create( const RenderSceneParameters& parameters )
	{
		if( !m_views.create( parameters.maxViewCount ) ||
			!m_batch.create( parameters.maxRenderSequenceCount, parameters.maxRenderCommandCount ) ||
			!m_directionalLights.create( parameters.maxDirectionalLightCount ) ||
			!m_pointLights.create( parameters.maxPointLightCount ) ||
			!m_spotLights.create( parameters.maxSpotLightCount ) )
		{
			dispose();
			return false;
		}		

		return true;
	}

	void RenderScene::dispose()
	{
		m_views.dispose();

		m_batch.dispose();

		m_directionalLights.dispose();
		m_pointLights.dispose();
		m_spotLights.dispose();
	}

	RenderView* RenderScene::addView( const RenderViewParameters& parameters )
	{
		RenderView* pView = nullptr;
		for( uint i = 0u; i < m_views.getCount(); ++i )
		{
			RenderView& currentView = m_views[ i ];

			if( !currentView.isCreated() )
			{
				pView = &currentView;
				break;
			}
		}

		if( pView != nullptr )
		{
			pView->create( *this, parameters );
		}

		return pView;
	}

	void RenderScene::removeView( RenderView*& pView )
	{
		TIKI_ASSERT( pView != nullptr );
		pView->dispose();
		pView = nullptr;
	}

	DirectionalLightData& RenderScene::addDirectionalLight()
	{
		return m_directionalLights.push();
	}

	PointLightData& RenderScene::addPointLight()
	{
		return m_pointLights.push();
	}

	SpotLightData& RenderScene::addSpotLight()
	{
		return m_spotLights.push();
	}

	void RenderScene::queueModel( const Model* pModel, const Matrix43* pWorldTransform /*= nullptr*/, const SkinningData** ppSkinningData /*= nullptr */ )
	{
		m_batch.queueModel( pModel, pWorldTransform, ppSkinningData );
	}

	void RenderScene::clearState()
	{
		m_batch.reset();

		m_directionalLights.clear();
		m_pointLights.clear();
		m_spotLights.clear();
	}
}