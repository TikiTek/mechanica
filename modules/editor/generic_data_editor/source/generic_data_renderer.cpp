#include "generic_data_renderer.hpp"

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
		m_width		= 100u;
		m_height	= 100u;

		Renderer2dCreationParameters rendererParameters;
		rendererParameters.width				= m_width;
		rendererParameters.height				= m_height;
		rendererParameters.layerCount			= 6u;
		rendererParameters.emissivLayerId		= 4u;
		rendererParameters.drawToWorldFactor	= 100.0f;

		if( !m_renderer.create( graphicsSystem, m_resourceRequests, rendererParameters ) )
		{
			dispose();
			return false;
		}

		resize( m_width, m_height );
		return true;
	}

	void GenericDataRenderer::dispose()
	{
		if( m_pGraphicsSystem == nullptr )
		{
			return;
		}

		m_renderer.dispose( m_resourceRequests );
		m_renderTarget.dispose( *m_pGraphicsSystem );
		m_renderTargetData.dispose( *m_pGraphicsSystem );
		m_resourceRequests.dispose();

		m_pGraphicsSystem = nullptr;
	}

	void GenericDataRenderer::update( float deltaTime )
	{
		m_renderer.update( deltaTime );
	}

	void GenericDataRenderer::render( GraphicsContext& graphicsContext )
	{
		Renderer2dRenderParameters renderParameters;
		renderParameters.pRenderTarget	= &m_renderTarget;

		m_renderer.render( graphicsContext, renderParameters );
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

	void GenericDataRenderer::registerView( const GenericDataTypeStruct* pType, GenericDataView* pView )
	{
		m_views.set( pType, pView );
	}

	void GenericDataRenderer::unregisterView( const GenericDataTypeStruct* pType )
	{
		m_views.remove( pType );
	}

	bool GenericDataRenderer::handleInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}
}
