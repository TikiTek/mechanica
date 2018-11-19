#include "generic_data_renderer.hpp"

#include "tiki/debug_renderer/debug_renderer.hpp"
#include "tiki/generic_data_editor/generic_data_view.hpp"
#include "tiki/graphics/graphics_context.hpp"

namespace tiki
{
	GenericDataRenderer::GenericDataRenderer()
	{
		m_pGraphicsSystem = nullptr;
	}

	GenericDataRenderer::~GenericDataRenderer()
	{
		dispose();
	}

	bool GenericDataRenderer::create( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem )
	{
		m_pGraphicsSystem = &graphicsSystem;
		m_resourceRequests.create( resourceManager );
		uint16 width	= 100u;
		uint16 height	= 100u;

		Renderer2dCreationParameters rendererParameters;
		rendererParameters.width				= width;
		rendererParameters.height				= height;
		rendererParameters.layerCount			= 6u;
		rendererParameters.emissivLayerId		= 4u;
		rendererParameters.drawToWorldFactor	= 100.0f;

		if( !m_renderer.create( graphicsSystem, m_resourceRequests, rendererParameters ) ||
			!m_immediateRenderer.create( graphicsSystem, resourceManager ) )
		{
			dispose();
			return false;
		}

		debugrenderer::initialize( resourceManager );

		return resize( width, height );
	}

	void GenericDataRenderer::dispose()
	{
		if( m_pGraphicsSystem == nullptr )
		{
			return;
		}

		debugrenderer::shutdown( *m_resourceRequests.getResourceManager() );
		m_immediateRenderer.dispose( *m_pGraphicsSystem, *m_resourceRequests.getResourceManager() );

		m_renderer.dispose( m_resourceRequests );
		m_renderTarget.dispose( *m_pGraphicsSystem );
		m_renderTargetData.dispose( *m_pGraphicsSystem );
		m_resourceRequests.dispose();

		m_pGraphicsSystem = nullptr;
	}

	void GenericDataRenderer::update( float deltaTime )
	{
		if( !m_resourceRequests.isFinish() )
		{
			m_resourceRequests.update();
			if( m_resourceRequests.isFinish() )
			{
				TIKI_VERIFY( m_renderer.createResources( *m_pGraphicsSystem, m_resourceRequests ) );
			}
			else
			{
				return;
			}
		}

		//for( GenericDataViewState* pState : m_states )
		//{
		//	pState->pView->update( pState );
		//}

		m_renderer.update( deltaTime );
	}

	void GenericDataRenderer::render( GraphicsContext& graphicsContext )
	{
		if( !m_resourceRequests.isFinish() )
		{
			return;
		}

		graphicsContext.clear( m_renderTarget, TIKI_COLOR_TRANSPARENT );

		for( const ObjectInfoMap::Pair& kvp : m_objectInfos )
		{
			kvp.value.pView->renderObject( m_renderer, kvp.value );
		}

		Renderer2dRenderParameters renderParameters;
		renderParameters.pRenderTarget	= &m_renderTarget;

		m_renderer.render( graphicsContext, renderParameters );

		static uint32 y = 200;
		y = (y + 1) % 200;
		debugrenderer::drawLine( vector::create( 10.0f, 10.0f ), vector::create( 200.0f, float( y ) ), TIKI_COLOR_GREEN );

		m_immediateRenderer.beginRendering( graphicsContext );
		debugrenderer::flush( m_immediateRenderer, m_renderer.getCamera(), &m_renderTarget );
		m_immediateRenderer.endRendering();
	}

	void GenericDataRenderer::setBaseObject( GenericDataObject* pBaseObject )
	{
		if( m_pBaseObject == pBaseObject )
		{
			return;
		}

		m_pBaseObject = pBaseObject;
		m_objectInfos.clear();
	}

	bool GenericDataRenderer::resize( uint16 width, uint16 height )
	{
		if( m_pGraphicsSystem == nullptr )
		{
			return false;
		}

		if( width == m_width && height == m_height )
		{
			return true;
		}

		m_renderTarget.dispose( *m_pGraphicsSystem );
		m_renderTargetData.dispose( *m_pGraphicsSystem );

		TextureDescription textureDescription;
		textureDescription.width	= width;
		textureDescription.height	= height;
		textureDescription.flags	= TextureFlags_RenderTarget | TextureFlags_ShaderInput;
		textureDescription.format	= PixelFormat_R16G16B16A16_Float;
		textureDescription.type		= TextureType_2d;
		if( !m_renderTargetData.create( *m_pGraphicsSystem, textureDescription ) )
		{
			dispose();
			return false;
		}

		RenderTargetBuffer renderTargetBuffer( m_renderTargetData );
		if( !m_renderTarget.create( *m_pGraphicsSystem, width, height, &renderTargetBuffer, 1u, nullptr ) )
		{
			dispose();
			return false;
		}

		if( !m_renderer.resize( width, height ) )
		{
			dispose();
			return false;
		}

		m_width = width;
		m_height = height;
		return true;
	}

	void GenericDataRenderer::registerView( GenericDataView& view )
	{
		m_views.set( view.getObjectType(), &view );
	}

	void GenericDataRenderer::unregisterView( GenericDataView& view )
	{
		m_views.remove( view.getObjectType() );
	}

	bool GenericDataRenderer::handleInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}
}
